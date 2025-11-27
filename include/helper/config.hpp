#pragma once

#include "helper/resolve_path.hpp"
#include <string>
#include <vector>
#include <yaml-cpp/node/node.h>
namespace AgniVault {
std::string getConfigFilePath();
bool fetchKeyFromConfig(std::vector<unsigned char> &key);

std::string getConfigFolderPath();
bool getUsernameFromConfig(std::string &uname);
bool getEmailFromConfig(std::string &email);

bool setUsernameInConfig(std::string &uname);
bool setEmailInConfig(std::string &email);

bool init_config();

std::string getKeyPath();
void saveConfig(const YAML::Node &node);
void printConfigMsg();

} // namespace AgniVault
