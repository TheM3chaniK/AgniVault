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

  bool encryptBuffer(const std::vector<unsigned char> &plianBytes,
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
  size_t getKeyByteSize();
};
} // namespace Crypto
} // namespace AgniVault
