#include "aes.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>

const unsigned char AES_CBC::sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const unsigned char AES_CBC::inv_sbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

const unsigned char AES_CBC::Rcon[11] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

AES_CBC::AES_CBC(const unsigned char *key) {
    memcpy(this->key, key, 16);
    // std::srand(std::time(nullptr)); // seed only once in main()
    // for (int i = 0; i < 16; i++) {
    //     this->iv[i] = std::rand() % 256;
    // }
    unsigned char iv[16] = {
        0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38 
    };
    memcpy(this->iv, iv, 16);
    KeyExpansion();
}

void printState(const unsigned char state[4][4]) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            std::cout << static_cast<int>(state[row][col]) << " ";
        }
        std::cout << "\n";
    }
}

void AES_CBC::PrintRoundKeys() {
    std::cout << "\n=== Key Schedule ===" << std::endl;
    for (int round = 0; round < 11; round++) {
        std::cout << "Round " << std::setw(2) << round << ": ";
        for (int i = 0; i < 16; i++) {
            std::cout << std::setw(2) << std::setfill('0') 
                     << (int)roundKeys[round][i] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "===================\n" << std::endl;
}

void AES_CBC::KeyExpansion() {
    unsigned char temp[4];
    
    // Copy the original key to the first round key
    memcpy(roundKeys[0], key, 16);
    
    for (int i = 1; i < 11; i++) {
        // Rotate the previous round key's last word
        temp[0] = roundKeys[i-1][12];
        temp[1] = roundKeys[i-1][13];
        temp[2] = roundKeys[i-1][14];
        temp[3] = roundKeys[i-1][15];
        
        // Perform key schedule core
        unsigned char t = temp[0];
        temp[0] = sbox[temp[1]] ^ Rcon[i];
        temp[1] = sbox[temp[2]];
        temp[2] = sbox[temp[3]];
        temp[3] = sbox[t];
        
        // XOR with the word from 4 positions back
        for (int j = 0; j < 4; j++) {
            roundKeys[i][j] = roundKeys[i-1][j] ^ temp[j];
        }
        
        // Fill the rest of the round key
        for (int j = 4; j < 16; j++) {
            roundKeys[i][j] = roundKeys[i-1][j] ^ roundKeys[i][j-4];
        }
    }
    // PrintRoundKeys();
}

void AES_CBC::AddRoundKey(unsigned char state[4][4], int round) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] ^= roundKeys[round][i * 4 + j];
        }
    }
}

void AES_CBC::SubBytes(unsigned char state[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = sbox[state[i][j]];
        }
    }
}

void AES_CBC::InvSubBytes(unsigned char state[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = inv_sbox[state[i][j]];
        }
    }
}

void AES_CBC::ShiftRows(unsigned char state[4][4]) {
    // Second row - rotate left by 1
    unsigned char temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;
    
    // Third row - rotate left by 2
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;
    
    // Fourth row - rotate left by 3 (equivalent to right by 1)
    temp = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = temp;
}

void AES_CBC::InvShiftRows(unsigned char state[4][4]) {
    // Second row - rotate right by 1
    unsigned char temp = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = temp;
    
    // Third row - rotate right by 2
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;
    
    // Fourth row - rotate right by 3 (equivalent to left by 1)
    temp = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = temp;
}

unsigned char AES_CBC::gfMultiply(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    unsigned char hi_bit;
    
    for (int i = 0; i < 8; i++) {
        if (b & 1) p ^= a;
        hi_bit = a & 0x80;
        a <<= 1;
        if (hi_bit) a ^= 0x1b; // x^8 + x^4 + x^3 + x + 1
        b >>= 1;
    }
    
    return p;
}

void AES_CBC::MixColumns(unsigned char state[4][4]) {
    unsigned char temp[4];
    
    for (int i = 0; i < 4; i++) {
        temp[0] = state[0][i];
        temp[1] = state[1][i];
        temp[2] = state[2][i];
        temp[3] = state[3][i];
        
        state[0][i] = gfMultiply(0x02, temp[0]) ^ gfMultiply(0x03, temp[1]) ^ temp[2] ^ temp[3];
        state[1][i] = temp[0] ^ gfMultiply(0x02, temp[1]) ^ gfMultiply(0x03, temp[2]) ^ temp[3];
        state[2][i] = temp[0] ^ temp[1] ^ gfMultiply(0x02, temp[2]) ^ gfMultiply(0x03, temp[3]);
        state[3][i] = gfMultiply(0x03, temp[0]) ^ temp[1] ^ temp[2] ^ gfMultiply(0x02, temp[3]);
    }
}

