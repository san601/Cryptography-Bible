#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/asn1.h>
#include <openssl/bn.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <iomanip>

// Smart pointer types for OpenSSL objects
template<typename T, void (*Func)(T*)>
using ossl_unique_ptr = std::unique_ptr<T, decltype(Func)>;

using BIO_ptr = ossl_unique_ptr<BIO, BIO_free_all>;
using X509_ptr = ossl_unique_ptr<X509, X509_free>;
using EVP_PKEY_ptr = ossl_unique_ptr<EVP_PKEY, EVP_PKEY_free>;
using X509_NAME_ptr = std::unique_ptr<X509_NAME, void(*)(X509_NAME*)>;

// Certificate information structure
struct CertificateInfo {
    std::string subject_name;
    std::string issuer_name;
    std::string serial_number;
    std::string valid_from;
    std::string valid_to;
    std::string signature_algorithm;
    std::string public_key_algorithm;
    std::string public_key_info;
    std::vector<std::string> key_usage;
    std::vector<std::string> extended_key_usage;
    std::vector<std::string> subject_alt_names;
    std::string fingerprint_sha1;
    std::string fingerprint_sha256;
    int version;
    bool is_ca;
    bool signature_valid;
};

// Error handling function
void handle_openssl_error(const char* context) {
    std::cerr << "OpenSSL Error in " << context << ":\n";
    ERR_print_errors_fp(stderr);
}

// Convert ASN1_TIME to readable string
std::string asn1_time_to_string(const ASN1_TIME* time) {
    if (!time) return "Invalid";
    
    BIO_ptr bio(BIO_new(BIO_s_mem()), BIO_free_all);
    if (!bio) return "Error";
    
    if (ASN1_TIME_print(bio.get(), time) <= 0) {
        return "Error";
    }
    
    char* data;
    long len = BIO_get_mem_data(bio.get(), &data);
    return std::string(data, len);
}

// Convert X509_NAME to string
std::string x509_name_to_string(X509_NAME* name) {
    if (!name) return "Invalid";
    
    BIO_ptr bio(BIO_new(BIO_s_mem()), BIO_free_all);
    if (!bio) return "Error";
    
    if (X509_NAME_print_ex(bio.get(), name, 0, XN_FLAG_ONELINE) <= 0) {
        return "Error";
    }
    
    char* data;
    long len = BIO_get_mem_data(bio.get(), &data);
    return std::string(data, len);
}

// Convert ASN1_INTEGER to hex string
std::string asn1_integer_to_hex(const ASN1_INTEGER* ai) {
    if (!ai) return "Invalid";
    
    BIGNUM* bn = ASN1_INTEGER_to_BN(ai, nullptr);
    if (!bn) return "Error";
    
    char* hex_str = BN_bn2hex(bn);
    if (!hex_str) {
        BN_free(bn);
        return "Error";
    }
    
    std::string result(hex_str);
    OPENSSL_free(hex_str);
    BN_free(bn);
    return result;
}

// Get certificate fingerprint
std::string get_certificate_fingerprint(X509* cert, const EVP_MD* md) {
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len;
    
    if (X509_digest(cert, md, digest, &digest_len) != 1) {
        return "Error";
    }
    
    std::stringstream ss;
    for (unsigned int i = 0; i < digest_len; i++) {
        ss << std::hex << std::setfill('0') << std::setw(2) << (int)digest[i];
        if (i < digest_len - 1) ss << ":";
    }
    return ss.str();
}

