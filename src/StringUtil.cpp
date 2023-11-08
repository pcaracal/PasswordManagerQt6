//
// Created by caracal on 11/1/23.
//

#include <string>
#include "StringUtil.hpp"
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <sstream>
#include <iostream>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <random>

std::string StringUtil::generate_salt() {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::stringstream ss;
  for (int i = 0; i < 32; i++) {
    ss << std::hex << gen();
  }
  return StringUtil::sha3_512(ss.str());
}

std::string StringUtil::sha3_512(const std::string &input) {
  EVP_MD_CTX *mdctx;
  const EVP_MD *md;
  unsigned char hash[64];
  unsigned int hash_len;

  md = EVP_sha3_512();
  mdctx = EVP_MD_CTX_new();

  EVP_DigestInit_ex(mdctx, md, nullptr);
  EVP_DigestUpdate(mdctx, input.c_str(), input.length());
  EVP_DigestFinal_ex(mdctx, hash, &hash_len);

  EVP_MD_CTX_free(mdctx);

  std::string output;
  for (int i = 0; i < hash_len; i++) {
    char buf[3];
    sprintf(buf, "%02x", hash[i]);
    output += buf;
  }
  return output;
}

std::string StringUtil::aes_encrypt(const std::string &input, const std::string &keyPlain) {
  std::string key = sha3_512(keyPlain);
  key = key.substr(0, 32);

  CryptoPP::SecByteBlock keyBytes((const unsigned char *) key.data(), key.size());

  CryptoPP::AES::Encryption aesEncryption(keyBytes, keyBytes.size());

  CryptoPP::ECB_Mode_ExternalCipher::Encryption ecbEncryption(aesEncryption);

  std::string cipherText;
  CryptoPP::StringSource(input, true,
                         new CryptoPP::StreamTransformationFilter(ecbEncryption,
                                                                  new CryptoPP::StringSink(cipherText)));

  std::string hexEncoded;
  CryptoPP::HexEncoder hexEncoder(new CryptoPP::StringSink(hexEncoded));
  hexEncoder.Put((const unsigned char *) cipherText.data(), cipherText.size());
  hexEncoder.MessageEnd();

  return hexEncoded;
}

std::string StringUtil::aes_decrypt(const std::string &input, const std::string &keyPlain) {
  std::string key = sha3_512(keyPlain);
  key = key.substr(0, 32);

  CryptoPP::SecByteBlock keyBytes((const unsigned char *) key.data(), key.size());

  CryptoPP::AES::Decryption aesDecryption(keyBytes, keyBytes.size());

  CryptoPP::ECB_Mode_ExternalCipher::Decryption ecbDecryption(aesDecryption);

  std::string decryptedText;
  CryptoPP::HexDecoder decryptor(new CryptoPP::StringSink(decryptedText));
  decryptor.Put((const unsigned char *) input.data(), input.size());
  decryptor.MessageEnd();

  std::string decryptedText2;
  CryptoPP::StreamTransformationFilter stfDecryptor(ecbDecryption, new CryptoPP::StringSink(decryptedText2));
  stfDecryptor.Put((const unsigned char *) decryptedText.data(), decryptedText.size());
  stfDecryptor.MessageEnd();

  return decryptedText2;
}