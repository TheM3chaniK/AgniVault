#include "encryption.hpp"
#include "logger.hpp"
#include "sodium/core.h"

using namespace AgniVault::Crypto;
bool Encryption::generateKey(unsigned char *key, size_t keyLen) {
  if (sodium_init() < 0) {
    LOG_DEBUG_ERROR("Sodium can not be initialised");

    return false;
  }
  if (keyLen < KEY_BYTES) {
    LOG_DEBUG("Size of passed key array in generatekey is lesser than"
              << KEY_BYTES);
    return false;
  }
  crypto_aead_xchacha20poly1305_ietf_keygen(key);

  return true;
}
