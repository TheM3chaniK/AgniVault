#include "helper/resolve_path.hpp"
#include <cstdlib>
#include <filesystem>

namespace AgniVault {
std::string resolve_path(const std::string &inputPath) {
  std::filesystem::path p = inputPath;
  if (inputPath.starts_with("~/")) {
    p = std::filesystem::path(std::getenv("HOME")) / inputPath.substr(2);
  }

  return std::filesystem::absolute(p).string();
}
} // namespace AgniVault
