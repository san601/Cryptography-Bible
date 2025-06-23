//C internal library 
#include <iostream>
using std::wcin;
using std::wcout;
using std::wcerr;
using std::endl;
#include <string>
using std::string;
using std::wstring;
#include <cstdlib>
using std::exit;
#include "assert.h"

//Cryptopp Librari
#include <cryptopp/files.h>
using CryptoPP::FileSource;
using CryptoPP::FileSink;
using CryptoPP::BufferedTransformation;

#include "cryptopp/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::Redirector; // string to bytes

#include "cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::byte;

#include "cryptopp/cryptlib.h"
using CryptoPP::Exception;

// convert string
// Hex <---> Binary
#include "cryptopp/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

// Base64 <---> Binary
#include "cryptopp/base64.h"
using CryptoPP::Base64Encoder;
using CryptoPP::Base64Decoder;

// Block cipher
#include "cryptopp/des.h"
using CryptoPP::DES;
#include "cryptopp/aes.h"
using CryptoPP::AES;

//Mode of operations
#include "cryptopp/modes.h" //ECB, CBC, CBC-CTS, CFB, OFB, CTR
using CryptoPP::ECB_Mode;
using CryptoPP::CBC_Mode;
using CryptoPP::CFB_Mode;
using CryptoPP::OFB_Mode;
using CryptoPP::CTR_Mode;
#include "cryptopp/xts.h"
using CryptoPP::XTS;
#include <cryptopp/ccm.h>
using CryptoPP::CCM;
#include "cryptopp/gcm.h"
using CryptoPP::GCM;
//Ref: more here https://www.cryptopp.com/wiki/AEAD_Comparison


/* Set utf8 support for windows*/ 
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#else
#endif
/* Convert string <--> utf8*/ 
#include <locale>
using std::wstring_convert;
#include <codecvt>
using  std::codecvt_utf8;
wstring string_to_wstring (const std::string& str);
string wstring_to_string (const std::wstring& str);

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

const int TAG_SIZE = 12;

using namespace CryptoPP;

wstring string_to_wstring(const string& str);
string wstring_to_string(const wstring& str);

