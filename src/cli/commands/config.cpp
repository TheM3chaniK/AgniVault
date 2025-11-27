#include "cli/commands/config.hpp"
#include "helper/config.hpp"
#include "logger.hpp"
#include <functional>
#include <map>
#include <string>

// Config subcommand handlers
int handleConfigUserName(int argc, char **argv) {
  if (argc == 3) { // No value given -> show current
    std::string name;
    if (AgniVault::getUsernameFromConfig(name)) {
      LOG_INFO("\tuser.name = " << name);
    } else {
      LOG_WARN("user.name is not set");
    }
    return 0;
  }

  // Set new username
  std::string name = argv[3];
  if (AgniVault::setUsernameInConfig(name)) {
    LOG_INFO("user.name updated to: " << name);
  } else {
    LOG_ERROR("Failed to update user.name!");
  }
  return 0;
}

int handleConfigUserEmail(int argc, char **argv) {
  if (argc == 3) { // No value given -> show current
    std::string email;
    if (AgniVault::getEmailFromConfig(email)) {
      LOG_INFO("\tuser.email = " << email);
    } else {
      LOG_WARN("user.email is not set");
    }
    return 0;
  }

  // Set new email
  std::string email = argv[3];
  if (AgniVault::setEmailInConfig(email)) {
    LOG_INFO("user.email updated to: " << email);
  } else {
    LOG_ERROR("Failed to update user.email!");
  }
  return 0;
}

int handleConfig(int argc, char **argv,
                 AgniVault::Project::Project & /*project*/) {
  if (argc < 3) {
    AgniVault::printConfigMsg();
    return 1;
  }

  std::string field = argv[2];
  std::map<std::string, std::function<int(int, char **)>> configSubcommands;
  configSubcommands["user.name"] = handleConfigUserName;
  configSubcommands["user.email"] = handleConfigUserEmail;

  auto it = configSubcommands.find(field);
  if (it != configSubcommands.end()) {
    return it->second(argc, argv);
  } else {
    LOG_ERROR("Unknown config key: " << field);
    LOG_INFO("Valid options:");
    LOG_INFO("\tagniv config user.name <value>");
    LOG_INFO("\tagniv config user.email <value>");
    return 1;
  }
}
