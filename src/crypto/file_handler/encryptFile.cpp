#include "file_handler.hpp"
#include "helper/file_helpers.hpp"
#include "helper/resolve_path.hpp"
#include "logger.hpp"
#include <string>

using namespace AgniVault::Crypto;

bool FileHandler::encryptFile(std::string filePath, unsigned char *key,
                              std::string &uname, std::string &projectName) {
  if (!AgniVault::File::readFile(AgniVault::resolve_path(filePath),
                                 this->fileContent)) {
    LOG_DEBUG_ERROR("File Can't Be Encrypted");
    return false;
  }
  std::vector<unsigned char> &plainBytes = fileContent;
  std::vector<unsigned char> nonce(this->encryption->getNonceByteSize());
  std::vector<unsigned char> outCipherBytes;
  // Currently Uname and Project Id is fixed.
  // Both Will be fetched from the db which will be made
  if (!this->encryption->encryptBuffer(plainBytes, key, nonce, uname,
                                       projectName, outCipherBytes)) {
    LOG_DEBUG_ERROR("Encryption Buffer Failed to Encrypt");
    return false;
  }

  outCipherBytes.insert(outCipherBytes.end(), nonce.begin(), nonce.end());

  if (!AgniVault::File::writeFile(filePath, outCipherBytes)) {
    LOG_DEBUG_ERROR("Can't write the CipherText into the file");
    ;
    return false;
  }
  return true;
}
