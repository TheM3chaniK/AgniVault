#include "helper/config.hpp"
#include "helper/file_helpers.hpp"
#include "helper/resolve_path.hpp"
#include "logger.hpp"
#include <filesystem>
#include <fstream>
#include <yaml-cpp/yaml.h>

namespace AgniVault {

void printConfigMsg() {
  LOG_WARN("Config not set. Please configure AgniVault:\n"
           "\tagniv config user.name <name>\n"
           "\tagniv config user.email <email>");
}
std::string getConfigFolderPath() {
  return resolve_path("~/.config/agnivault");
}

std::string getConfigFilePath() {
  return resolve_path("~/.config/agnivault/config.yaml");
}

bool init_config() {

  // Create folder if missing
  if (!std::filesystem::exists(getConfigFolderPath())) {
    std::filesystem::create_directories(getConfigFolderPath());
  }

  // If file does not exist, warn the user
  if (!std::filesystem::exists(getConfigFilePath())) {
    return false;
  }

  // If exists, verify that fields exist & not empty
  YAML::Node node = YAML::LoadFile(getConfigFilePath());

  if (!node["name"] || node["name"].as<std::string>().empty() ||
      !node["email"] || node["email"].as<std::string>().empty()) {
    printConfigMsg();
    return false;
  }

  return true;
}

bool getUsernameFromConfig(std::string &uname) {
  if (!std::filesystem::exists(getConfigFilePath()))
    return false;

  YAML::Node node = YAML::LoadFile(getConfigFilePath());
  if (!node["name"])
    return false;

  uname = node["name"].as<std::string>();
  return true;
}

bool getEmailFromConfig(std::string &email) {
  if (!std::filesystem::exists(getConfigFilePath()))
    return false;

  YAML::Node node = YAML::LoadFile(getConfigFilePath());
  if (!node["email"])
    return false;

  email = node["email"].as<std::string>();
  return true;
}

bool setUsernameInConfig(std::string &uname) {
  YAML::Node node;
  if (std::filesystem::exists(getConfigFilePath()))
    node = YAML::LoadFile(getConfigFilePath());

  node["name"] = uname;
  saveConfig(node);
  return true;
}

bool setEmailInConfig(std::string &email) {
  YAML::Node node;
  if (std::filesystem::exists(getConfigFilePath()))
    node = YAML::LoadFile(getConfigFilePath());

  node["email"] = email;
  saveConfig(node);
  return true;
}

void saveConfig(const YAML::Node &node) {
  std::ofstream out(getConfigFilePath());
  out << node;
  out.close();
}
std::string getKeyPath() {
  std::string keyPath = getConfigFolderPath() + "/VaultCode";
  return keyPath;
}

bool fetchKeyFromConfig(std::vector<unsigned char> &key) {
  std::string keyPath = getKeyPath();
  return File::readFile(keyPath, key);
}

} // namespace AgniVault
