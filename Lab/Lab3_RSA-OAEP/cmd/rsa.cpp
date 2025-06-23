#include <iostream>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/queue.h>
#include <cryptopp/files.h>
#include <cryptopp/base64.h>
#include <cryptopp/filters.h>
#include <cryptopp/cryptlib.h>
#include <fstream>
#include <cstdlib> // For atoi

#include <cryptopp/integer.h>
#include <cryptopp/modarith.h>
#include <cryptopp/nbtheory.h>

using namespace CryptoPP;

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif
 
// C-style exports for DLL
extern "C" {
    EXPORT void RSA_GenKey(
        unsigned int keyLength, 
        const char *privateKeyBaseName, 
        const char *publicKeyBaseName, 
        bool outputDER, 
        bool outputPEM
    );
    EXPORT void RSA_Encrypt(
        const char *publicKeyFile, 
        const char *plaintextFile, 
        bool saveToFile, 
        const char *outputFile, 
        bool usePEM
    );
    EXPORT void RSA_Decrypt(
        const char *privateKeyFile, 
        const char *ciphertextFile, 
        bool saveToFile, 
        const char *outputFile, 
        bool usePEM
    );
}


void DERToPEM(const std::string& derFilename, const std::string& pemFilename, 
              const std::string& header, const std::string& footer) {
    // Read DER file
    std::ifstream derFile(derFilename, std::ios::binary);
    std::vector<char> derData((std::istreambuf_iterator<char>(derFile)),
                               std::istreambuf_iterator<char>());
    derFile.close();
    
    // Base64 encode
    std::string base64Data;
    StringSource ss(reinterpret_cast<const byte*>(derData.data()), derData.size(), true,
        new Base64Encoder(
            new StringSink(base64Data), true, 64
        )
    );
    
    // Write PEM file
    std::ofstream pemFile(pemFilename);
    pemFile << header << std::endl;
    pemFile << base64Data;
    pemFile << footer << std::endl;
    pemFile.close();
}

void SavePrivateKeyToDERFile(const RSA::PrivateKey& key, const std::string& filename) {
    ByteQueue queue;
    key.Save(queue);
    
    FileSink file(filename.c_str());
    queue.CopyTo(file);
    file.MessageEnd();
}

void SavePublicKeyToDERFile(const RSA::PublicKey& key, const std::string& filename) {
    ByteQueue queue;
    key.Save(queue);
    
    FileSink file(filename.c_str());
    queue.CopyTo(file);
    file.MessageEnd();
}

// Generate a large prime of the specified bit length
Integer GenerateLargePrime(AutoSeededRandomPool& rng, size_t bitLength) {
    Integer candidate;
    do {
        candidate.Randomize(rng, Integer::Power2(bitLength - 1), Integer::Power2(bitLength) - 1);
    } while (!IsPrime(candidate));
    return candidate;
}

RSA::PublicKey LoadPublicKeyFromPEM(const std::string& filename) {
    // Read the PEM file
    std::ifstream file(filename);
    std::string line, base64Data;
    bool inKey = false;
    
    while (std::getline(file, line)) {
        if (line == "-----BEGIN PUBLIC KEY-----") {
            inKey = true;
        } else if (line == "-----END PUBLIC KEY-----") {
            inKey = false;
        } else if (inKey) {
            base64Data += line;
        }
    }
    
    // Decode the Base64 data
    std::string derData;
    StringSource ss(base64Data, true,
        new Base64Decoder(
            new StringSink(derData)
        )
    );
    
    // Load the key
    RSA::PublicKey publicKey;
    ArraySource as(reinterpret_cast<const byte*>(derData.data()), derData.size(), true);
    publicKey.Load(as);
    
    return publicKey;
}

RSA::PrivateKey LoadPrivateKeyFromPEM(const std::string& filename) {
    // Read the PEM file
    std::ifstream file(filename);
    std::string line, base64Data;
    bool inKey = false;
    
    while (std::getline(file, line)) {
        if (line == "-----BEGIN PRIVATE KEY-----" || line == "-----BEGIN RSA PRIVATE KEY-----") {
            inKey = true;
        } else if (line == "-----END PRIVATE KEY-----" || line == "-----END RSA PRIVATE KEY-----") {
            inKey = false;
        } else if (inKey) {
            base64Data += line;
        }
    }
    
    // Decode the Base64 data
    std::string derData;
    StringSource ss(base64Data, true,
        new Base64Decoder(
            new StringSink(derData)
        )
    );
    
    // Load the key
    RSA::PrivateKey privateKey;
    ArraySource as(reinterpret_cast<const byte*>(derData.data()), derData.size(), true);
    privateKey.Load(as);
    
    return privateKey;
}