void GenerateKey(const char *algo, const int mode, CryptoPP::byte *key, CryptoPP::byte *iv) {
    int default_keylength, blocksize;
    if (!strcmp(algo, "AES")) 
    {
        default_keylength = AES::DEFAULT_KEYLENGTH;
        blocksize = AES::BLOCKSIZE;
    }
    else
    {
        default_keylength = DES::DEFAULT_KEYLENGTH;
        blocksize = DES::BLOCKSIZE;
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

void SaveKeyToFile(const char *algo, const int mode, const char *filename, const CryptoPP::byte *key, const CryptoPP::byte *iv) {
    int default_keylength, blocksize;
    if (!strcmp(algo, "AES")) 
    {
        default_keylength = AES::DEFAULT_KEYLENGTH;
        blocksize = AES::BLOCKSIZE;
    }
    else
    {
        default_keylength = DES::DEFAULT_KEYLENGTH;
        blocksize = DES::BLOCKSIZE;
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
    std::wcout << "Key and IV saved to: " << filename << std::endl;
}

// Load key and IV from binary file using FileSource and ArraySink
void LoadKeyFromFile(const char *algo, const int mode, const char *filename, CryptoPP::byte *key, CryptoPP::byte *iv) {
    int default_keylength, blocksize;
    if (!strcmp(algo, "AES")) 
        {
            default_keylength = AES::DEFAULT_KEYLENGTH;
            blocksize = AES::BLOCKSIZE;
        }
        else
        {
            default_keylength = DES::DEFAULT_KEYLENGTH;
            blocksize = DES::BLOCKSIZE;
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

        std::wcout << "Key and IV loaded from file: " << filename << std::endl;
    } catch (const CryptoPP::Exception& e) {
        std::cerr << "Error loading key and IV: " << e.what() << std::endl;
    }
}

void AESEncrypt(const int mode, const char *plaintext, bool saveToFile, const char *output, const byte *key, const byte *iv) {
    try {
        std::string result;
        switch (mode)
        {
            case 1: // ECB
            {
                ECB_Mode<AES>::Encryption encryptor;
                encryptor.SetKey(key, AES::DEFAULT_KEYLENGTH);
                StringSource ss1( plaintext, true, 
                    new StreamTransformationFilter( encryptor,
                        new StringSink( result ),
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
                        new StringSink( result )//,
                        // BlockPaddingSchemeDef::PKCS_PADDING
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
                        new StringSink( result )
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
                        new StringSink( result )
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
                        new StringSink( result )
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
                        new StringSink( result ),
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
                        new StringSink( result )
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
                        new StringSink( result ), false, TAG_SIZE
                    ) // AuthenticatedEncryptionFilter
                ); // StringSource
                break;
            }
            default:
                return;
        }
        if (saveToFile)
        {
            StringSource(result, true,
                new FileSink(output)
            );
            std::wcout << L"Ciphertext saved\n";
        }
    } catch (const Exception& e) {
        std::cerr << "Encryption Error: " << e.what() << std::endl;
    }
}

void AESDecrypt(const int mode, const byte *ciphertext, int size, bool saveToFile, const char *output, const byte *key, const byte *iv) {
    try {
        std::string result;
        switch (mode)
        {
            case 1: // ECB
            {
                // std::wcout << "Ciphertext: " << std::strlen(ciphertext) << "\n";
                ECB_Mode<AES>::Decryption decryptor;
                decryptor.SetKey(key, AES::DEFAULT_KEYLENGTH);
                StringSource ss3( ciphertext, size, true, 
                    new StreamTransformationFilter( decryptor,
                        new StringSink( result ),
                        BlockPaddingSchemeDef::PKCS_PADDING
                    ) // StreamTransformationFilter
                ); // StringSource

                break;
            }
            case 2: // CBC
            {
                CBC_Mode<AES>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
                StringSource ss( ciphertext, size, true, 
                    new StreamTransformationFilter( decryptor,
                        new StringSink( result )//,
                        // BlockPaddingSchemeDef::PKCS_PADDING
                    ) // StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 3: // OFB
            {
                OFB_Mode<AES>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
                StringSource(ciphertext, size, true,
                    new StreamTransformationFilter(decryptor,
                        new StringSink( result )
                    ) //StreamTransformationFilter
                ); //StringSource
                break;
            }
            case 4: // CFB
            {
                OFB_Mode<AES>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
                StringSource ss3( ciphertext, size, true, 
                    new StreamTransformationFilter( decryptor,
                        new StringSink( result )
                    ) // StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 5: // CTR
            {
                CTR_Mode<AES>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
                StringSource ss3( ciphertext, size, true, 
                    new StreamTransformationFilter( decryptor,
                        new StringSink( result )
                    ) // StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 6: // XTS
            {
                XTS_Mode<AES>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH * 2, iv, AES::BLOCKSIZE);
                StringSource ss( ciphertext, size, true, 
                    new StreamTransformationFilter( decryptor,
                        new StringSink( result ),
                        StreamTransformationFilter::NO_PADDING
                    ) // StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 7: // CCM
            {
                CCM<AES, TAG_SIZE>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv, 12);
                decryptor.SpecifyDataLengths( 0, size - TAG_SIZE, 0 );
                AuthenticatedDecryptionFilter df( decryptor,
                    new StringSink( result )
                ); // AuthenticatedDecryptionFilter
                StringSource ss2( ciphertext, size, true,
                    new Redirector( df )
                ); // StringSource
                break;
            }    
            case 8: // GCM
            {
                GCM<AES>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
                AuthenticatedDecryptionFilter df( decryptor,
                    new StringSink( result ),
                    0, TAG_SIZE
                ); // AuthenticatedDecryptionFilter

                // The StringSource dtor will be called immediately
                //  after construction below. This will cause the
                //  destruction of objects it owns. To stop the
                //  behavior so we can get the decoding result from
                //  the DecryptionFilter, we must use a redirector
                //  or manually Put(...) into the filter without
                //  using a StringSource.
                StringSource ss2( ciphertext, size, true,
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
        if (saveToFile)
        {
            StringSource(result, true,
                new FileSink(output)
            );
            std::wcout << L"Recovered text saved\n";
        }
    } catch (const Exception& e) {
        std::cerr << "Decryption Error: " << e.what() << std::endl;
    }
}

void DESEncrypt(const int mode, const char *plaintext, bool saveToFile, const char *output, const byte *key, const byte *iv) {
    try {
        std::string result;
        switch (mode)
        {
            case 1: // ECB
            {
                ECB_Mode<DES>::Encryption encryptor;
                encryptor.SetKey(key, DES::DEFAULT_KEYLENGTH);
                StringSource ss1( plaintext, true, 
                    new StreamTransformationFilter( encryptor,
                        new StringSink( result ),
                        BlockPaddingSchemeDef::PKCS_PADDING
                    ) // StreamTransformationFilter      
                ); // StringSource
                break;
            }
            case 2: // CBC
            {
                CBC_Mode<DES>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, DES::DEFAULT_KEYLENGTH, iv);
                StringSource ss( plaintext, true, 
                    new StreamTransformationFilter( encryptor,
                        new StringSink( result ),
                        BlockPaddingSchemeDef::PKCS_PADDING
                    ) // StreamTransformationFilter      
                ); // StringSource
                break;
            }
            case 3: // OFB
            {
                OFB_Mode<DES>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, DES::DEFAULT_KEYLENGTH, iv);
                StringSource(plaintext, true,
                    new StreamTransformationFilter(encryptor,
                        new StringSink( result )
                    ) //StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 4: // CFB
            {
                OFB_Mode<DES>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, DES::DEFAULT_KEYLENGTH, iv);
                StringSource( plaintext, true, 
                    new StreamTransformationFilter(encryptor,
                        new StringSink( result )
                    ) // StreamTransformationFilter      
                ); // StringSource
                break;
            }
            case 5: // CTR
            {
                CTR_Mode<DES>::Encryption encryptor;
                encryptor.SetKeyWithIV(key, DES::DEFAULT_KEYLENGTH, iv);
                StringSource ss1( plaintext, true, 
                    new StreamTransformationFilter( encryptor,
                        new StringSink( result )
                    ) // StreamTransformationFilter      
                ); // StringSource
                break;
            }
            default:
                std::wcout << "Unsupported mode on DES\n";
                return;
        }
        if (saveToFile)
        {
            StringSource(result, true,
                new FileSink(output)
            );
            std::wcout << L"Ciphertext saved\n";
        }
    } catch (const Exception& e) {
        std::cerr << "Encryption Error: " << e.what() << std::endl;
    }
}

void DESDecrypt(const int mode, const byte *ciphertext, int size, bool saveToFile, const char *output, const byte *key, const byte *iv) {
    try {
        std::string result;
        switch (mode)
        {
            case 1: // ECB
            {
                ECB_Mode<DES>::Decryption decryptor;
                decryptor.SetKey(key, DES::DEFAULT_KEYLENGTH);
                StringSource ss3( ciphertext, size, true, 
                    new StreamTransformationFilter( decryptor,
                        new StringSink( result ),
                        BlockPaddingSchemeDef::PKCS_PADDING
                    ) // StreamTransformationFilter
                ); // StringSource

                break;
            }
            case 2: // CBC
            {
                CBC_Mode<DES>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, DES::DEFAULT_KEYLENGTH, iv);
                StringSource ss( ciphertext, size, true, 
                    new StreamTransformationFilter( decryptor,
                        new StringSink( result ),
                        BlockPaddingSchemeDef::PKCS_PADDING
                    ) // StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 3: // OFB
            {
                OFB_Mode<DES>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, DES::DEFAULT_KEYLENGTH, iv);
                StringSource(ciphertext, size, true,
                    new StreamTransformationFilter(decryptor,
                        new StringSink( result )
                    ) //StreamTransformationFilter
                ); //StringSource
                break;
            }
            case 4: // CFB
            {
                OFB_Mode<DES>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, DES::DEFAULT_KEYLENGTH, iv);
                StringSource ss3( ciphertext, size, true, 
                    new StreamTransformationFilter( decryptor,
                        new StringSink( result )
                    ) // StreamTransformationFilter
                ); // StringSource
                break;
            }
            case 5: // CTR
            {
                CTR_Mode<DES>::Decryption decryptor;
                decryptor.SetKeyWithIV(key, DES::DEFAULT_KEYLENGTH, iv);
                StringSource ss3( ciphertext, size, true, 
                    new StreamTransformationFilter( decryptor,
                        new StringSink( result )
                    ) // StreamTransformationFilter
                ); // StringSource
                break;
            }
            default:
                return;
        }
        if (saveToFile)
        {
            StringSource(result, true,
                new FileSink(output)
            );
            std::wcout << L"Recovered text saved\n";
        }

    } catch (const Exception& e) {
        std::cerr << "Decryption Error: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[])
{
    #ifdef __linux__
    setlocale(LC_ALL, "");
    #elif _WIN32
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    #endif

    // Abundantly allocating key and iv for later use
    CryptoPP::byte *key = new CryptoPP::byte[AES::DEFAULT_KEYLENGTH * 2];
    CryptoPP::byte *iv = new CryptoPP::byte[AES::BLOCKSIZE];

    int aescipher;
    char *algo;
    int algoChoice, mode;
    std::wcout << "Choose your algorithm:\n"
          << "1. AES\n"
          << "2. DES\n"
          << "Please enter your number: ";
    std::wcin >> algoChoice;
    switch (algoChoice)
    {
        case 1:
        {
            algo = "AES";
            break;
        }
        case 2:
        {
            algo = "DES";
            break;
        }
        default:
        {
            std::wcout << "Invalid algorithm";
            return 1;
        }
    }
    std::wcout << "Choose your mode:\n"
        << "1. ECB\n"
        << "2. CBC\n"
        << "3. OFB\n"
        << "4. CFB\n"
        << "5. CTR\n"
        << "6. XTS\n"
        << "7. CCM\n"
        << "8. GCM\n"
        << "Please enter your number: ";
    std::wcin >> mode;
    if (!(mode >= 1 && mode <= 8))
    {
        std::wcout << "Invalid mode\n";
        return 1;
    } 

    std::wcout << "Would you like to encryption or decryption message:\n"
          << "1. key and iv generation;\n"
          << "2. encryption;\n"
          << "3. decryption;\n"
          << "Please enter your number: ";
    std::wcin >> aescipher;
    std::wcin.ignore();
    std::wstring filename;

    switch (aescipher) 
    {
        case 1: 
        {
            std::wcout << "File name to save: ";
            std::wcin >> filename;
            GenerateKey(algo, mode, key, iv);

            std::string file = wstring_to_string(filename);
            SaveKeyToFile(algo, mode, file.c_str(), key, iv);

            break;
        }
        case 2: 
        {
            // Load key
            std::wcin.ignore();
            std::wcout << "Load key from: ";
            std::wcin >> filename;

            std::string file = wstring_to_string(filename);
            LoadKeyFromFile(algo, mode, file.c_str(), key, iv);
            
            std::wcout << "Input from:\n"
                       << "1. Console;\n"
                       << "2. File;\n"
                       << "Please enter your number: ";
            int inputChoice;
            std::wcin >> inputChoice;
            std::wstring plaintext; 
            switch (inputChoice)
            {
                case 1:
                {
                    std::wcin.ignore();
                    std::wcout << "Nhap plaintext: ";
                    std::wcin.ignore();
                    std::getline(wcin, plaintext);
                    // std::wcout << plaintext << "\n";
                    break;
                }   
                case 2:
                {
                    std::wcin.ignore();
                    std::wstring filename;
                    std::wcout << "File name: ";
                    std::wcin >> filename;
                    std::string temp;
                    FileSource(wstring_to_string(filename).c_str(), true, new StringSink(temp));
                    plaintext = string_to_wstring(temp);
                    // std::wcout << plaintext << "\n";

                    break;
                }
                default:
                {
                    std::wcout << "Invalid input\n";
                    return 1;
                }
            }
            std::wstring output;
            std::wcout << "Output file: ";
            std::wcin >> output;
            // Encrypt
            auto start = std::chrono::high_resolution_clock::now();
        
            for (int i = 0; i < 1000; ++i) {
                if (!std::strcmp(algo, "AES"))
                {
                    AESEncrypt(mode, wstring_to_string(plaintext).c_str(), false, wstring_to_string(output).c_str(), key, iv);
                }
                else 
                {
                    DESEncrypt(mode, wstring_to_string(plaintext).c_str(), false, wstring_to_string(output).c_str(), key, iv);
                }
            }

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            double averageTime = static_cast<double>(duration) / 1000.0;
            std::wcout << L"Average time for encryption over 1000 rounds: " << averageTime << L" ms\n";
            

            // Create a ciphertext file for decryption process later
            if (!std::strcmp(algo, "AES"))
            {
                AESEncrypt(mode, wstring_to_string(plaintext).c_str(), true, wstring_to_string(output).c_str(), key, iv);
            }
            else 
            {
                DESEncrypt(mode, wstring_to_string(plaintext).c_str(), true, wstring_to_string(output).c_str(), key, iv);
            }

            // std::string result;
            // FileSource(
            //     wstring_to_string(output).c_str(), true, 
            //     new HexEncoder(
            //         new StringSink(result)
            //     )  
            // );
            // std::wcout << L"Result: " << string_to_wstring(result) << "\n";
            break;
        }
        case 3: 
        {
            // Load key
            std::wcin.ignore();
            std::wcout << "Load key from: ";
            std::wcin >> filename;

            std::string file = wstring_to_string(filename);
            LoadKeyFromFile(algo, mode, file.c_str(), key, iv);
            
            std::wcout << "Input from:\n"
                       << "1. Console (hex);\n"
                       << "2. File;\n"
                       << "Please enter your number: ";
            int inputChoice;
            std::wcin >> inputChoice;
            std::string ciphertext; 
            switch (inputChoice)
            {
                case 1:
                {
                    std::wstring temp;
                    std::wcin.ignore();
                    std::wcout << "Nhap ciphertext (hex): ";
                    std::wcin >> temp;
                    CryptoPP::StringSource ss(wstring_to_string(temp), true,
                        new CryptoPP::HexDecoder(
                            new CryptoPP::StringSink(ciphertext)
                        )
                    );
                    std::wcout << L"ciphertext size: " << ciphertext.size() << "\n";
                    break;
                }   
                case 2:
                {
                    std::wcin.ignore();
                    std::wstring filename;
                    std::wcout << "File name: ";
                    std::wcin >> filename;
                    std::string temp;
                    FileSource(wstring_to_string(filename).c_str(), true, new StringSink(temp));
                    ciphertext = temp;
                    std::wcout << L"ciphertext size: " << ciphertext.size() << "\n";
                    break;
                }
                default:
                {
                    std::wcout << "Invalid input\n";
                    return 1;
                }
            }
            std::wstring output;
            std::wcout << "Output file: ";
            std::wcin >> output;

            // Decrypt
            int len = ciphertext.size();
            CryptoPP::byte *cipherBytes = new CryptoPP::byte[len];
            std::memcpy(cipherBytes, ciphertext.data(), len);

            auto start = std::chrono::high_resolution_clock::now();
        
            for (int i = 0; i < 1000; ++i) 
            {
                if (!std::strcmp(algo, "AES")) 
                {
                    AESDecrypt(mode, cipherBytes, len, false, wstring_to_string(output).c_str(), key, iv);
                }   
                else 
                {
                    DESDecrypt(mode, cipherBytes, len, false, wstring_to_string(output).c_str(), key, iv);
                }    
            }

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            double averageTime = static_cast<double>(duration) / 1000.0;
            std::wcout << L"Average time for decryption over 1000 rounds: " << averageTime << L" ms\n";

            // Check result
            // if (!std::strcmp(algo, "AES")) 
            // {
            //     AESDecrypt(mode, cipherBytes, len, true, wstring_to_string(output).c_str(), key, iv);
            // }   
            // else 
            // {
            //     DESDecrypt(mode, cipherBytes, len, true, wstring_to_string(output).c_str(), key, iv);
            // }
            
            // std::string result;
            // FileSource(
            //     wstring_to_string(output).c_str(), true, 
            //     new StringSink(result)
            // );
            // std::wcout << L"Result: " << string_to_wstring(result) << "\n";
            // delete[] cipherBytes;
            // break;
        }
        default:
            std::wcout << L"Invalid input\n";
            break;
    }

    delete[] key;
    delete[] iv;
    return 0;
}

wstring string_to_wstring(const string& str) {
    wstring_convert<codecvt_utf8<wchar_t>> towstring;
    return towstring.from_bytes(str);
}

string wstring_to_string(const wstring& str) {
    wstring_convert<codecvt_utf8<wchar_t>> tostring;
    return tostring.to_bytes(str);
}
