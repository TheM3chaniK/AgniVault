#include "encryption.hpp"
#include "file_handler.hpp"
#include "helper/file_helpers.hpp"
#include "helper/resolve_path.hpp"
#include "logger.hpp"
#include <memory>
#include <vector>

using namespace AgniVault::Crypto;
bool FileHandler::decryptFile(std::string filePath, unsigned char *key) {
  std::vector<unsigned char> cipherBytesWithNonce;
  if (!AgniVault::File::readFile(AgniVault::resolve_path(filePath),
                                 cipherBytesWithNonce)) {
    LOG_DEBUG_ERROR("Can't Read File");
    return false;
  }
  if (cipherBytesWithNonce.size() <= this->encryption->getNonceByteSize()) {
    LOG_DEBUG_ERROR("Total Bytes can't be less than or equal to "
                    << this->encryption->getNonceByteSize() << " bytes");
    return false;
  }
  std::vector<unsigned char> cipherBytes(cipherBytesWithNonce.size() -
                                         this->encryption->getNonceByteSize());

  cipherBytes.assign(cipherBytesWithNonce.begin(),
                     cipherBytesWithNonce.end() -
                         this->encryption->getNonceByteSize());
  std::vector<unsigned char> nonceBytes(this->encryption->getNonceByteSize());

  nonceBytes.assign(cipherBytesWithNonce.end() -
                        this->encryption->getNonceByteSize(),
                    cipherBytesWithNonce.end());

  std::vector<unsigned char> outPlainBytes(cipherBytes.size() -
                                           this->encryption->getTagByteSize());

  // Currently Uname and Project Id is fixed.
  // Both Will be fetched from the db which will be made
  if (!this->encryption->decryptBuffer(outPlainBytes, key, nonceBytes, "M3K",
                                       "1234", cipherBytes)) {
    LOG_DEBUG_ERROR("File Decryption Failed");
    return false;
  }

  File::writeFile(filePath, outPlainBytes);

  return true;
}
