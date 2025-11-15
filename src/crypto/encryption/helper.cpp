#include "encryption.hpp"
#include "logger.hpp"
#include <sodium/crypto_generichash.h>
using namespace AgniVault::Crypto;

size_t Encryption::getKeyByteSize() { return KEY_BYTES; }
size_t Encryption::getNonceByteSize() { return NONCE_BYTES; }
size_t Encryption::getTagByteSize() { return TAG_BYTES; }

bool Encryption::derivedAd(unsigned char *ad, size_t adSize, std::string uname,
                           std::string projectId) {
  std::string additionalData =
      std::string("AgniVault") + "::" + uname + "::" + projectId;

  if (crypto_generichash(
          ad, adSize,
          reinterpret_cast<const unsigned char *>(additionalData.data()),
          additionalData.size(), nullptr, 0) < 0) {

    LOG_DEBUG("Couldn't generate additional data hash");

    return false;
  };

  return true;
}
