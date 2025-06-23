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

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <key_length> <private_key_basename> <public_key_basename> [--no-der] [--no-pem]\n";
        return 1;
    }
 
    // Required arguments
    unsigned int keyLength = std::atoi(argv[1]);
    //std::string privateBase = argv[2];
    //std::string publicBase = argv[3];
 
    // Optional flags
    bool outputDER = true;
    bool outputPEM = true;
 
    for (int i = 4; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--no-der") {
            outputDER = false;
        } else if (arg == "--no-pem") {
            outputPEM = false;
        } else {
            std::cerr << "Unknown option: " << arg << std::endl;
            return 1;
        }
    }
 
    RSA_GenKey(keyLength, argv[2], argv[3], outputDER, outputPEM);
    return 0;
}