#include <iostream>
#include <string>

// Crypto++ Headers
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/secblock.h>
#include <cryptopp/xts.h>
#include <cryptopp/ccm.h>
#include <cryptopp/gcm.h>
#include <cryptopp/des.h>

using namespace CryptoPP;

const int TAG_SIZE = 12;

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" {
    EXPORT void __cdecl GenerateAESKey(byte *key, byte *iv);
    EXPORT void __cdecl SaveKeyToFile(const char *filename, const byte *key, const byte *iv);
    EXPORT void __cdecl LoadKeyFromFile(const char *filename, byte *key, byte *iv);
    EXPORT void __cdecl AESEncrypt(const char *plaintext, const char *output, const byte *key, const byte *iv);
    EXPORT void __cdecl AESDecrypt(const char *ciphertext, const char *output, const byte *key, const byte *iv);
}

#include <iostream>

// Helper function to print byte arrays in hex format
void PrintHexFromBytes(const std::string& label, byte* data, size_t length) {
    std::string encoded;
    StringSource ss(data, length, true,
        new HexEncoder(
            new StringSink(encoded)
        )
    );
    std::cout << label << ": " << encoded << std::endl;
}

void PrintHexFromFile(const std::string& label, const char *file) {
    std::string encoded;
    FileSource(file, true, 
        new HexEncoder(
            new StringSink(encoded)
        )
    );
    std::cout << label << encoded << std::endl;
}

// Generate random key and IV
void GenerateKey(const char *algo, const int mode, byte *key, byte *iv) {
    int default_keylength, blocksize;
    if (!strcmp(algo, "AES")) 
    {
        default_keylength = AES::DEFAULT_KEYLENGTH;
        blocksize = AES::BLOCKSIZE;
    }
    else
    {
        default_keylength = DES_EDE3::DEFAULT_KEYLENGTH;
        blocksize = DES_EDE3::BLOCKSIZE;
    }
    AutoSeededRandomPool *prng = new AutoSeededRandomPool();
    if (mode != 6)
    {
        if (mode != 7)
        {
            prng->GenerateBlock(key, default_keylength);
            prng->GenerateBlock(iv, blocksize);
        }

        prng->GenerateBlock(key, default_keylength);
        prng->GenerateBlock(iv, 12);
    }
    else
    {
        prng->GenerateBlock(key, default_keylength * 2);
        prng->GenerateBlock(iv, blocksize);
    }
}

void SaveKeyToFile(const char *algo, const int mode, const char *filename, const byte *key, const byte *iv) {
    int default_keylength, blocksize;
    if (!strcmp(algo, "AES")) 
    {
        default_keylength = AES::DEFAULT_KEYLENGTH;
        blocksize = AES::BLOCKSIZE;
    }
    else
    {
        default_keylength = DES_EDE3::DEFAULT_KEYLENGTH;
        blocksize = DES_EDE3::BLOCKSIZE;
    }
    FileSink file(filename, true);
    if (mode != 6)
    {
        if (mode != 7)
        {
            file.Put(key, default_keylength); 
            file.Put(iv, blocksize);  
        }

        file.Put(key, default_keylength); 
        file.Put(iv, 12);  
    }
    else
    {
        file.Put(key, default_keylength * 2); 
        file.Put(iv, blocksize);  
    }        
    file.MessageEnd();
    std::cout << "Key and IV saved to: " << filename << std::endl;
}

// Load key and IV from binary file using FileSource and ArraySink
void LoadKeyFromFile(const char *algo, const int mode, const char *filename, byte *key, byte *iv) {
    int default_keylength, blocksize;
    if (!strcmp(algo, "AES")) 
        {
            default_keylength = AES::DEFAULT_KEYLENGTH;
            blocksize = AES::BLOCKSIZE;
        }
        else
        {
            default_keylength = DES_EDE3::DEFAULT_KEYLENGTH;
            blocksize = DES_EDE3::BLOCKSIZE;
        }
    try {
        FileSource file(filename, false);  // Don't PumpAll yet

        if (mode != 6)
        {
            // Read key
            file.Attach(new ArraySink(key, default_keylength));
            file.Pump(default_keylength);
        }
        else
        {
            // Read key
            file.Attach(new ArraySink(key, default_keylength * 2));
            file.Pump(default_keylength * 2);
        }
        // Reposition and read IV
        if (mode != 7)
        {
            file.Attach(new ArraySink(iv, blocksize));
            file.Pump(blocksize);
        }
        else 
        {
            file.Attach(new ArraySink(iv, 12));
            file.Pump(12);
        }

        std::cout << "Key and IV loaded from file: " << filename << std::endl;
    } catch (const CryptoPP::Exception& e) {
        std::cerr << "Error loading key and IV: " << e.what() << std::endl;
    }
}

