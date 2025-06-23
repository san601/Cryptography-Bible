// src/key_generation.cpp
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
 
// C-style exports for DLL
#ifdef __cplusplus
extern "C" {
#endif
 
EXPORT bool generate_ecdsa_keypair(const char *curve_name_chr, const char *private_key_path_chr, const char *public_key_path_chr);
EXPORT bool generate_rsa_keypair(int bits, const char *private_key_path_chr, const char *public_key_path_chr);
EXPORT bool sign_ecdsa(const char *private_key_path_chr, const char *message_path_chr, const char *signature_path_chr);
EXPORT bool sign_rsapss(const char *private_key_path_chr, const char *message_path_chr, const char *signature_path_chr);
EXPORT bool verify_ecdsa(const char *public_key_path_chr, const char *message_path_chr, const char *signature_path_chr);
EXPORT bool verify_rsapss(const char *public_key_path_chr, const char *message_path_chr, const char *signature_path_chr);
 
#ifdef __cplusplus
}
#endif


// --- Helper: Unique pointers for OpenSSL types ---
template<typename T, void (*Func)(T*)>
using ossl_unique_ptr = std::unique_ptr<T, decltype(Func)>;

using BIO_ptr = ossl_unique_ptr<BIO, BIO_free_all>;
using EVP_PKEY_ptr = ossl_unique_ptr<EVP_PKEY, EVP_PKEY_free>;
using EVP_PKEY_CTX_ptr = ossl_unique_ptr<EVP_PKEY_CTX, EVP_PKEY_CTX_free>;
using X509_REQ_ptr = ossl_unique_ptr<X509_REQ, X509_REQ_free>;
using X509_ptr = ossl_unique_ptr<X509, X509_free>;
using X509_NAME_ptr = ossl_unique_ptr<X509_NAME, X509_NAME_free>;
using ASN1_INTEGER_ptr = ossl_unique_ptr<ASN1_INTEGER, ASN1_INTEGER_free>;
using EVP_MD_CTX_ptr = ossl_unique_ptr<EVP_MD_CTX, EVP_MD_CTX_free>;


// Helper to load private key (moved to helpers.cpp, but keep a local version for now if needed)
// Or better: ensure helpers.cpp is linked correctly and remove this duplicate.
// Assuming helpers.cpp is linked, we declare it here:
EVP_PKEY_ptr load_private_key(const std::string& key_path);

// --- File I/O Helpers ---
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

bool write_file_bytes(const std::string& file_path, const std::vector<unsigned char>& data) {
    std::ofstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file for writing: " << file_path << std::endl;
        return false;
    }
    if (!file.write(reinterpret_cast<const char*>(data.data()), data.size())) {
        std::cerr << "Error: Failed to write to file: " << file_path << std::endl;
        return false;
    }
    return true;
}


// --- Error Handling ---
void handle_openssl_error(const char *context) {
    std::cerr << "OpenSSL Error in " << context << ":\n";
    ERR_print_errors_fp(stderr);
}

// --- Base64 Helper ---
std::string base64_encode(const std::vector<unsigned char>& input) {
    BIO_ptr b64(BIO_new(BIO_f_base64()), BIO_free_all);
    BIO_ptr bmem(BIO_new(BIO_s_mem()), BIO_free_all);
    BIO_set_flags(b64.get(), BIO_FLAGS_BASE64_NO_NL); // No newlines in output
    BIO_push(b64.get(), bmem.get());

    if (BIO_write(b64.get(), input.data(), input.size()) <= 0) {
        handle_openssl_error("BIO_write for base64 encode");
        return "";
    }
    if (BIO_flush(b64.get()) != 1) {
        handle_openssl_error("BIO_flush for base64 encode");
        return "";
    }

    BUF_MEM* bptr = nullptr;
    BIO_get_mem_ptr(bmem.get(), &bptr);
    if (!bptr || !bptr->data || bptr->length == 0) {
        handle_openssl_error("BIO_get_mem_ptr for base64 encode");
        return "";
    }

    std::string result(bptr->data, bptr->length);
    return result;
}

