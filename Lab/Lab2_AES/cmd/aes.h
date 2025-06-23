#ifndef AES_H
#define AES_H

#include <cstdint>
#include <string.h>

#define AES_BLOCK_SIZE 16
#define AES_ROUNDS 10
#define AES_KEY_SIZE 16

class AES_CBC {
private:
    // AES S-box
    static const unsigned char sbox[256];
    static const unsigned char inv_sbox[256];
    static const unsigned char Rcon[11];

    unsigned char key[16];
    unsigned char iv[16];
    unsigned char roundKeys[11][16];

    // Helper functions
    void KeyExpansion();
    void AddRoundKey(unsigned char state[4][4], int round);
    void SubBytes(unsigned char state[4][4]);
    void InvSubBytes(unsigned char state[4][4]);
    void ShiftRows(unsigned char state[4][4]);
    void InvShiftRows(unsigned char state[4][4]);
    void MixColumns(unsigned char state[4][4]);
    void InvMixColumns(unsigned char state[4][4]);
    unsigned char gfMultiply(unsigned char a, unsigned char b);
    void xorBlocks(const unsigned char a[16], const unsigned char b[16], unsigned char result[16]);
    void padPKCS7(unsigned char *data, int &length);
    void unpadPKCS7(unsigned char *data, int &length);
    void PrintRoundKeys();

public:
    AES_CBC(const unsigned char *key);
    void generateRandomIV(unsigned char* iv);
    void encrypt(unsigned char *data, int &length);
    void decrypt(unsigned char *data, int &length);
    void printHex(const unsigned char *data, int length);
};

#endif //AES_H