void AES_CBC::InvMixColumns(unsigned char state[4][4]) {
    unsigned char temp[4];
    
    for (int i = 0; i < 4; i++) {
        temp[0] = state[0][i];
        temp[1] = state[1][i];
        temp[2] = state[2][i];
        temp[3] = state[3][i];
        
        state[0][i] = gfMultiply(0x0e, temp[0]) ^ gfMultiply(0x0b, temp[1]) ^ gfMultiply(0x0d, temp[2]) ^ gfMultiply(0x09, temp[3]);
        state[1][i] = gfMultiply(0x09, temp[0]) ^ gfMultiply(0x0e, temp[1]) ^ gfMultiply(0x0b, temp[2]) ^ gfMultiply(0x0d, temp[3]);
        state[2][i] = gfMultiply(0x0d, temp[0]) ^ gfMultiply(0x09, temp[1]) ^ gfMultiply(0x0e, temp[2]) ^ gfMultiply(0x0b, temp[3]);
        state[3][i] = gfMultiply(0x0b, temp[0]) ^ gfMultiply(0x0d, temp[1]) ^ gfMultiply(0x09, temp[2]) ^ gfMultiply(0x0e, temp[3]);
    }
}

void AES_CBC::xorBlocks(const unsigned char a[16], const unsigned char b[16], unsigned char result[16]) {
    for (int i = 0; i < 16; i++) {
        result[i] = a[i] ^ b[i];
    }
}

void AES_CBC::padPKCS7(unsigned char *data, int &length) {
    int pad_len = 16 - (length % 16);
    for (int i = length; i < length + pad_len; i++) {
        data[i] = pad_len;
    }
    length += pad_len;
}

void AES_CBC::unpadPKCS7(unsigned char *data, int &length) {
    int pad_len = data[length - 1];
    length -= pad_len;
}

void AES_CBC::encrypt(unsigned char *data, int &length) {
    // Pad the data
    padPKCS7(data, length);
    
    unsigned char previousBlock[16];
    memcpy(previousBlock, iv, 16);
    
    for (int i = 0; i < length; i += 16) {
        // XOR with previous ciphertext block (or IV for first block)
        xorBlocks(&data[i], previousBlock, &data[i]);
        
        // Convert to state matrix (column-major order)
        unsigned char state[4][4];
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                state[row][col] = data[i + row + 4 * col];
            }
        }
        // std::cout << "State before AddRoundKey (round 0):" << std::endl;
        // printState(state);
        // AES encryption rounds
        AddRoundKey(state, 0);
        // std::cout << "State after AddRoundKey (round 0):" << std::endl;
        // printState(state);
        
        for (int round = 1; round < 10; round++) {
            SubBytes(state);
            // std::cout << "State after SubBytes (round " << round << "):" << std::endl;
            // printState(state);

            ShiftRows(state);
            std::cout << "State after ShiftRows (round " << round << "):" << std::endl;
            printState(state);

            MixColumns(state);
            std::cout << "State after MixColumns (round " << round << "):" << std::endl;
            printState(state);

            AddRoundKey(state, round);
            // std::cout << "State after AddRoundKey (round " << round << "):" << std::endl;
            // printState(state);
        }
        
        SubBytes(state);
        // std::cout << "State after SubBytes (round 10):" << std::endl;
        // printState(state);

        ShiftRows(state);
        // std::cout << "State after ShiftRows (round 10):" << std::endl;
        // printState(state);

        AddRoundKey(state, 10);
        // std::cout << "State after AddRoundKey (round 10):" << std::endl;
        // printState(state);
        
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                data[i + row + 4 * col] = state[row][col];
            }
        }
        memcpy(previousBlock, &data[i], 16);
    }
    // Add iv to the beginning of the data
    memmove(data + 16, data, length);
    memcpy(data, iv, 16);
    length += 16;
}

void AES_CBC::decrypt(unsigned char *data, int &length) {
    // Extract iv
    memcpy(iv, data, 16);

    // Shift ciphertext back to overwrite the IV
    memmove(data, data + 16, length - 16);
    length -= 16;

    unsigned char previousBlock[16];
    memcpy(previousBlock, iv, 16);
    
    for (int i = 0; i < length; i += 16) {
        // Save current cipher block for next block
        unsigned char currentCipherBlock[16];
        memcpy(currentCipherBlock, &data[i], 16);

        unsigned char state[4][4];
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                state[row][col] = data[i + row + 4 * col];
            }
        }
        
        // AES decryption rounds
        AddRoundKey(state, 10);
        
        for (int round = 9; round > 0; round--) {
            InvShiftRows(state);
            InvSubBytes(state);
            AddRoundKey(state, round);
            InvMixColumns(state);
        }
        
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, 0);
        
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                data[i + row + 4 * col] = state[row][col];
            }
        }
        
        // xorBlocks(&data[i], previousBlock, &data[i]);
        // memcpy(previousBlock, &data[i], 16);

        xorBlocks(&data[i], previousBlock, &data[i]);
        memcpy(previousBlock, currentCipherBlock, 16);
    }
    unpadPKCS7(data, length);
}

void AES_CBC::printHex(const unsigned char *data, int length) {
    for (int i = 0; i < length; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
    }
    std::cout << std::dec << std::endl;
}