// Get public key information
std::string get_public_key_info(EVP_PKEY* pkey) {
    if (!pkey) return "Invalid";
    
    std::stringstream info;
    int key_type = EVP_PKEY_base_id(pkey);
    
    switch (key_type) {
        case EVP_PKEY_RSA: {
            info << "RSA, ";
            int key_size = EVP_PKEY_bits(pkey);
            info << key_size << " bits";
            break;
        }
        case EVP_PKEY_EC: {
            info << "EC, ";
            int key_size = EVP_PKEY_bits(pkey);
            info << key_size << " bits";
            break;
        }
        case EVP_PKEY_DSA: {
            info << "DSA, ";
            int key_size = EVP_PKEY_bits(pkey);
            info << key_size << " bits";
            break;
        }
        default:
            info << "Unknown key type (" << key_type << ")";
    }
    
    return info.str();
}

// Parse key usage extensions
std::vector<std::string> parse_key_usage(X509* cert) {
    std::vector<std::string> usage;
    
    ASN1_BIT_STRING* key_usage = (ASN1_BIT_STRING*)X509_get_ext_d2i(cert, NID_key_usage, nullptr, nullptr);
    if (key_usage) {
        if (ASN1_BIT_STRING_get_bit(key_usage, 0)) usage.push_back("Digital Signature");
        if (ASN1_BIT_STRING_get_bit(key_usage, 1)) usage.push_back("Non Repudiation");
        if (ASN1_BIT_STRING_get_bit(key_usage, 2)) usage.push_back("Key Encipherment");
        if (ASN1_BIT_STRING_get_bit(key_usage, 3)) usage.push_back("Data Encipherment");
        if (ASN1_BIT_STRING_get_bit(key_usage, 4)) usage.push_back("Key Agreement");
        if (ASN1_BIT_STRING_get_bit(key_usage, 5)) usage.push_back("Certificate Sign");
        if (ASN1_BIT_STRING_get_bit(key_usage, 6)) usage.push_back("CRL Sign");
        if (ASN1_BIT_STRING_get_bit(key_usage, 7)) usage.push_back("Encipher Only");
        if (ASN1_BIT_STRING_get_bit(key_usage, 8)) usage.push_back("Decipher Only");
        ASN1_BIT_STRING_free(key_usage);
    }
    
    return usage;
}

// Parse extended key usage
std::vector<std::string> parse_extended_key_usage(X509* cert) {
    std::vector<std::string> ext_usage;
    
    EXTENDED_KEY_USAGE* eku = (EXTENDED_KEY_USAGE*)X509_get_ext_d2i(cert, NID_ext_key_usage, nullptr, nullptr);
    if (eku) {
        int count = sk_ASN1_OBJECT_num(eku);
        for (int i = 0; i < count; i++) {
            ASN1_OBJECT* obj = sk_ASN1_OBJECT_value(eku, i);
            char buf[256];
            OBJ_obj2txt(buf, sizeof(buf), obj, 0);
            ext_usage.push_back(std::string(buf));
        }
        EXTENDED_KEY_USAGE_free(eku);
    }
    
    return ext_usage;
}

// Parse Subject Alternative Names
std::vector<std::string> parse_subject_alt_names(X509* cert) {
    std::vector<std::string> san_list;
    
    GENERAL_NAMES* san_names = (GENERAL_NAMES*)X509_get_ext_d2i(cert, NID_subject_alt_name, nullptr, nullptr);
    if (san_names) {
        int count = sk_GENERAL_NAME_num(san_names);
        for (int i = 0; i < count; i++) {
            GENERAL_NAME* gen_name = sk_GENERAL_NAME_value(san_names, i);
            if (gen_name->type == GEN_DNS) {
                char* dns_name = (char*)ASN1_STRING_get0_data(gen_name->d.dNSName);
                san_list.push_back("DNS:" + std::string(dns_name));
            } else if (gen_name->type == GEN_EMAIL) {
                char* email = (char*)ASN1_STRING_get0_data(gen_name->d.rfc822Name);
                san_list.push_back("EMAIL:" + std::string(email));
            } else if (gen_name->type == GEN_IPADD) {
                // Handle IP addresses
                san_list.push_back("IP:...");
            }
        }
        GENERAL_NAMES_free(san_names);
    }
    
    return san_list;
}

