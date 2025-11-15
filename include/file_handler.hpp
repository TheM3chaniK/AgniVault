#pragma once

#include "encryption.hpp"
#include <memory>
#include <string>
#include <vector>
namespace AgniVault {
namespace Crypto {
class FileHandler {
private:
  std::vector<unsigned char> fileContent;
  std::unique_ptr<Encryption> encryption;

public:
  bool encryptFile(std::string filePath, unsigned char *key);
  bool decryptFile(std::string filePath, unsigned char *key);
};
} // namespace Crypto
} // namespace AgniVault
