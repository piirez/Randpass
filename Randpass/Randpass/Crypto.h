#pragma once
#include <iostream>
#include <string>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/sha.h> // Add this line for SHA-256

using namespace CryptoPP;

class Crypto
{
private:
    static void DeriveKeyFromPassword(const std::string& password, byte* derivedKey, size_t derivedKeyLength);
    static void DeriveIVFromPassword(const std::string& password, byte* derivedIV, size_t derivedIVLength);
public:
    // Function to perform AES encryption
    static std::string AESEncrypt(const std::string& plainText, const std::string& password);

    // Function to perform AES decryption
    static std::string AESDecrypt(const std::string& cipherText, const std::string& password);

    static std::string Base64Encode(const std::string& plainText);

    static std::string Base64Decode(const std::string& encodedText);
};

