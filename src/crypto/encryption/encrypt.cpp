#include "encryption.hpp"
#include "logger.hpp"
#include <sodium/core.h>
#include <sodium/crypto_aead_xchacha20poly1305.h>
#include <sodium/randombytes.h>
#include <string>
#include <vector>

using namespace AgniVault::Crypto;

bool Encryption::encryptBuffer(const std::vector<unsigned char> &plainBytes,
                               unsigned char *key,
                               std::vector<unsigned char> &nonce,
                               std::string uname, std::string projectId,
                               std::vector<unsigned char> &outCipherBytes) {
  if (sodium_init() < 0) {
    LOG_DEBUG_ERROR("Sodium INIT error");
    return false;
  }
  if (plainBytes.empty())
    return false;

  unsigned char _nonce[NONCE_BYTES];
  randombytes_buf(_nonce, NONCE_BYTES);

  nonce.assign(_nonce, _nonce + NONCE_BYTES);

  unsigned long long generatedCipherTextLength;

  unsigned char ad[TAG_BYTES];
  this->derivedAd(ad, sizeof(ad), uname, projectId);
  outCipherBytes.resize(plainBytes.size() + TAG_BYTES);
  // Encrypting the data
  if (crypto_aead_xchacha20poly1305_ietf_encrypt(
          outCipherBytes.data(), &generatedCipherTextLength, plainBytes.data(),
          plainBytes.size(), ad, sizeof(ad), nullptr, _nonce, key) < 0) {
    LOG_ERROR("[-] Error while encrypting");
    return false;
  };

  return true;
}
