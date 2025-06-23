#include "crypto_lib.h"
#include <openssl/pem.h>
#include <openssl/ec.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <cstring>
#include <stdexcept>

#if defined(_WIN32) || defined(__CYGWIN__)
  #define EXPORT __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
  #define EXPORT __attribute__ ((visibility ("default")))
#else
  #define EXPORT
#endif

// --- Helper: Unique pointers for OpenSSL types ---
template<typename T, void (*Func)(T*)>
using ossl_unique_ptr = std::unique_ptr<T, decltype(Func)>;

template<typename T, void (*Func)(T*)>
using ossl_unique_ptr = std::unique_ptr<T, decltype(Func)>;

using EVP_MD_CTX_ptr = ossl_unique_ptr<EVP_MD_CTX, EVP_MD_CTX_free>;
using BIO_ptr = ossl_unique_ptr<BIO, BIO_free_all>;
using EVP_PKEY_ptr = ossl_unique_ptr<EVP_PKEY, EVP_PKEY_free>;
using EVP_PKEY_CTX_ptr = ossl_unique_ptr<EVP_PKEY_CTX, EVP_PKEY_CTX_free>;
using X509_REQ_ptr = ossl_unique_ptr<X509_REQ, X509_REQ_free>;
using X509_ptr = ossl_unique_ptr<X509, X509_free>;
using X509_NAME_ptr = ossl_unique_ptr<X509_NAME, X509_NAME_free>;
using ASN1_INTEGER_ptr = ossl_unique_ptr<ASN1_INTEGER, ASN1_INTEGER_free>;

// Forward declarations for helpers/functions from other files
// (Ideally, these would be in included headers)
void handle_openssl_error(const char* context);
bool verify_ecdsa(const std::string& public_key_path, const std::string& message_path, const std::string& signature_path);
bool verify_rsapss(const std::string& public_key_path, const std::string& message_path, const std::string& signature_path);

bool read_file_bytes(const std::string& file_path, std::vector<unsigned char>& data);

// --- Error Handling ---
void handle_openssl_error(const char *context) {
    std::cerr << "OpenSSL Error in " << context << ":\n";
    ERR_print_errors_fp(stderr);
}

bool read_file_bytes(const std::string& file_path, std::vector<unsigned char>& data) {
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file for reading: " << file_path << std::endl;
        return false;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    data.resize(size);
    if (!file.read(reinterpret_cast<char*>(data.data()), size)) {
        std::cerr << "Error: Failed to read file: " << file_path << std::endl;
        return false;
    }
    return true;
}

// Helper to load public key (implementation)
EVP_PKEY_ptr load_public_key(const std::string& key_path) {
    BIO_ptr key_bio(BIO_new_file(key_path.c_str(), "rb"), BIO_free_all); // Corrected init
    if (!key_bio) {
        handle_openssl_error("BIO_new_file for loading public key");
        return EVP_PKEY_ptr(nullptr, EVP_PKEY_free); // Corrected return
    }
    EVP_PKEY* pkey_raw = PEM_read_bio_PUBKEY(key_bio.get(), nullptr, nullptr, nullptr);
    if (!pkey_raw) {
        handle_openssl_error("PEM_read_bio_PUBKEY");
        return EVP_PKEY_ptr(nullptr, EVP_PKEY_free); // Corrected return
    }
    return EVP_PKEY_ptr(pkey_raw, EVP_PKEY_free); // Corrected init
}

// --- Verification Implementations ---

bool verify_ecdsa(const std::string& public_key_path, const std::string& message_path, const std::string& signature_path) {
    EVP_PKEY_ptr pkey = load_public_key(public_key_path);
    if (!pkey) {
        return false;
    }

    // Check if the key is ECDSA
    if (EVP_PKEY_base_id(pkey.get()) != EVP_PKEY_EC) {
        std::cerr << "Error: Provided key is not an EC key for ECDSA verification." << std::endl;
        return false;
    }

    std::vector<unsigned char> message_data;
    if (!read_file_bytes(message_path, message_data)) {
        return false;
    }

    std::vector<unsigned char> signature_data;
    if (!read_file_bytes(signature_path, signature_data)) {
        return false;
    }

    EVP_MD_CTX_ptr md_ctx(EVP_MD_CTX_new(), EVP_MD_CTX_free); // Corrected init
    if (!md_ctx) {
        handle_openssl_error("EVP_MD_CTX_new for verification");
        return false;
    }

    const EVP_MD* md = EVP_sha256(); // Must match the digest used for signing

    if (EVP_DigestVerifyInit(md_ctx.get(), nullptr, md, nullptr, pkey.get()) <= 0) {
        handle_openssl_error("EVP_DigestVerifyInit");
        return false;
    }

    if (EVP_DigestVerifyUpdate(md_ctx.get(), message_data.data(), message_data.size()) <= 0) {
        handle_openssl_error("EVP_DigestVerifyUpdate");
        return false;
    }

    // EVP_DigestVerifyFinal returns 1 for success (valid signature), 0 for failure (invalid signature),
    // and a negative value for other errors.
    int verify_result = EVP_DigestVerifyFinal(md_ctx.get(), signature_data.data(), signature_data.size());

    if (verify_result == 1) {
        return true; // Signature is valid
    } else if (verify_result == 0) {
        std::cerr << "Verification failed: Signature is invalid." << std::endl;
        return false; // Signature is invalid
    } else {
        handle_openssl_error("EVP_DigestVerifyFinal");
        return false; // An error occurred during verification
    }
}