// Load certificate from file (PEM or DER)
X509_ptr load_certificate_auto(const std::string& cert_path) {
    // Try PEM first
    BIO_ptr bio(BIO_new_file(cert_path.c_str(), "rb"), BIO_free_all);
    if (!bio) {
        handle_openssl_error("BIO_new_file");
        return X509_ptr(nullptr, X509_free);
    }
    
    X509* cert = PEM_read_bio_X509(bio.get(), nullptr, nullptr, nullptr);
    if (cert) {
        return X509_ptr(cert, X509_free);
    }
    
    // Reset BIO and try DER
    BIO_reset(bio.get());
    cert = d2i_X509_bio(bio.get(), nullptr);
    if (cert) {
        return X509_ptr(cert, X509_free);
    }
    
    handle_openssl_error("Failed to load certificate (tried both PEM and DER)");
    return X509_ptr(nullptr, X509_free);
}

// Verify certificate signature
bool verify_certificate_signature(X509* cert, X509* issuer_cert = nullptr) {
    if (!cert) return false;
    
    EVP_PKEY* pkey = nullptr;
    
    if (issuer_cert) {
        // Use issuer's public key for verification
        pkey = X509_get_pubkey(issuer_cert);
        if (!pkey) {
            handle_openssl_error("X509_get_pubkey (issuer)");
            return false;
        }
    } else {
        // Check if this is a self-signed certificate
        X509_NAME* subject = X509_get_subject_name(cert);
        X509_NAME* issuer = X509_get_issuer_name(cert);
        
        if (X509_NAME_cmp(subject, issuer) == 0) {
            // Self-signed: use certificate's own public key
            pkey = X509_get_pubkey(cert);
            if (!pkey) {
                handle_openssl_error("X509_get_pubkey (self-signed)");
                return false;
            }
            std::cout << "Note: Verifying self-signed certificate" << std::endl;
        } else {
            // Not self-signed and no issuer provided
            std::cerr << "Error: Certificate is not self-signed but no issuer certificate provided" << std::endl;
            std::cerr << "Subject: " << x509_name_to_string(subject) << std::endl;
            std::cerr << "Issuer:  " << x509_name_to_string(issuer) << std::endl;
            return false;
        }
    }
    
    EVP_PKEY_ptr pkey_ptr(pkey, EVP_PKEY_free);
    
    // Verify the certificate signature
    int result = X509_verify(cert, pkey);
    if (result == 1) {
        return true;  // Signature is valid
    } else if (result == 0) {
        std::cerr << "Certificate signature verification failed" << std::endl;
        return false;
    } else {
        handle_openssl_error("X509_verify");
        return false;
    }
}

