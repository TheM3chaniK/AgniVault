#include "encryption.hpp"
#include "file_handler.hpp"
#include "helper/file_helpers.hpp"
#include "helper/resolve_path.hpp"
#include "logger.hpp"

#include <fstream>
#include <string>
#include <vector>

void writeKey(const std::string &path, const unsigned char *key, size_t len) {
  std::ofstream fout(path, std::ios::binary);
  if (!fout) {
    LOG_DEBUG_ERROR("Failed to open key file for writing");
    exit(1);
  }
  fout.write(reinterpret_cast<const char *>(key), len);
}

std::vector<unsigned char> readKey(const std::string &path) {
  std::ifstream fin(path, std::ios::binary);
  if (!fin) {
    LOG_DEBUG_ERROR("Failed to open key file");
    exit(1);
  }
  size_t size = AgniVault::File::getFileSize(fin);
  std::vector<unsigned char> key(size);
  fin.read(reinterpret_cast<char *>(key.data()), size);
  return key;
}

int main(int argc, char **argv) {

  if (argc < 2) {
    LOG_DEBUG_INFO("Usage:");
    LOG_DEBUG_INFO("  gen-key <keyfile>");
    LOG_DEBUG_INFO("  encrypt <file> <keyfile>");
    LOG_DEBUG_INFO("  decrypt <file> <keyfile>");
    return 0;
  }

  std::string cmd = argv[1];
  AgniVault::Crypto::FileHandler fileHandler;
  AgniVault::Crypto::Encryption encryption;

  // ----------------- KEY GENERATION -----------------
  if (cmd == "gen-key") {
    if (argc < 3) {
      LOG_DEBUG_ERROR("Missing <keyfile> argument");
      return 1;
    }

    std::string keyfile = AgniVault::resolve_path(argv[2]);

    std::vector<unsigned char> key(
        AgniVault::Crypto::Encryption::getKeyByteSize());
    encryption.generateKey(key.data(), key.size());

    writeKey(keyfile, key.data(),
             AgniVault::Crypto::Encryption::getKeyByteSize());

    LOG_DEBUG_INFO("Key generated: " << keyfile);
    return 0;
  }

  // ----------------- ENCRYPT -----------------
  if (cmd == "encrypt") {
    if (argc < 4) {
      LOG_DEBUG_ERROR("Usage: encrypt <file> <keyfile>");
      return 1;
    }

    std::string filePath = AgniVault::resolve_path(argv[2]);
    std::string keyfile = AgniVault::resolve_path(argv[3]);

    auto key = readKey(keyfile);

    if (!fileHandler.encryptFile(filePath, key.data())) {
      LOG_DEBUG_ERROR("Encryption failed.");
      return 1;
    }

    LOG_DEBUG_INFO("Encryption successful.");
    return 0;
  }

  // ----------------- DECRYPT -----------------
  if (cmd == "decrypt") {
    if (argc < 4) {
      LOG_DEBUG_ERROR("Usage: decrypt <file> <keyfile>");
      return 1;
    }

    std::string filePath = AgniVault::resolve_path(argv[2]);
    std::string keyfile = AgniVault::resolve_path(argv[3]);

    auto key = readKey(keyfile);

    if (!fileHandler.decryptFile(filePath, key.data())) {
      LOG_DEBUG_ERROR("Decryption failed.");
      return 1;
    }

    LOG_DEBUG_INFO("Decryption successful.");
    return 0;
  }

  LOG_DEBUG_ERROR("Unknown command: " << cmd);
  LOG_DEBUG_INFO("Valid commands: gen-key, encrypt, decrypt");
  return 1;
}