// --- Key Loading Helper ---
// Defined in key_generation.cpp, but needed here too. Ideally in helpers.cpp
// For now, we duplicate the definition. A better approach is linking object files.
EVP_PKEY_ptr load_private_key(const std::string& key_path) {
    BIO_ptr key_bio(BIO_new_file(key_path.c_str(), "rb"), BIO_free_all);
    if (!key_bio) {
        handle_openssl_error("BIO_new_file for loading private key");
        return EVP_PKEY_ptr(nullptr, EVP_PKEY_free); // Corrected return for unique_ptr
    }
    // Try reading as PKCS8 first, then specific types if needed
    EVP_PKEY* pkey_raw = PEM_read_bio_PrivateKey(key_bio.get(), nullptr, nullptr, nullptr);
    if (!pkey_raw) {
        // Reset BIO position if first read failed
        BIO_ctrl(key_bio.get(), BIO_CTRL_RESET, 0, nullptr);
        // Try reading as RSA specifically (older format)
        RSA* rsa_raw = PEM_read_bio_RSAPrivateKey(key_bio.get(), nullptr, nullptr, nullptr);
        if (rsa_raw) {
            pkey_raw = EVP_PKEY_new();
            if (pkey_raw && EVP_PKEY_assign_RSA(pkey_raw, rsa_raw)) {
                // Success, EVP_PKEY owns rsa_raw now
            } else {
                RSA_free(rsa_raw);
                EVP_PKEY_free(pkey_raw);
                pkey_raw = nullptr;
            }
        } else {
             // Reset BIO position again
             BIO_ctrl(key_bio.get(), BIO_CTRL_RESET, 0, nullptr);
             // Try reading as EC specifically
             EC_KEY* ec_raw = PEM_read_bio_ECPrivateKey(key_bio.get(), nullptr, nullptr, nullptr);
             if (ec_raw) {
                 pkey_raw = EVP_PKEY_new();
                 if (pkey_raw && EVP_PKEY_assign_EC_KEY(pkey_raw, ec_raw)) {
                     // Success, EVP_PKEY owns ec_raw now
                 } else {
                     EC_KEY_free(ec_raw);
                     EVP_PKEY_free(pkey_raw);
                     pkey_raw = nullptr;
                 }
             }
        }
    }

    if (!pkey_raw) {
        handle_openssl_error("Failed to read private key (tried PKCS8, RSA, EC)");
        return EVP_PKEY_ptr(nullptr, EVP_PKEY_free); // Corrected return for unique_ptr
    }
    return EVP_PKEY_ptr(pkey_raw, EVP_PKEY_free);
}

// --- Key Generation Implementations ---

bool generate_ecdsa_keypair(const char *curve_name_chr, const char *private_key_path_chr, const char *public_key_path_chr) {


    EVP_PKEY_CTX_ptr pctx(EVP_PKEY_CTX_new_id(EVP_PKEY_EC, nullptr), EVP_PKEY_CTX_free);

    std::string curve_name(curve_name_chr);
    std::string private_key_path(private_key_path_chr);
    std::string public_key_path(public_key_path_chr);

    if (!pctx) {
        handle_openssl_error("EVP_PKEY_CTX_new_id for EC");
        return false;
    }

    if (EVP_PKEY_keygen_init(pctx.get()) <= 0) {
        handle_openssl_error("EVP_PKEY_keygen_init");
        return false;
    }

    if (EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pctx.get(), OBJ_txt2nid(curve_name.c_str())) <= 0) {
        handle_openssl_error("EVP_PKEY_CTX_set_ec_paramgen_curve_nid");
        return false;
    }

    EVP_PKEY* pkey_raw = nullptr;
    if (EVP_PKEY_keygen(pctx.get(), &pkey_raw) <= 0) {
        handle_openssl_error("EVP_PKEY_keygen");
        return false;
    }
    EVP_PKEY_ptr pkey(pkey_raw, EVP_PKEY_free); // Corrected init

    // Write private key
    BIO_ptr priv_bio(BIO_new_file(private_key_path.c_str(), "wb"), BIO_free_all); // Corrected init
    if (!priv_bio) {
        handle_openssl_error("BIO_new_file for private key");
        return false;
    }
    if (PEM_write_bio_PrivateKey(priv_bio.get(), pkey.get(), nullptr, nullptr, 0, nullptr, nullptr) != 1) {
        handle_openssl_error("PEM_write_bio_PrivateKey");
        return false;
    }

    // Write public key
    BIO_ptr pub_bio(BIO_new_file(public_key_path.c_str(), "wb"), BIO_free_all); // Corrected init
    if (!pub_bio) {
        handle_openssl_error("BIO_new_file for public key");
        return false;
    }
    if (PEM_write_bio_PUBKEY(pub_bio.get(), pkey.get()) != 1) {
        handle_openssl_error("PEM_write_bio_PUBKEY");
        return false;
    }

    return true;
}