void RSA_GenKey(
    unsigned int keyLength,
    const char *privateKeyBaseName, 
    const char *publicKeyBaseName, 
    bool outputDER = true, bool 
    outputPEM = true)
{
    std::string privateKeyBase(privateKeyBaseName);
    std::string publicKeyBase(publicKeyBaseName);
    AutoSeededRandomPool rng;
    const size_t primeBits = keyLength / 2;

    // std::cout << "Generating large primes p and q..." << std::endl;
    Integer p = GenerateLargePrime(rng, primeBits + 10);
    Integer q = GenerateLargePrime(rng, primeBits - 10);

    // std::cout << "Calculating RSA parameters..." << std::endl;
    Integer n = p * q;
    Integer phi = (p - 1) * (q - 1);
    Integer e("4294967297"); // Common public exponent 2^32 +1
    // e.d =1 mod phi=(p-1)(q-1), lamda(p-1,q-1)
    ModularArithmetic ma(phi);
    Integer d = ma.MultiplicativeInverse(e);

    Integer dP = d % (p - 1);
    Integer dQ = d % (q - 1);
    ModularArithmetic mp(p);
    Integer qInv = mp.MultiplicativeInverse(q); //1/q mod p

    // std::cout << "Initializing RSA keys..." << std::endl;
    RSA::PrivateKey privateKey;
    privateKey.Initialize(n, e, d, p, q, dP, dQ, qInv);

    RSA::PublicKey publicKey;
    publicKey.Initialize(n, e);
    
    // Verify the keys by NIST rules
    bool result = privateKey.Validate(rng, 3);
    if (!result) {
        std::cerr << "Private key validation failed" << std::endl;
        return;
    }
    
    result = publicKey.Validate(rng, 3);
    if (!result) {
        std::cerr << "Public key validation failed" << std::endl;
        return;
    }
    
    // std::cout << "RSA keys generated and validated successfully." << std::endl;
    SavePrivateKeyToDERFile(privateKey, privateKeyBase + ".der");
    SavePublicKeyToDERFile(publicKey, publicKeyBase + ".der");

    if (outputPEM)
    {
        // Convert private key from DER to PEM
        DERToPEM(privateKeyBase + ".der", privateKeyBase + ".pem", 
            "-----BEGIN RSA PRIVATE KEY-----", 
            "-----END RSA PRIVATE KEY-----");

        // Convert public key from DER to PEM
        DERToPEM(publicKeyBase + ".der", publicKeyBase + ".pem", 
            "-----BEGIN PUBLIC KEY-----", 
            "-----END PUBLIC KEY-----");
    }
    
    if (!outputDER)
    {
        std::remove((privateKeyBase + ".der").c_str());
        std::remove((publicKeyBase + ".der").c_str());
    }
}