bool verify_rsapss(const std::string& public_key_path, const std::string& message_path, const std::string& signature_path) {
    EVP_PKEY_ptr pkey = load_public_key(public_key_path);
    if (!pkey) {
        return false;
    }

    // Check if the key is RSA
    if (EVP_PKEY_base_id(pkey.get()) != EVP_PKEY_RSA) {
        std::cerr << "Error: Provided key is not an RSA key for RSASSA-PSS verification." << std::endl;
        return false;
    }

    std::vector<unsigned char> message_data;
    if (!read_file_bytes(message_path, message_data)) {
        return false;
    }

    std::vector<unsigned char> signature_data;
    if (!read_file_bytes(signature_path, signature_data)) {
        return false;
    }

    // Calculate message hash (SHA256)
    std::vector<unsigned char> digest(EVP_MAX_MD_SIZE);
    unsigned int digest_len = 0;
    const EVP_MD* md = EVP_sha256();
    if (!EVP_Digest(message_data.data(), message_data.size(), digest.data(), &digest_len, md, nullptr)) {
        handle_openssl_error("EVP_Digest (calculate hash for PSS verify)");
        return false;
    }
    digest.resize(digest_len);

    // --- Verify the signature using PSS padding ---
    EVP_PKEY_CTX_ptr pctx(EVP_PKEY_CTX_new(pkey.get(), nullptr), EVP_PKEY_CTX_free); // Corrected init
    if (!pctx) {
        handle_openssl_error("EVP_PKEY_CTX_new for PSS verification");
        return false;
    }

    if (EVP_PKEY_verify_init(pctx.get()) <= 0) {
        handle_openssl_error("EVP_PKEY_verify_init");
        return false;
    }

    // Set padding to PSS
    if (EVP_PKEY_CTX_set_rsa_padding(pctx.get(), RSA_PKCS1_PSS_PADDING) <= 0) {
        handle_openssl_error("EVP_PKEY_CTX_set_rsa_padding (PSS)");
        return false;
    }

    // Set PSS salt length (must match the one used for signing, e.g., -1 for hash length)
    if (EVP_PKEY_CTX_set_rsa_pss_saltlen(pctx.get(), -1) <= 0) {
        handle_openssl_error("EVP_PKEY_CTX_set_rsa_pss_saltlen");
        return false;
    }

    // Set the digest type used for MGF1 and the message hashing (must match signing)
    if (EVP_PKEY_CTX_set_signature_md(pctx.get(), md) <= 0) {
        handle_openssl_error("EVP_PKEY_CTX_set_signature_md");
        return false;
    }

    // Perform verification
    // EVP_PKEY_verify returns 1 for success, 0 for failure, <0 for error
    int verify_result = EVP_PKEY_verify(pctx.get(), signature_data.data(), signature_data.size(), digest.data(), digest.size());

    if (verify_result == 1) {
        return true; // Signature is valid
    } else if (verify_result == 0) {
        std::cerr << "Verification failed: RSASSA-PSS Signature is invalid." << std::endl;
        return false; // Signature is invalid
    } else {
        handle_openssl_error("EVP_PKEY_verify");
        return false; // An error occurred
    }
}

// Helper to load an X.509 certificate from a file
X509_ptr load_certificate(const std::string& cert_path) {
    BIO_ptr cert_bio(BIO_new_file(cert_path.c_str(), "rb"), BIO_free_all); // Corrected init
    if (!cert_bio) {
        handle_openssl_error("BIO_new_file for loading certificate");
        return X509_ptr(nullptr, X509_free); // Corrected return
    }
    X509* cert_raw = PEM_read_bio_X509(cert_bio.get(), nullptr, nullptr, nullptr);
    if (!cert_raw) {
        handle_openssl_error("PEM_read_bio_X509");
        return X509_ptr(nullptr, X509_free); // Corrected return
    }
    return X509_ptr(cert_raw, X509_free); // Corrected init
}

// --- X.509 Operations Implementations ---

