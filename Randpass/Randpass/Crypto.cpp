#include "Crypto.h"

// Function to derive a key from a password using PBKDF2
void Crypto::DeriveKeyFromPassword(const std::string& password, byte* derivedKey, size_t derivedKeyLength)
{
    PKCS5_PBKDF2_HMAC<SHA256> pbkdf2;
    pbkdf2.DeriveKey(derivedKey, derivedKeyLength, 0, reinterpret_cast<const byte*>(password.data()), password.size(), nullptr, 0, 1000); // Adjust the iteration count as needed
}

void Crypto::DeriveIVFromPassword(const std::string& password, byte* derivedIV, size_t derivedIVLength)
{
    PKCS5_PBKDF2_HMAC<SHA256> pbkdf2;
    pbkdf2.DeriveKey(derivedIV, derivedIVLength, 0, reinterpret_cast<const byte*>(password.data()), password.size(), nullptr, 0, 1000); // Adjust the iteration count as needed
}

std::string Crypto::AESEncrypt(const std::string& plainText, const std::string& password)
{
    std::string cipherText;

    byte key[AES::DEFAULT_KEYLENGTH];
    Crypto::DeriveKeyFromPassword(password, key, AES::DEFAULT_KEYLENGTH);

    byte iv[AES::BLOCKSIZE];
    DeriveIVFromPassword(password, iv, AES::BLOCKSIZE);

    try
    {
        CBC_Mode<AES>::Encryption encryption;
        encryption.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);

        StringSource(plainText, true,
            new StreamTransformationFilter(encryption,
                new StringSink(cipherText)
            )
        );
    }
    catch (const Exception& e)
    {
        //std::cout << "wrong password, exiting..." << '\n';
        std::string eg;
        std::cin >> eg;
        exit(1);
    }

    return cipherText;
}

std::string Crypto::AESDecrypt(const std::string& cipherText, const std::string& password)
{
    std::string decryptedText;

    byte key[AES::DEFAULT_KEYLENGTH];
    Crypto::DeriveKeyFromPassword(password, key, AES::DEFAULT_KEYLENGTH);

    byte iv[AES::BLOCKSIZE];
    DeriveIVFromPassword(password, iv, AES::BLOCKSIZE);

    try
    {
        CBC_Mode<AES>::Decryption decryption;
        decryption.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);

        StringSource(cipherText, true,
            new StreamTransformationFilter(decryption,
                new StringSink(decryptedText)
            )
        );
    }
    catch (const Exception& e)
    {
        return "Fail";
    }

    return decryptedText;
}

std::string Crypto::Base64Encode(const std::string& plainText)
{
    std::string encodedText;
    StringSource(plainText, true,
        new Base64Encoder(
            new StringSink(encodedText),
            false // do not append line breaks
        )
    );
    return encodedText;
}

std::string Crypto::Base64Decode(const std::string& encodedText)
{
    std::string decodedText;
    StringSource(encodedText, true,
        new Base64Decoder(
            new StringSink(decodedText)
        )
    );
    return decodedText;
}