void RSA_Encrypt(const char *publicKeyFile, const char *plaintextFile, bool saveToFile, const char *outputFile, bool usePEM)
{
    try {
        RSA::PublicKey publicKey;
        // Load the public key for encryption
        if (usePEM)
        {
            publicKey = LoadPublicKeyFromPEM(publicKeyFile);
        }
        else
        {
            FileSource fs(publicKeyFile, true);
            publicKey.Load(fs);
        }
        
        // Validate the key
        AutoSeededRandomPool rng;
        bool result = publicKey.Validate(rng, 3);
        if (!result) {
            std::cerr << "Public key validation failed" << std::endl;
            return;
        }
        
        // Message to encrypt
        std::string plaintext = "RSA encryption test with Crypto++";
        std::cout << "Plaintext: " << plaintext << std::endl;
        
        // Encrypt using PKCS#1 v1.5 padding
        RSAES_OAEP_SHA256_Encryptor encryptor(publicKey);
        
        // Check if the message fits within the maximum size
        size_t maxPlaintextLength = encryptor.FixedMaxPlaintextLength();
        if (plaintext.length() > maxPlaintextLength) {
            std::cerr << "Message too long for RSA encryption. Maximum length: " 
                      << maxPlaintextLength << " bytes." << std::endl;
            return;
        }
        
        // Perform encryption
        std::string ciphertext;
        StringSource ss(plaintext, true,
            new PK_EncryptorFilter(rng, encryptor,
                new StringSink(ciphertext)
            )
        );
        
        // Output the encrypted data in Base64 format for readability
        std::string encoded;
        StringSource ss2(ciphertext, true,
            new Base64Encoder(
                new StringSink(encoded)
            )
        );

        if (saveToFile)
        {
            StringSource(ciphertext, true,
                new FileSink(outputFile)
            );
            std::cout << "Ciphertext saved\n";
        }
        
        std::cout << "Ciphertext (Base64): " << encoded << std::endl;
        
    } catch (const Exception& e) {
        std::cerr << "Crypto++ exception: " << e.what() << std::endl;
        return;
    } catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
        return;
    }
}

void RSA_Decrypt(const char *privateKeyFile, const char *ciphertextFile, bool saveToFile, const char *outputFile, bool usePEM)
{
    try {
        RSA::PrivateKey privateKey;        
        // Load the public key for encryption
        if (usePEM)
        {
            privateKey = LoadPrivateKeyFromPEM(privateKeyFile);
        }
        else
        {
            FileSource fs(privateKeyFile, true);
            privateKey.Load(fs);
        }        
        
        // Validate the key
        AutoSeededRandomPool rng;
        bool result = privateKey.Validate(rng, 3);
        if (!result) {
            std::cerr << "Private key validation failed" << std::endl;
            return;
        }
        
        // Load the encrypted data
        std::string ciphertext;
        FileSource fs2(ciphertextFile, true, new StringSink(ciphertext));
        
        // Decrypt using PKCS#1 v1.5 padding
        RSAES_OAEP_SHA256_Decryptor decryptor(privateKey);
        
        // Perform decryption
        std::string recovered;
        StringSource ss(ciphertext, true,
            new PK_DecryptorFilter(rng, decryptor,
                new StringSink(recovered)
            )
        );
        
        std::cout << "Recovered plaintext: " << recovered << std::endl;

        if (saveToFile)
        {
            StringSource(recovered, true,
                new FileSink(outputFile)
            );
            std::cout << "Recovered plaintext saved\n";
        }
        
    } catch (const Exception& e) {
        std::cerr << "Crypto++ exception: " << e.what() << std::endl;
        return;
    } catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
        return;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " genkey <key_length> <private_key_basename> <public_key_basename> [--no-der] [--no-pem]\n"
                  << "       " << argv[0] << " encrypt <public_key> <plaintext_file> [--use-pem]\n"
                  << "       " << argv[0] << " decrypt <private_key> <ciphertext_file> [--use-pem]\n";
        return 1;
    }

    // Optional flags
    bool outputDER = true;
    bool outputPEM = true;
    bool usePEM = false;

    for (int i = 4; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--no-der") {
            outputDER = false;
        } else if (arg == "--no-pem") {
            outputPEM = false;
        } else if (arg == "--use-pem") {
            usePEM = true;
        }
    }
 
    std::string mode = argv[1];
    if (mode == "genkey")
    {
        // Required arguments
        unsigned int keyLength = std::atoi(argv[2]);
        //std::string privateBase = argv[2];
        //std::string publicBase = argv[3];
    
        RSA_GenKey(keyLength, argv[3], argv[4], outputDER, outputPEM);
    }
    else if (mode == "encrypt")
    {
        char *keyFile = argv[2], *plaintextFile = argv[3];
        RSA_Encrypt(keyFile, plaintextFile, true, "ciphertext", usePEM);
    }
    else if (mode == "decrypt")
    {
        char *keyFile = argv[2], *ciphertextFile = argv[3];
        RSA_Decrypt(keyFile, ciphertextFile, true, "recovered", usePEM);
    }
    else 
    {
        std::cout << "Invalid mode";
        return 1;
    }
    return 0;
}