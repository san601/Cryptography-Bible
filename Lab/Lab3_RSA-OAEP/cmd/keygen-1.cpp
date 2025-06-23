#include <iostream>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/queue.h>
#include <cryptopp/files.h>
#include <cryptopp/base64.h>
#include <cryptopp/filters.h>
#include <fstream>

using namespace CryptoPP;

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

int main() {
    // Create a random number generator
    AutoSeededRandomPool rng;
    
    // Generate RSA keys
    RSA::PrivateKey privateKey;
    privateKey.GenerateRandomWithKeySize(rng, 3072);
    
    // Extract the public key from the private key
    RSA::PublicKey publicKey;
    publicKey.AssignFrom(privateKey);
    
    // Verify the keys by NIST rules
    bool result = privateKey.Validate(rng, 3);
    if (!result) {
        std::cerr << "Private key validation failed" << std::endl;
        return 1;
    }
    
    result = publicKey.Validate(rng, 3);
    if (!result) {
        std::cerr << "Public key validation failed" << std::endl;
        return 1;
    }
    
    std::cout << "RSA keys generated and validated successfully." << std::endl;
    SavePrivateKeyToDERFile(privateKey, "private_key.der");
    SavePublicKeyToDERFile(publicKey, "public_key.der");
    // Convert private key from DER to PEM
    DERToPEM("private_key.der", "private_key.pem", 
         "-----BEGIN RSA PRIVATE KEY-----", 
         "-----END RSA PRIVATE KEY-----");

    // Convert public key from DER to PEM
    DERToPEM("public_key.der", "public_key.pem", 
         "-----BEGIN PUBLIC KEY-----", 
         "-----END PUBLIC KEY-----");
    return 0;
}