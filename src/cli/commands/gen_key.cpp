#include "cli/commands/gen_key.hpp"
#include "encryption.hpp"
#include "helper/config.hpp"
#include "helper/file_helpers.hpp"
#include "logger.hpp"
#include <filesystem>
#include <iostream> // Required for std::cin
#include <vector>

int handleGenKey(int /*argc*/, char ** /*argv*/,
                 AgniVault::Project::Project & /*project*/) {
  if (std::filesystem::exists(AgniVault::getKeyPath())) {
    char ans;
    LOG_WARN("A master key already exists." 
             "\n\tGenerating a new one will DELETE the old key permanently." 
             "\n\tYou will NOT be able to recover it.");
    LOG_INFO("Do you want to continue? (y/N): ");

    std::cin >> ans;
    if (ans != 'y') {
      LOG_INFO("Terminating Key-Generation");
      return 1;
    } else {
      LOG_WARN("Deleteing Old Key!");
      std::filesystem::remove(AgniVault::getKeyPath());
    }
  }
  AgniVault::Crypto::Encryption encryption;
  std::vector<unsigned char> key(encryption.getKeyByteSize());
  encryption.generateKey(key.data(), key.size());

  AgniVault::File::writeFile(AgniVault::getKeyPath(), key);
  LOG_INFO("Key Generation Successfull");
  return 0;
}
