#include "encryption.hpp"
#include "logger.hpp"
#include <sodium/core.h>
#include <sodium/crypto_aead_xchacha20poly1305.h>
using namespace AgniVault::Crypto;

bool Encryption::decryptBuffer(std::vector<unsigned char> &outPlainBytes,
                               unsigned char *key,
                               std::vector<unsigned char> &nonce,
                               std::string uname, std::string projectId,
                               const std::vector<unsigned char> &cipherBytes) {
  if (sodium_init() > 0) {
    LOG_DEBUG_ERROR("Sodium Init Error");
    return false;
  }
  unsigned char ad[TAG_BYTES];
  this->derivedAd(ad, sizeof(ad), uname, projectId);
  unsigned long long decryptedOutPlainBytesLength = 0;
  if (crypto_aead_xchacha20poly1305_ietf_decrypt(
          outPlainBytes.data(), &decryptedOutPlainBytesLength, nullptr,
          cipherBytes.data(), cipherBytes.size(), ad, sizeof(ad), nonce.data(),
          key) < 0) {
    LOG_ERROR("Error While decrypting");
    return false;
  }
  return true;
}