bool generate_rsa_keypair(int bits, const char *private_key_path_chr, const char *public_key_path_chr) {
    EVP_PKEY_CTX_ptr pctx(EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr), EVP_PKEY_CTX_free);

    std::string private_key_path(private_key_path_chr);
    std::string public_key_path(public_key_path_chr);

    if (!pctx) {
        handle_openssl_error("EVP_PKEY_CTX_new_id for RSA");
        return false;
    }

    if (EVP_PKEY_keygen_init(pctx.get()) <= 0) {
        handle_openssl_error("EVP_PKEY_keygen_init");
        return false;
    }

    if (EVP_PKEY_CTX_set_rsa_keygen_bits(pctx.get(), bits) <= 0) {
        handle_openssl_error("EVP_PKEY_CTX_set_rsa_keygen_bits");
        return false;
    }

    EVP_PKEY *pkey_raw = nullptr;
    if (EVP_PKEY_keygen(pctx.get(), &pkey_raw) <= 0) {
        handle_openssl_error("EVP_PKEY_keygen");
        return false;
    }
    EVP_PKEY_ptr pkey(pkey_raw, EVP_PKEY_free); // Corrected init

    // Write private key
    BIO_ptr priv_bio(BIO_new_file(private_key_path.c_str(), "wb"), BIO_free_all); // Corrected init
    if (!priv_bio) {
        handle_openssl_error("BIO_new_file for private key");
        return false;
    }
    // Use traditional format for broader compatibility, can use PKCS8 as well
    // Note: EVP_PKEY_get0_RSA returns an internal pointer, do not free it.
    const RSA* rsa_key = EVP_PKEY_get0_RSA(pkey.get());
    if (!rsa_key) {
        handle_openssl_error("EVP_PKEY_get0_RSA");
        return false;
    }
    if (PEM_write_bio_RSAPrivateKey(priv_bio.get(), rsa_key, nullptr, nullptr, 0, nullptr, nullptr) != 1) {
         handle_openssl_error("PEM_write_bio_RSAPrivateKey");
         return false;
    }

    // Write public key
    BIO_ptr pub_bio(BIO_new_file(public_key_path.c_str(), "wb"), BIO_free_all); // Corrected init
    if (!pub_bio) {
        handle_openssl_error("BIO_new_file for public key");
        return false;
    }
    if (PEM_write_bio_PUBKEY(pub_bio.get(), pkey.get()) != 1) {
        handle_openssl_error("PEM_write_bio_PUBKEY");
        return false;
    }

    return true;
}

