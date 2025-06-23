#include <iostream>
#include <string>
#include "cryptopp/filters.h"
using CryptoPP::StringSink;
#include "cryptopp/hex.h"
#include <cryptopp/files.h>
using CryptoPP::FileSource;
using CryptoPP::FileSink;

// Header for hash funtions
#include "cryptopp/sha.h" // sha-1, sha-2
#include "cryptopp/sha3.h" // sha3
#include "cryptopp/shake.h" // shake

using std::string;
using std::wstring;

#include <locale>
using std::wstring_convert;
#include <codecvt>
using  std::codecvt_utf8;

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" {
    EXPORT const char* __cdecl hash_char(const int mode, const char* char_message, size_t shakeLen);
}

wstring s2ws(const string& str) {
    wstring_convert<codecvt_utf8<wchar_t>> towstring;
    return towstring.from_bytes(str);
}

string ws2s(const wstring& str) {
    wstring_convert<codecvt_utf8<wchar_t>> tostring;
    return tostring.to_bytes(str);
}

CryptoPP::HashTransformation* createHash(const int mode, size_t shakeLen = 32) {
    if (mode == 1) return new CryptoPP::SHA224();
    else if (mode == 2) return new CryptoPP::SHA256();
    else if (mode == 3) return new CryptoPP::SHA384();
    else if (mode == 4) return new CryptoPP::SHA512();
    else if (mode == 5) return new CryptoPP::SHA3_224();
    else if (mode == 6) return new CryptoPP::SHA3_256();
    else if (mode == 7) return new CryptoPP::SHA3_384();
    else if (mode == 8) return new CryptoPP::SHA3_512();
    else if (mode == 9) return new CryptoPP::SHAKE128(shakeLen);
    else if (mode == 10) return new CryptoPP::SHAKE256(shakeLen);

    throw std::invalid_argument("Unsupported hash mode");
}

std::string hash(const int mode, const std::string& message, size_t shakeLen = 32) {
    // SHA224, SHA256, SHA384, SHA512, SHA3-224, SHA3-256, SHA3-384, SHA3-512, SHAKE128,SHAKE256
    CryptoPP::HashTransformation* hash = createHash(mode, shakeLen);
    // Hash algorithms information
    std::wcout << "Name: " << s2ws(hash->AlgorithmName()) << std::endl;
    std::wcout << "Digest size: " << hash->DigestSize() << std::endl;
    std::wcout << "Block size: " << hash->BlockSize() << std::endl;

    // Compute disgest
    std::string digest;
    hash->Restart(); 
    hash->Update((const CryptoPP::byte*)message.data(), message.size()); // setup input
    digest.resize(hash->DigestSize()); // set ouput length
    hash->TruncatedFinal((CryptoPP::byte*)&digest[0], digest.size()); // compute ouput

    // convert digest into hex format
    std::string encode;
    encode.clear();
    CryptoPP::StringSource ss(digest, true, 
        new CryptoPP::HexEncoder(
            new CryptoPP::StringSink (encode)
        )
    );
    std::wcout << "Digest string: " << s2ws(encode) << std::endl;
    return encode;
}

const char* hash_char(const int mode, const char* char_message, size_t shakeLen = 32)
{
    std::string message(char_message);
    std::string result = hash(mode, message, shakeLen);
    char* output = new char[result.size() + 1];
    std::strcpy(output, result.c_str());
    return output;
}

std::string read_utf8_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) throw std::runtime_error("Unable to open file: " + filename);

    std::string content(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    wstring wcontent = s2ws(content);
    std::cout << "Read file: " << filename << std::endl;
    std::wcout << L"File content: " << wcontent << std::endl;
    return content;
}

int main() {
    #ifdef _WIN32
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    #endif

    std::wcout << "Select hash algorithm:" << std::endl;
    std::wcout << "1. SHA224" << std::endl;
    std::wcout << "2. SHA256" << std::endl;
    std::wcout << "3. SHA384" << std::endl;
    std::wcout << "4. SHA512" << std::endl;
    std::wcout << "5. SHA3-224" << std::endl;
    std::wcout << "6. SHA3-256" << std::endl;
    std::wcout << "7. SHA3-384" << std::endl;
    std::wcout << "8. SHA3-512" << std::endl;
    std::wcout << "9. SHAKE128" << std::endl;
    std::wcout << "10. SHAKE256" << std::endl;
    int mode;
    size_t shakeLen = 32;
    // std::wcout << "Enter your choice (1-10): ";

    // std::wcin >> mode;
    mode = 6;
    if (mode == 9 || mode == 10) {
        std::wcout << "Enter output length for SHAKE (default 32): ";
        std::wcin >> shakeLen;
        if (shakeLen <= 0) shakeLen = 32;
    }

    // std::wcin.ignore();
    // std::wcin.ignore();
    std::wcout << "File or message to hash: ";
    std::wstring input = L"file";
    // std::getline(std::wcin, input);
    std::string message;
    if (input == L"file") {
        // std::wcin.ignore();
        std::wcout << "Enter file path: ";
        std::wstring filePath = L"hehe.txt";
        // std::getline(std::wcin, filePath);
        message = read_utf8_file(ws2s(filePath));
    }
    else message = ws2s(input);
    std::cout << "Digest: " << hash(mode, message, shakeLen);
}