void AESEncrypt(const int mode, const char *plaintext, const char *output, const byte *key, const byte *iv) {
    std::cout << "Plaintext: " << plaintext << "\n";
    try {
        switch (mode)
        {
            case 1: // ECB
            {
                ECB_Mode<AES>::Encryption encryptor;
                encryptor.SetKey(key, AES::DEFAULT_KEYLENGTH);
                StringSource ss1( plaintext, true, 
                    new StreamTransformationFilter( encryptor,
                        new FileSink( output ),
                        BlockPaddingSchemeDef::PKCS_PADDING
                    ) // StreamTransformationFilter      
                ); // StringSource
                break;
            }
            case 2: // CBC
            {
                CBC_Mode<AES>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
                StringSource ss( plaintext, true, 
                    new StreamTransformationFilter( encryptor,
                        new FileSink( output ),
                        BlockPaddingSchemeDef::PKCS_PADDING
                    ) // StreamTransformationFilter      
                ); // StringSource
                break;
            }
            case 3: // OFB
            {
                OFB_Mode<AES>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
                StringSource(plaintext, true,
                    new StreamTransformationFilter(encryptor,
                        new FileSink( output )
                    ) //StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 4: // CFB
            {
                OFB_Mode<AES>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
                StringSource( plaintext, true, 
                    new StreamTransformationFilter(encryptor,
                        new FileSink( output )
                    ) // StreamTransformationFilter      
                ); // StringSource
                break;
            }
            case 5: // CTR
            {
                CTR_Mode<AES>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
                StringSource ss1( plaintext, true, 
                    new StreamTransformationFilter( encryptor,
                        new FileSink( output )
                    ) // StreamTransformationFilter      
                ); // StringSource
                break;
            }
            case 6: // XTS
            {
                XTS_Mode<AES>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH * 2, iv);
                StringSource ss( plaintext, true, 
                    new StreamTransformationFilter( encryptor,
                        new FileSink( output ),
                        StreamTransformationFilter::NO_PADDING
                    ) // StreamTransformationFilter      
                ); // StringSource
                break;
            }
            case 7: // CCM
            {
                CCM<AES, TAG_SIZE>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv, 12);
                size_t len = std::strlen(plaintext);
                encryptor.SpecifyDataLengths( 0, len, 0 );

                StringSource ss1( plaintext, true,
                    new AuthenticatedEncryptionFilter( encryptor,
                        new FileSink( output )
                    ) // AuthenticatedEncryptionFilter
                ); // StringSource
                break;
            }
            case 8: // GCM
            {
                GCM<AES>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);

                StringSource ss1( plaintext, true,
                    new AuthenticatedEncryptionFilter( encryptor,
                        new FileSink( output ), false, TAG_SIZE
                    ) // AuthenticatedEncryptionFilter
                ); // StringSource
                break;
            }
            default:
                return;
        }
    } catch (const Exception& e) {
        std::cerr << "Encryption Error: " << e.what() << std::endl;
    }
    PrintHexFromFile("Ciphertext: ", output);
}

