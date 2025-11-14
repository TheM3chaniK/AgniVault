#include "encryption.hpp"
#include "logger.hpp"
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

int main() {
  AgniVault::Crypto::Encryption *encryption =
      new AgniVault::Crypto::Encryption();
  std::vector<unsigned char> key(encryption->getKeyByteSize());
  if (!encryption->generateKey(key.data(), key.size())) {
    LOG_ERROR("Key Generation Failed");
  }

  std::ifstream fin("test.txt", std::ios::binary);
  if (!fin) {
    LOG_ERROR("File can not be open");
    return 1;
  }
  fin.seekg(0, std::ios::end);
  size_t fileSize = fin.tellg();
  fin.seekg(0, std::ios::beg);
  std::vector<unsigned char> buffer(fileSize);
  fin.read(reinterpret_cast<char *>(buffer.data()), buffer.size());
  fin.close();

  std::vector<unsigned char> cipherText;
  std::vector<unsigned char> nonce;
  if (!encryption->encryptBuffer(buffer, key.data(), nonce, "TheM3chanik", "1234", cipherText)) {
    LOG_ERROR("Encryption Faild");
    return 1;
  }
  LOG_DEBUG_INFO("Encryption Successfull");
  std::ofstream fout("test.txt", std::ios::binary);

  LOG_DEBUG_INFO("Writing Encryption To The File");
  fout.write(reinterpret_cast<char *>(cipherText.data()), cipherText.size());
  // fout.write(reinterpret_cast<char *>(nonce.data()), nonce.size());
  LOG_INFO("Write complete");
  fout.close();
  delete encryption;
  return 0;
}