// Parse all certificate fields
CertificateInfo parse_certificate(const std::string& cert_path) {
    CertificateInfo info = {};
    
    X509_ptr cert = load_certificate_auto(cert_path);
    if (!cert) {
        std::cerr << "Failed to load certificate from: " << cert_path << std::endl;
        return info;
    }
    
    // Version
    info.version = X509_get_version(cert.get()) + 1;  // Version is 0-indexed
    
    // Serial number
    ASN1_INTEGER* serial = X509_get_serialNumber(cert.get());
    info.serial_number = asn1_integer_to_hex(serial);
    
    // Subject and Issuer
    X509_NAME* subject = X509_get_subject_name(cert.get());
    X509_NAME* issuer = X509_get_issuer_name(cert.get());
    info.subject_name = x509_name_to_string(subject);
    info.issuer_name = x509_name_to_string(issuer);
    
    // Validity period
    const ASN1_TIME* not_before = X509_get0_notBefore(cert.get());
    const ASN1_TIME* not_after = X509_get0_notAfter(cert.get());
    info.valid_from = asn1_time_to_string(not_before);
    info.valid_to = asn1_time_to_string(not_after);
    
    // Signature algorithm
    const X509_ALGOR* sig_alg;
    X509_get0_signature(nullptr, &sig_alg, cert.get());
    if (sig_alg) {
        char buf[256];
        OBJ_obj2txt(buf, sizeof(buf), sig_alg->algorithm, 0);
        info.signature_algorithm = std::string(buf);
    }
    
    // Public key information
    EVP_PKEY* pkey = X509_get_pubkey(cert.get());
    if (pkey) {
        EVP_PKEY_ptr pkey_ptr(pkey, EVP_PKEY_free);
        info.public_key_algorithm = OBJ_nid2sn(EVP_PKEY_base_id(pkey));
        info.public_key_info = get_public_key_info(pkey);
    }
    
    // Extensions
    info.key_usage = parse_key_usage(cert.get());
    info.extended_key_usage = parse_extended_key_usage(cert.get());
    info.subject_alt_names = parse_subject_alt_names(cert.get());
    
    // Check if it's a CA certificate
    BASIC_CONSTRAINTS* bc = (BASIC_CONSTRAINTS*)X509_get_ext_d2i(cert.get(), NID_basic_constraints, nullptr, nullptr);
    if (bc) {
        info.is_ca = bc->ca ? true : false;
        BASIC_CONSTRAINTS_free(bc);
    }
    
    // Fingerprints
    info.fingerprint_sha1 = get_certificate_fingerprint(cert.get(), EVP_sha1());
    info.fingerprint_sha256 = get_certificate_fingerprint(cert.get(), EVP_sha256());
    
    // Verify signature
    info.signature_valid = verify_certificate_signature(cert.get(), issuer_cert.get());
    
    return info;
}

// Extract public key from certificate if signature is valid
EVP_PKEY* extract_public_key_if_valid(const std::string& cert_path, const std::string& issuer_cert_path = "") {
    X509_ptr cert = load_certificate_auto(cert_path);
    if (!cert) {
        return nullptr;
    }
    
    // Load issuer certificate if provided
    X509_ptr issuer_cert = nullptr;
    if (!issuer_cert_path.empty()) {
        issuer_cert = load_certificate_auto(issuer_cert_path);
    }
    
    // Verify certificate signature first
    if (!verify_certificate_signature(cert.get(), issuer_cert.get())) {
        std::cerr << "Certificate signature is invalid - returning null" << std::endl;
        return nullptr;
    }
    
    // Extract public key if signature is valid
    EVP_PKEY* pkey = X509_get_pubkey(cert.get());
    if (!pkey) {
        handle_openssl_error("X509_get_pubkey");
        return nullptr;
    }
    
    return pkey;  // Caller is responsible for freeing this
}

// Save public key to file
bool save_public_key(EVP_PKEY* pkey, const std::string& output_path) {
    if (!pkey) return false;
    
    BIO_ptr bio(BIO_new_file(output_path.c_str(), "wb"), BIO_free_all);
    if (!bio) {
        handle_openssl_error("BIO_new_file for public key output");
        return false;
    }
    
    if (PEM_write_bio_PUBKEY(bio.get(), pkey) != 1) {
        handle_openssl_error("PEM_write_bio_PUBKEY");
        return false;
    }
    
    return true;
}