void AESDecrypt(const int mode, const char *ciphertext, const char *output, const byte *key, const byte *iv, int ciphertextSize = 0) {
    try {
        switch (mode)
        {
            case 1: // ECB
            {
                ECB_Mode<AES>::Decryption decryptor;
                decryptor.SetKey(key, AES::DEFAULT_KEYLENGTH);
                StringSource ss3( ciphertext, true, 
                    new StreamTransformationFilter( decryptor,
                        new FileSink( output ),
                        BlockPaddingSchemeDef::PKCS_PADDING
                    ) // StreamTransformationFilter
                ); // StringSource

                break;
            }
            case 2: // CBC
            {
                CBC_Mode<AES>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
                StringSource ss( ciphertext, true, 
                    new StreamTransformationFilter( decryptor,
                        new FileSink( output ),
                        BlockPaddingSchemeDef::PKCS_PADDING
                    ) // StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 3: // OFB
            {
                OFB_Mode<AES>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
                CryptoPP::StringSource(ciphertext, true,
                    new CryptoPP::StreamTransformationFilter(decryptor,
                        new CryptoPP::FileSink(output)
                    ) //StreamTransformationFilter
                ); //StringSource
                break;
            }
            case 4: // CFB
            {
                OFB_Mode<AES>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
                StringSource ss3( ciphertext, true, 
                    new StreamTransformationFilter( decryptor,
                        new FileSink( output )
                    ) // StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 5: // CTR
            {
                CTR_Mode<AES>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
                StringSource ss3( ciphertext, true, 
                    new StreamTransformationFilter( decryptor,
                        new FileSink( output )
                    ) // StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 6: // XTS
            {
                XTS_Mode<AES>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH * 2, iv, AES::BLOCKSIZE);
                StringSource ss( ciphertext, true, 
                    new StreamTransformationFilter( decryptor,
                        new FileSink( output ),
                        StreamTransformationFilter::NO_PADDING
                    ) // StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 7: // CCM
            {
                CCM<AES, TAG_SIZE>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv, 12);
                decryptor.SpecifyDataLengths( 0, ciphertextSize -TAG_SIZE, 0 );
                AuthenticatedDecryptionFilter df( decryptor,
                    new FileSink( output )
                ); // AuthenticatedDecryptionFilter
                StringSource ss2( ciphertext, true,
                    new Redirector( df )
                ); // StringSource
                break;
            }    
            case 8: // GCM
            {
                GCM<AES>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
                AuthenticatedDecryptionFilter df( decryptor,
                    new FileSink( output ),
                    0, TAG_SIZE
                ); // AuthenticatedDecryptionFilter

                // The StringSource dtor will be called immediately
                //  after construction below. This will cause the
                //  destruction of objects it owns. To stop the
                //  behavior so we can get the decoding result from
                //  the DecryptionFilter, we must use a redirector
                //  or manually Put(...) into the filter without
                //  using a StringSource.
                StringSource ss2( ciphertext, true,
                    new Redirector( df /*, PASS_EVERYTHING */ )
                ); // StringSource

                // If the object does not throw, here's the only
                //  opportunity to check the data's integrity
                // if( true == df.GetLastResult() ) {
                //     cout << "recovered text: " << output << endl;
                // }
                break;
            }
            default:
                return;
        }
        std::string result;
        FileSource(output, true,
            new StringSink(result)
        );
        std::cout << "Recovered text: " << result << "\n";

    } catch (const Exception& e) {
        std::cerr << "Decryption Error: " << e.what() << std::endl;
    }
}

void DESEncrypt(const int mode, const char *plaintext, const char *output, const byte *key, const byte *iv) {
    try {
        switch (mode)
        {
            case 1: // ECB
            {
                ECB_Mode<DES_EDE3>::Encryption encryptor;
                encryptor.SetKey(key, DES_EDE3::DEFAULT_KEYLENGTH);
                StringSource ss1( plaintext, true, 
                    new StreamTransformationFilter( encryptor,
                        new FileSink( output ),
                        BlockPaddingSchemeDef::PKCS_PADDING
                    ) // StreamTransformationFilter      
                ); // StringSource
                break;
            }
            case 2: // CBC
            {
                CBC_Mode<DES_EDE3>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, DES_EDE3::DEFAULT_KEYLENGTH, iv);
                StringSource ss( plaintext, true, 
                    new StreamTransformationFilter( encryptor,
                        new FileSink( output ),
                        BlockPaddingSchemeDef::PKCS_PADDING
                    ) // StreamTransformationFilter      
                ); // StringSource
                break;
            }
            case 3: // OFB
            {
                OFB_Mode<DES_EDE3>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, DES_EDE3::DEFAULT_KEYLENGTH, iv);
                StringSource(plaintext, true,
                    new StreamTransformationFilter(encryptor,
                        new FileSink( output )
                    ) //StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 4: // CFB
            {
                OFB_Mode<DES_EDE3>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, DES_EDE3::DEFAULT_KEYLENGTH, iv);
                StringSource( plaintext, true, 
                    new StreamTransformationFilter(encryptor,
                        new FileSink( output )
                    ) // StreamTransformationFilter      
                ); // StringSource
                break;
            }
            case 5: // CTR
            {
                CTR_Mode<DES_EDE3>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, DES_EDE3::DEFAULT_KEYLENGTH, iv);
                StringSource ss1( plaintext, true, 
                    new StreamTransformationFilter( encryptor,
                        new FileSink( output )
                    ) // StreamTransformationFilter      
                ); // StringSource
                break;
            }
            case 6: // XTS
            {
                XTS_Mode<DES_EDE3>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, DES_EDE3::DEFAULT_KEYLENGTH * 2, iv);
                StringSource ss( plaintext, true, 
                    new StreamTransformationFilter( encryptor,
                        new FileSink( output ),
                        StreamTransformationFilter::NO_PADDING
                    ) // StreamTransformationFilter      
                ); // StringSource
                break;
            }
            case 7: // CCM
            {
                CCM<DES_EDE3, TAG_SIZE>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, DES_EDE3::DEFAULT_KEYLENGTH, iv, 12);
                size_t len = std::strlen(plaintext);
                encryptor.SpecifyDataLengths( 0, len, 0 );

                StringSource ss1( plaintext, true,
                    new AuthenticatedEncryptionFilter( encryptor,
                        new FileSink( output )
                    ) // AuthenticatedEncryptionFilter
                ); // StringSource
                break;
            }
            case 8: // GCM
            {
                GCM<DES_EDE3>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, DES_EDE3::DEFAULT_KEYLENGTH, iv);

                StringSource ss1( plaintext, true,
                    new AuthenticatedEncryptionFilter( encryptor,
                        new FileSink( output ), false, TAG_SIZE
                    ) // AuthenticatedEncryptionFilter
                ); // StringSource
                break;
            }
            default:
                return;
        }

    } catch (const Exception& e) {
        std::cerr << "Encryption Error: " << e.what() << std::endl;
    }
    PrintHexFromFile("Ciphertext: ", output);
}

void DESDecrypt(const int mode, const char *ciphertext, const char *output, const byte *key, const byte *iv) {
    try {
        switch (mode)
        {
            case 1: // ECB
            {
                ECB_Mode<DES_EDE3>::Decryption decryptor;
                decryptor.SetKey(key, DES_EDE3::DEFAULT_KEYLENGTH);
                StringSource ss3( ciphertext, true, 
                    new StreamTransformationFilter( decryptor,
                        new FileSink( output ),
                        BlockPaddingSchemeDef::PKCS_PADDING
                    ) // StreamTransformationFilter
                ); // StringSource

                break;
            }
            case 2: // CBC
            {
                CBC_Mode<DES_EDE3>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, DES_EDE3::DEFAULT_KEYLENGTH, iv);
                StringSource ss( ciphertext, true, 
                    new StreamTransformationFilter( decryptor,
                        new FileSink( output ),
                        BlockPaddingSchemeDef::PKCS_PADDING
                    ) // StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 3: // OFB
            {
                OFB_Mode<DES_EDE3>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, DES_EDE3::DEFAULT_KEYLENGTH, iv);
                CryptoPP::StringSource(ciphertext, true,
                    new CryptoPP::StreamTransformationFilter(decryptor,
                        new CryptoPP::FileSink(output)
                    ) //StreamTransformationFilter
                ); //StringSource
                break;
            }
            case 4: // CFB
            {
                OFB_Mode<DES_EDE3>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, DES_EDE3::DEFAULT_KEYLENGTH, iv);
                StringSource ss3( ciphertext, true, 
                    new StreamTransformationFilter( decryptor,
                        new FileSink( output )
                    ) // StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 5: // CTR
            {
                CTR_Mode<DES_EDE3>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, DES_EDE3::DEFAULT_KEYLENGTH, iv);
                StringSource ss3( ciphertext, true, 
                    new StreamTransformationFilter( decryptor,
                        new FileSink( output )
                    ) // StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 6: // XTS
            {
                XTS_Mode<DES_EDE3>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, DES_EDE3::DEFAULT_KEYLENGTH * 2, iv, DES_EDE3::BLOCKSIZE);
                StringSource ss( ciphertext, true, 
                    new StreamTransformationFilter( decryptor,
                        new FileSink( output ),
                        StreamTransformationFilter::NO_PADDING
                    ) // StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 7: // CCM
            {
                CCM<DES_EDE3, TAG_SIZE>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, DES_EDE3::DEFAULT_KEYLENGTH, iv, 12);
                // decryptor.SpecifyDataLengths( 0, ciphertext.size() -TAG_SIZE, 0 );
                AuthenticatedDecryptionFilter df( decryptor,
                    new FileSink( output )
                ); // AuthenticatedDecryptionFilter
                StringSource ss2( ciphertext, true,
                    new Redirector( df )
                ); // StringSource
                break;
            }    
            case 8: // GCM
            {
                GCM<DES_EDE3>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, DES_EDE3::DEFAULT_KEYLENGTH, iv);
                AuthenticatedDecryptionFilter df( decryptor,
                    new FileSink( output ),
                    0, TAG_SIZE
                ); 
                StringSource ss2( ciphertext, true,
                    new Redirector( df /*, PASS_EVERYTHING */ )
                ); // StringSource
                break;
            }
            default:
                return;
        }
        std::string result;
        FileSource(output, true,
            new StringSink(result)
        );
        std::cout << "Recovered text: " << result << "\n";

    } catch (const Exception& e) {
        std::cerr << "Decryption Error: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 8) printHelp();
    std::map<std::string, std::string> args;
    std::map<std::string, int> modeMap = {
        {"ECB", 1},
        {"CBC", 2},
        {"OFB", 3},
        {"CFB", 4},
        {"CTR", 5},
        {"XTS", 6},
        {"CCM", 7},
        {"GCM", 8}
    };

    // Abundantly allocating key and iv for later use
    byte *key = new byte[AES::DEFAULT_KEYLENGTH * 2];
    byte *iv = new byte[AES::BLOCKSIZE];

    char *algo = argv[1];
    char *modes = argv[2];
    char *keyfile = argv[3];
    char *ivfile = argv[4];
    char *plaintext = argv[5];
    char *ciphertext = argv[6];
    char *usage = argv[7];
    char *outputType = argv[8];
    char *outputFile = argv[9];

    int mode = modeMap[modes];

    char* filename;
    if (!strcmp(keyfile, "auto")) 
    {
        filename = "keyfile.bin";
        GenerateKey(algo, mode, key, iv);
        SaveKeyToFile(algo, mode, filename, key, iv);
    }    
    else 
    {
        filename = keyfile;
        LoadKeyFromFile(algo, mode, filename, key, iv);
    }
    
    std::string plain;
    FileSource(plaintext, true, new StringSink(plain));
    char *input = new char[plain.size()];
    std::memcpy(input, plain.data(), plain.size());

    if (!strcmp(algo, "AES"))
    {
        AESEncrypt(mode, input, ciphertext, key, iv);
        std::string cipher;
        FileSource(ciphertext, true, new StringSink(cipher));
        char *nextInput = new char[cipher.size()];
        std::memcpy(nextInput, cipher.data(), cipher.size());
        std::cout << cipher.size() << "\n";
        AESDecrypt(mode, nextInput, outputFile, key, iv, cipher.size());
    }
    else
    {
        DESEncrypt(mode, input, ciphertext, key, iv);
        std::string cipher;
        FileSource(ciphertext, true, new StringSink(cipher));
        char *nextInput = new char[cipher.size()];
        std::memcpy(nextInput, cipher.data(), cipher.size());
        DESDecrypt(mode, nextInput, outputFile, key, iv);
    }


    // Choose either AES or DES with mode
    

    // std::string filename1 = "keyfile.bin";
    // CryptoPP::SecByteBlock keySec, ivSec;
    // byte *key = new byte[DES_EDE3::DEFAULT_KEYLENGTH * 2];
    // byte *iv = new byte[DES_EDE3::BLOCKSIZE];
    // char filename[] = "keyfile.bin";
    // char plain[] = "tung tung tung sahur";
    // std::string ciphertext, output;


    // AESmode = i;
    // std::string ciphertext = "", output = "";
    // std::cout << "Mode: " << i << "\n";
    // GenerateAESKey(AESmode, keySec, ivSec);
    // SaveKeyToFile(filename1, keySec, ivSec);

    // LoadKeyFromFile(AESmode, filename, key, iv);
    
    // DESEncrypt(AESmode, plain, ciphertext, key, iv);
    // DESDecrypt(AESmode, ciphertext, output, key, iv);
    
    

    return 0;
}