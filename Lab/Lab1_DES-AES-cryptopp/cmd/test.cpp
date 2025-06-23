#include <iostream>
#include <map>
#include <cstring>
#include <cryptopp/cryptlib.h>
#include <cryptopp/des.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <locale>
using std::wstring_convert;
#include <codecvt>
using  std::codecvt_utf8;
using namespace std;

int main() {

    std::wstring a;
    std::wcin >> a;
    std::wcout << "hehe " << a << endl;
    std::wcin >> a;
    std::wcout << "lmao " << a << endl;
}