bool sign_ecdsa(const char *private_key_path_chr, const char *message_path_chr, const char *signature_path_chr) {
    std::string private_key_path(private_key_path_chr);
    std::string message_path(message_path_chr);
    std::string signature_path(signature_path_chr);

    EVP_PKEY_ptr pkey = load_private_key(private_key_path);
    if (!pkey) {
        return false;
    }

    // Check if the key is ECDSA
    if (EVP_PKEY_base_id(pkey.get()) != EVP_PKEY_EC) {
        std::cerr << "Error: Provided key is not an EC key for ECDSA signing." << std::endl;
        return false;
    }

    std::vector<unsigned char> message_data;
    if (!read_file_bytes(message_path, message_data)) {
        return false;
    }

    EVP_MD_CTX_ptr md_ctx(EVP_MD_CTX_new(), EVP_MD_CTX_free);
    if (!md_ctx) {
        handle_openssl_error("EVP_MD_CTX_new");
        return false;
    }

    const EVP_MD* md = EVP_sha256(); // Use SHA256

    if (EVP_DigestSignInit(md_ctx.get(), nullptr, md, nullptr, pkey.get()) <= 0) {
        handle_openssl_error("EVP_DigestSignInit");
        return false;
    }

    if (EVP_DigestSignUpdate(md_ctx.get(), message_data.data(), message_data.size()) <= 0) {
        handle_openssl_error("EVP_DigestSignUpdate");
        return false;
    }

    // Determine required buffer size for signature
    size_t sig_len = 0;
    if (EVP_DigestSignFinal(md_ctx.get(), nullptr, &sig_len) <= 0) {
        handle_openssl_error("EVP_DigestSignFinal (get size)");
        return false;
    }

    std::vector<unsigned char> signature(sig_len);
    if (EVP_DigestSignFinal(md_ctx.get(), signature.data(), &sig_len) <= 0) {
        handle_openssl_error("EVP_DigestSignFinal (sign)");
        return false;
    }
    signature.resize(sig_len); // Adjust size if needed

    if (!write_file_bytes(signature_path, signature)) {
        return false;
    }

    return true;
}

