#pragma once
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_GRAY "\033[90m"

inline std::string log_timestamp() {
  auto now = std::chrono::system_clock::now();
  auto t = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&t), "%H:%M:%S");
  return ss.str();
}

#define LOG_INFO(msg)                                                          \
  std::cerr << COLOR_BLUE << "[" << log_timestamp() << " INFO] "               \
            << COLOR_RESET << msg << std::endl

#define LOG_WARN(msg)                                                          \
  std::cerr << COLOR_YELLOW << "[" << log_timestamp() << " WARN] "             \
            << COLOR_RESET << msg << std::endl

#define LOG_ERROR(msg)                                                         \
  std::cerr << COLOR_RED << "[" << log_timestamp() << " ERROR] "               \
            << COLOR_RESET << msg << std::endl

// ───────────────────────────────────────────────
// Debug-only logs (active only in -DDEBUG builds)
// ───────────────────────────────────────────────
#ifdef DEBUG

#define LOG_DEBUG(msg)                                                         \
  std::cerr << COLOR_GRAY << "[" << log_timestamp() << " DEBUG] "              \
            << COLOR_RESET << msg << std::endl

#define LOG_DEBUG_INFO(msg)                                                    \
  std::cerr << COLOR_BLUE << "[" << log_timestamp() << " D-INFO] "             \
            << COLOR_RESET << msg << std::endl

#define LOG_DEBUG_WARN(msg)                                                    \
  std::cerr << COLOR_YELLOW << "[" << log_timestamp() << " D-WARN] "           \
            << COLOR_RESET << msg << std::endl

#define LOG_DEBUG_ERROR(msg)                                                   \
  std::cerr << COLOR_RED << "[" << log_timestamp() << " D-ERROR] "             \
            << COLOR_RESET << msg << std::endl

#else
// In release builds → do nothing
#define LOG_DEBUG(msg)                                                         \
  do {                                                                         \
  } while (0)
#define LOG_DEBUG_INFO(msg)                                                    \
  do {                                                                         \
  } while (0)
#define LOG_DEBUG_WARN(msg)                                                    \
  do {                                                                         \
  } while (0)
#define LOG_DEBUG_ERROR(msg)                                                   \
  do {                                                                         \
  } while (0)
#endif
