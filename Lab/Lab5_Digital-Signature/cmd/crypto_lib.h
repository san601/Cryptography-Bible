// crypto_lib.h
#ifndef CRYPTO_LIB_H
#define CRYPTO_LIB_H

#include <string>
#include <vector>

// --- Error Handling ---
void handle_openssl_error(const char* context);

// --- Helper Functions ---

/**
 * @brief Reads the entire content of a file into a byte vector.
 * @param file_path Path to the file.
 * @param data Output vector containing file bytes.
 * @return true on success, false on failure.
 */
bool read_file_bytes(const std::string& file_path, std::vector<unsigned char>& data);

/**
 * @brief Writes a byte vector to a file.
 * @param file_path Path to the file.
 * @param data Vector containing bytes to write.
 * @return true on success, false on failure.
 */
bool write_file_bytes(const std::string& file_path, const std::vector<unsigned char>& data);

/**
 * @brief Encodes a byte vector into a Base64 string.
 * @param input Byte vector to encode.
 * @return Base64 encoded string, or empty string on failure.
 */
std::string base64_encode(const std::vector<unsigned char>& input);

// --- Key Generation ---

/**
 * @brief Generates an ECDSA key pair using a specified curve.
 * @param curve_name The name of the elliptic curve (e.g., "prime256v1").
 * @param private_key_path Path to save the private key (PEM format).
 * @param public_key_path Path to save the public key (PEM format).
 * @return true on success, false on failure.
 */
bool generate_ecdsa_keypair(const std::string& curve_name, const std::string& private_key_path, const std::string& public_key_path);

/**
 * @brief Generates an RSA key pair with a specified bit length.
 * @param bits The number of bits for the RSA key (e.g., 2048, 3072, 4096).
 * @param private_key_path Path to save the private key (PEM format).
 * @param public_key_path Path to save the public key (PEM format).
 * @return true on success, false on failure.
 */
bool generate_rsa_keypair(int bits, const std::string& private_key_path, const std::string& public_key_path);

/**
 * @brief Generates a Certificate Signing Request (CSR) using a private key.
 * @param private_key_path Path to the private key (PEM format, e.g., RSA or ECDSA).
 * @param csr_path Path to save the CSR (PEM format).
 * @param subject_info Vector of subject components (e.g., {"C=US", "ST=CA", "O=MyOrg", "CN=localhost"}).
 * @return true on success, false on failure.
 */
bool generate_csr(const std::string& private_key_path, const std::string& csr_path, const std::vector<std::string>& subject_info);

/**
 * @brief Generates a self-signed X.509 certificate from a CSR and private key.
 * @param csr_path Path to the CSR file (PEM format).
 * @param private_key_path Path to the private key used for the CSR (PEM format).
 * @param certificate_path Path to save the certificate (PEM format).
 * @param days Validity period in days.
 * @return true on success, false on failure.
 */
bool generate_self_signed_certificate(const std::string& csr_path, const std::string& private_key_path, const std::string& certificate_path, int days);

// --- Signing ---

/**
 * @brief Signs a message file using an ECDSA private key (SHA256 digest).
 * @param private_key_path Path to the ECDSA private key (PEM format).
 * @param message_path Path to the message file to sign.
 * @param signature_path Path to save the binary signature.
 * @return true on success, false on failure.
 */
bool sign_ecdsa(const std::string& private_key_path, const std::string& message_path, const std::string& signature_path);

/**
 * @brief Signs a message file using an RSA private key with RSASSA-PSS padding (SHA256 digest).
 * @param private_key_path Path to the RSA private key (PEM format).
 * @param message_path Path to the message file to sign.
 * @param signature_path Path to save the binary signature.
 * @return true on success, false on failure.
 */
bool sign_rsapss(const std::string& private_key_path, const std::string& message_path, const std::string& signature_path);

// --- Verification ---

/**
 * @brief Verifies an ECDSA signature (SHA256 digest).
 * @param public_key_path Path to the ECDSA public key (PEM format).
 * @param message_path Path to the original message file.
 * @param signature_path Path to the binary signature file.
 * @return true if signature is valid, false otherwise (or on error).
 */
bool verify_ecdsa(const std::string& public_key_path, const std::string& message_path, const std::string& signature_path);

/**
 * @brief Verifies an RSASSA-PSS signature (SHA256 digest).
 * @param public_key_path Path to the RSA public key (PEM format).
 * @param message_path Path to the original message file.
 * @param signature_path Path to the binary signature file.
 * @return true if signature is valid, false otherwise (or on error).
 */
bool verify_rsapss(const std::string& public_key_path, const std::string& message_path, const std::string& signature_path);


// --- X.509 Operations ---

/**
 * @brief Extracts the public key from an X.509 certificate file.
 * @param certificate_path Path to the certificate file (PEM format).
 * @param public_key_path Path to save the extracted public key (PEM format).
 * @return true on success, false on failure.
 */
bool extract_pubkey_from_cert(const std::string& certificate_path, const std::string& public_key_path);

/**
 * @brief Verifies a signature using the public key from an X.509 certificate.
 *        Determines the algorithm (ECDSA/RSA-PSS) based on the key type in the certificate.
 * @param certificate_path Path to the certificate file (PEM format).
 * @param message_path Path to the original message file.
 * @param signature_path Path to the binary signature file.
 * @return true if signature is valid, false otherwise (or on error).
 */
bool verify_signature_with_cert(const std::string& certificate_path, const std::string& message_path, const std::string& signature_path);


#endif // CRYPTO_LIB_H

