#pragma once

#include <chrono>
#include <string>

namespace AgniVault {
inline std::string getCurrentIsoTime(){

  auto now = std::chrono::system_clock::now();
  auto now_sec = std::chrono::floor<std::chrono::seconds>(now);
  std::string result = std::format("{:%Y-%m-%dT%H:%M:%SZ}", now_sec);

  return result;
}
} // namespace AgniVault