bool extract_pubkey_from_cert(const std::string& certificate_path, const std::string& public_key_path) {
    X509_ptr cert = load_certificate(certificate_path);
    if (!cert) {
        return false;
    }

    // Extract the public key from the certificate
    // X509_get_pubkey returns an internal pointer, but increments its reference count.
    // It needs to be freed with EVP_PKEY_free.
    EVP_PKEY* pkey_raw = X509_get_pubkey(cert.get());
    if (!pkey_raw) {
        handle_openssl_error("X509_get_pubkey");
        return false;
    }
    EVP_PKEY_ptr pkey(pkey_raw, EVP_PKEY_free); // Corrected init

    // Write the extracted public key to the specified file
    BIO_ptr pub_bio(BIO_new_file(public_key_path.c_str(), "wb"), BIO_free_all); // Corrected init
    if (!pub_bio) {
        handle_openssl_error("BIO_new_file for extracted public key");
        return false;
    }
    if (PEM_write_bio_PUBKEY(pub_bio.get(), pkey.get()) != 1) {
        handle_openssl_error("PEM_write_bio_PUBKEY for extracted key");
        return false;
    }

    return true;
}

bool verify_signature_with_cert(const std::string& certificate_path, const std::string& message_path, const std::string& signature_path) {
    X509_ptr cert = load_certificate(certificate_path);
    if (!cert) {
        return false;
    }

    // Extract the public key from the certificate
    EVP_PKEY* pkey_raw = X509_get_pubkey(cert.get());
     if (!pkey_raw) {
        handle_openssl_error("X509_get_pubkey for verification");
        return false;
    }
    EVP_PKEY_ptr pkey(pkey_raw, EVP_PKEY_free); // Corrected init

    // Determine the key type and call the appropriate verification function
    int key_type = EVP_PKEY_base_id(pkey.get());

    // We need to write the extracted public key to a temporary file
    // because our existing verify functions expect a file path.
    // A more integrated approach would pass the EVP_PKEY* directly.
    std::string temp_pubkey_path = "temp_cert_pubkey.pem"; // Use a more robust temp file mechanism in production
    BIO_ptr temp_pub_bio(BIO_new_file(temp_pubkey_path.c_str(), "wb"), BIO_free_all); // Corrected init
    if (!temp_pub_bio) {
        handle_openssl_error("BIO_new_file for temporary public key");
        return false;
    }
    if (PEM_write_bio_PUBKEY(temp_pub_bio.get(), pkey.get()) != 1) {
        handle_openssl_error("PEM_write_bio_PUBKEY for temporary key");
        remove(temp_pubkey_path.c_str()); // Clean up temp file on error
        return false;
    }
    // Ensure the BIO is flushed before proceeding
    BIO_flush(temp_pub_bio.get());
    // Close the BIO to ensure file is written before reading
    temp_pub_bio.reset(); // This calls BIO_free_all

    bool result = false;
    if (key_type == EVP_PKEY_EC) {
        std::cout << "Verifying using ECDSA (key from certificate)..." << std::endl;
        result = verify_ecdsa(temp_pubkey_path, message_path, signature_path);
    } else if (key_type == EVP_PKEY_RSA) {
        std::cout << "Verifying using RSASSA-PSS (key from certificate)..." << std::endl;
        // NOTE: Assumes the signature was created with PSS padding if the key is RSA.
        // A more robust system might need metadata about the signature algorithm.
        result = verify_rsapss(temp_pubkey_path, message_path, signature_path);
    } else {
        std::cerr << "Error: Unsupported key type in certificate for verification (" << OBJ_nid2sn(key_type) << ")." << std::endl;
        result = false;
    }

    // Clean up the temporary file
    remove(temp_pubkey_path.c_str());

    return result;
}

void print_usage() {
    std::cerr << "  --verify-cert <cert_in> <der/pem> <message_in> <signature_in> <pubkey_out>" << std::endl;
    std::cerr << "      Example: --verify-cert server.crt pem message.txt message.rsapss.sig cert_pubkey.pem" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::string command = argv[1];
    bool success = false;

    try {
        if (command == "--verify-cert" && argc == 6) {
            success = verify_signature_with_cert(argv[2], argv[4], argv[5]);
            if (success) {
                std::cout << "Certificate-based Verification Successful." << std::endl;
                extract_pubkey_from_cert(argv[2], argv[6]);
            } else {
                 // Error message printed within verify_signature_with_cert
            }
            // Don't print generic success/failure message for verify
            return success ? 0 : 1;
        } else {
            std::cerr << "Error: Unknown command or incorrect number of arguments for " << command << std::endl;
            print_usage();
            return 1;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid numeric argument provided: " << e.what() << std::endl;
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Numeric argument out of range: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
        return 1;
    }

    if (success) {
        std::cout << "Operation \"" << command << "\" completed successfully." << std::endl;
        return 0;
    } else {
        std::cerr << "Operation \"" << command << "\" failed." << std::endl;
        // Specific errors should have been printed by the functions or handle_openssl_error
        return 1;
    }
}