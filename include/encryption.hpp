#pragma once

#include <sodium/core.h>
#include <sodium/crypto_aead_xchacha20poly1305.h>
#include <stdexcept>
#include <string>
#include <vector>

namespace AgniVault {
namespace Crypto {
class Encryption {
private:
  static constexpr size_t KEY_BYTES =
      crypto_aead_xchacha20poly1305_ietf_KEYBYTES;
  static constexpr size_t NONCE_BYTES =
      crypto_aead_xchacha20poly1305_ietf_NPUBBYTES;
  static constexpr size_t TAG_BYTES = crypto_aead_xchacha20poly1305_ietf_ABYTES;

public:
  Encryption() {
    if (sodium_init() < 0) {
      throw std::runtime_error("Failed to initialize libsodium!");
    }
  }

  bool encryptBuffer(const std::vector<unsigned char> &plainBytes,
                     unsigned char *key, std::vector<unsigned char> &nonce,
                     std::string uname, std::string projectId,
                     std::vector<unsigned char> &outCipherBytes);
  bool decryptBuffer(std::vector<unsigned char> &outPlainBytes,
                     unsigned char *key, std::vector<unsigned char> &nonce,
                     std::string uname, std::string projectId,
                     const std::vector<unsigned char> &cipherBytes);
  bool generateKey(unsigned char *, size_t);
  bool derivedAd(unsigned char *ad, size_t adSize, std::string uname,
                 std::string projectId);
  static size_t getKeyByteSize();
  static size_t getNonceByteSize();
  static size_t getTagByteSize();

  bool readKey(std::string keyFilePath, std::vector<unsigned char> *key);
};
} // namespace Crypto
} // namespace AgniVault