// Print certificate information
void print_certificate_info(const CertificateInfo& info) {
    std::cout << "=== X.509 Certificate Information ===" << std::endl;
    std::cout << "Version: " << info.version << std::endl;
    std::cout << "Serial Number: " << info.serial_number << std::endl;
    std::cout << "Subject: " << info.subject_name << std::endl;
    std::cout << "Issuer: " << info.issuer_name << std::endl;
    std::cout << "Valid From: " << info.valid_from << std::endl;
    std::cout << "Valid To: " << info.valid_to << std::endl;
    std::cout << "Signature Algorithm: " << info.signature_algorithm << std::endl;
    std::cout << "Public Key Algorithm: " << info.public_key_algorithm << std::endl;
    std::cout << "Public Key Info: " << info.public_key_info << std::endl;
    std::cout << "Is CA: " << (info.is_ca ? "Yes" : "No") << std::endl;
    std::cout << "SHA1 Fingerprint: " << info.fingerprint_sha1 << std::endl;
    std::cout << "SHA256 Fingerprint: " << info.fingerprint_sha256 << std::endl;
    
    if (!info.key_usage.empty()) {
        std::cout << "Key Usage: ";
        for (size_t i = 0; i < info.key_usage.size(); i++) {
            std::cout << info.key_usage[i];
            if (i < info.key_usage.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
    
    if (!info.extended_key_usage.empty()) {
        std::cout << "Extended Key Usage: ";
        for (size_t i = 0; i < info.extended_key_usage.size(); i++) {
            std::cout << info.extended_key_usage[i];
            if (i < info.extended_key_usage.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
    
    if (!info.subject_alt_names.empty()) {
        std::cout << "Subject Alt Names: ";
        for (size_t i = 0; i < info.subject_alt_names.size(); i++) {
            std::cout << info.subject_alt_names[i];
            if (i < info.subject_alt_names.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "Signature Valid: " << (info.signature_valid ? "Yes" : "No") << std::endl;
    std::cout << "=================================" << std::endl;
}

void print_usage() {
    std::cout << "Usage:" << std::endl;
    std::cout << "  --parse-cert <certificate_file> [issuer_cert_file] [output_pubkey_file]" << std::endl;
    std::cout << "      Parse and display all certificate fields" << std::endl;
    std::cout << "      If signature is valid, optionally save public key to file" << std::endl;
    std::cout << "      Supports both PEM and DER formats" << std::endl;
    std::cout << "      issuer_cert_file: Required for non-self-signed certificates" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  # Self-signed certificate" << std::endl;
    std::cout << "  ./cert_parser --parse-cert self_signed.pem" << std::endl;
    std::cout << "  # Certificate signed by CA" << std::endl;
    std::cout << "  ./cert_parser --parse-cert server.pem ca.pem" << std::endl;
    std::cout << "  # With public key extraction" << std::endl;
    std::cout << "  ./cert_parser --parse-cert server.pem ca.pem extracted_pubkey.pem" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }
    
    std::string command = argv[1];
    
    if (command == "--parse-cert") {
        std::string cert_file = argv[2];
        std::string issuer_cert = "";
        std::string pubkey_output = "";
        
        // Parse arguments: cert_file [issuer_cert] [pubkey_output]
        if (argc >= 4) {
            // Check if arg 3 ends with .pem or .der (likely another cert)
            std::string arg3 = argv[3];
            if (arg3.find(".pem") != std::string::npos || 
                arg3.find(".der") != std::string::npos ||
                arg3.find(".crt") != std::string::npos) {
                issuer_cert = arg3;
                if (argc >= 5) {
                    pubkey_output = argv[4];
                }
            } else {
                pubkey_output = arg3;
            }
        }
        
        // Parse certificate and display all information
        CertificateInfo info = parse_certificate(cert_file, issuer_cert);
        print_certificate_info(info);
        
        // Extract public key if signature is valid
        EVP_PKEY* pkey = extract_public_key_if_valid(cert_file, issuer_cert);
        if (pkey) {
            std::cout << "\nSignature is VALID - Public key extracted successfully!" << std::endl;
            
            if (!pubkey_output.empty()) {
                if (save_public_key(pkey, pubkey_output)) {
                    std::cout << "Public key saved to: " << pubkey_output << std::endl;
                } else {
                    std::cerr << "Failed to save public key to file" << std::endl;
                }
            }
            
            EVP_PKEY_free(pkey);
            return 0;
        } else {
            std::cout << "\nSignature is INVALID - No public key extracted (returning null)" << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        print_usage();
        return 1;
    }
}