bool sign_rsapss(const char *private_key_path_chr, const char *message_path_chr, const char *signature_path_chr) {
    std::string private_key_path(private_key_path_chr);
    std::string message_path(message_path_chr);
    std::string signature_path(signature_path_chr);

    EVP_PKEY_ptr pkey = load_private_key(private_key_path);
    if (!pkey) {
        return false;
    }

    // Check if the key is RSA
    if (EVP_PKEY_base_id(pkey.get()) != EVP_PKEY_RSA) {
        std::cerr << "Error: Provided key is not an RSA key for RSASSA-PSS signing." << std::endl;
        return false;
    }

    std::vector<unsigned char> message_data;
    if (!read_file_bytes(message_path, message_data)) {
        return false;
    }

    // Calculate message hash (SHA256)
    std::vector<unsigned char> digest(EVP_MAX_MD_SIZE);
    unsigned int digest_len = 0;
    const EVP_MD* md = EVP_sha256();
    if (!EVP_Digest(message_data.data(), message_data.size(), digest.data(), &digest_len, md, nullptr)) {
        handle_openssl_error("EVP_Digest (calculate hash for PSS)");
        return false;
    }
    digest.resize(digest_len);

    // --- Sign the hash using PSS padding ---
    EVP_PKEY_CTX_ptr pctx(EVP_PKEY_CTX_new(pkey.get(), nullptr), EVP_PKEY_CTX_free);
    if (!pctx) {
        handle_openssl_error("EVP_PKEY_CTX_new for PSS signing");
        return false;
    }

    if (EVP_PKEY_sign_init(pctx.get()) <= 0) {
        handle_openssl_error("EVP_PKEY_sign_init");
        return false;
    }

    // Set padding to PSS
    if (EVP_PKEY_CTX_set_rsa_padding(pctx.get(), RSA_PKCS1_PSS_PADDING) <= 0) {
        handle_openssl_error("EVP_PKEY_CTX_set_rsa_padding (PSS)");
        return false;
    }

    // Set PSS salt length (e.g., match hash length)
    if (EVP_PKEY_CTX_set_rsa_pss_saltlen(pctx.get(), -1) <= 0) { // -1 means salt len = hash len
        handle_openssl_error("EVP_PKEY_CTX_set_rsa_pss_saltlen");
        return false;
    }

    // Set the digest type used for MGF1 and the message hashing (must match)
    if (EVP_PKEY_CTX_set_signature_md(pctx.get(), md) <= 0) {
        handle_openssl_error("EVP_PKEY_CTX_set_signature_md");
        return false;
    }

    // Determine signature length
    size_t sig_len = 0;
    if (EVP_PKEY_sign(pctx.get(), nullptr, &sig_len, digest.data(), digest.size()) <= 0) {
        handle_openssl_error("EVP_PKEY_sign (get size)");
        return false;
    }

    std::vector<unsigned char> signature(sig_len);

    // Perform signing
    if (EVP_PKEY_sign(pctx.get(), signature.data(), &sig_len, digest.data(), digest.size()) <= 0) {
        handle_openssl_error("EVP_PKEY_sign (sign)");
        return false;
    }
    signature.resize(sig_len); // Adjust size

    if (!write_file_bytes(signature_path, signature)) {
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

bool verify_ecdsa(const char *public_key_path_chr, const char *message_path_chr, const char *signature_path_chr) {
    std::string public_key_path(public_key_path_chr);
    std::string message_path(message_path_chr);
    std::string signature_path(signature_path_chr);
    
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

bool verify_rsapss(const char *public_key_path_chr, const char *message_path_chr, const char *signature_path_chr) {
    std::string public_key_path(public_key_path_chr);
    std::string message_path(message_path_chr);
    std::string signature_path(signature_path_chr);

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

void print_usage() {
    std::cerr << "Usage: digital_signature <command> [options]" << std::endl;
    std::cerr << "Commands & Options:" << std::endl;
    std::cerr << "  --gen-ecdsa <curve> <privkey_out> <pubkey_out>" << std::endl;
    std::cerr << "      Example: --gen-ecdsa prime256v1 ecdsa_priv.pem ecdsa_pub.pem" << std::endl;
    std::cerr << "  --gen-rsa <bits> <privkey_out> <pubkey_out>" << std::endl;
    std::cerr << "      Example: --gen-rsa 2048 rsa_priv.pem rsa_pub.pem" << std::endl;
    std::cerr << "  --sign-ecdsa <privkey> <message_file> <signature_out>" << std::endl;
    std::cerr << "      Example: --sign-ecdsa ecdsa_priv.pem message.txt ecdsa_signature.bin" << std::endl;
    std::cerr << "  --sign-rsapss <privkey> <message_file> <signature_out>" << std::endl;
    std::cerr << "      Example: --sign-rsapss rsa_priv.pem message.txt rsa_signature.bin" << std::endl;
    std::cerr << "  --verify-ecdsa <pubkey> <message_file> <signature_file>" << std::endl;
    std::cerr << "      Example: --verify-ecdsa ecdsa_pub.pem message.txt ecdsa_signature.bin" << std::endl;
    std::cerr << "  --verify-rsapss <pubkey> <message_file> <signature_file>" << std::endl;
    std::cerr << "      Example: --verify-rsapss rsa_pub.pem message.txt rsa_signature.bin" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::string command = argv[1];
    bool success = false;

    try {
        if (command == "--gen-ecdsa" && argc == 5) {
            success = generate_ecdsa_keypair(argv[2], argv[3], argv[4]);
        } else if (command == "--gen-rsa" && argc == 5) {
            int bits = std::stoi(argv[2]);
            success = generate_rsa_keypair(bits, argv[3], argv[4]);
        } else if (command == "--sign-ecdsa" && argc == 5) {
            
            char* private_key_path = argv[2];
            char* message_path = argv[3];
            char* signature_path = argv[4];
            success = sign_ecdsa(private_key_path, message_path, signature_path);
        } else if (command == "--sign-rsapss" && argc == 5) {
            char* private_key_path = argv[2];
            char* message_path = argv[3];
            char* signature_path = argv[4];
            success = sign_rsapss(private_key_path, message_path, signature_path);
        } else if (command == "--verify-ecdsa" && argc == 5) {
            char* public_key_path = argv[2];
            char* message_path = argv[3];
            char* signature_path = argv[4];
            success = verify_ecdsa(public_key_path, message_path, signature_path);
            if (!success) {
                std::cerr << "ECDSA verification failed." << std::endl;
            } else {
                std::cout << "ECDSA verification succeeded." << std::endl;
            }
        } else if (command == "--verify-rsapss" && argc == 5) {
            char* public_key_path = argv[2];
            char* message_path = argv[3];
            char* signature_path = argv[4];
            success = verify_rsapss(public_key_path, message_path, signature_path);
            if (!success) {
                std::cerr << "RSASSA-PSS verification failed." << std::endl;
            } else {
                std::cout << "RSASSA-PSS verification succeeded." << std::endl;
            }
        } else if (command == "--help") {
            print_usage();
            return 0;
        } else {
            std::cout << "Request not supported" << std::endl;
        }
        return success ? 0 : 1;

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
}