#include <iostream>
#include <string>
#include "aes.h"

int main() {
    // Example key and IV
    unsigned char key[16] = {
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 
        0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68
    };
    
    AES_CBC aes(key);
    
    // Example plaintext
    std::string plaintext;
    std:: cout << "Enter plaintext: ";
    getline(std::cin, plaintext);
    int length = strlen(plaintext.c_str());
    
    // Allocate space for ciphertext (will be padded)
    unsigned char *data = new unsigned char[length + 32];
    memcpy(data, plaintext.c_str(), length);
    
    std::cout << "Original message: " << plaintext << std::endl;
    std::cout << "Original length: " << length << std::endl;
    
    // Encrypt
    aes.encrypt(data, length);
    std::cout << "Encrypted (hex): ";
    aes.printHex(data, length);
    
    // Decrypt
    aes.decrypt(data, length);
    std::cout << "Decrypted message: ";
    std::cout.write((char*)data, length);
    std::cout << "\nDecrypted length: " << length << std::endl;
    
    delete[] data;
    return 0;
}