#include "encryption.hpp"
#include "helper/config.hpp"
#include "helper/file_helpers.hpp"
#include "logger.hpp"
#include "project.hpp"
#include <filesystem>
#include <string>

void printUsage() {
  LOG_INFO(
      "Usage:"
      "\n\tagniv init <project_name>\t\tCreate a new project"
      "\n\tagniv update <old_name> <new_name>\tRename a project"
      "\n\tagniv list [project_name]\t\tList all projects or details of one"
      "\n\tagniv open <project_name>\t\tMount the project at "
      "/dev/shm/agnivault/"
      "\n\tagniv view \t\tView all open projects"
      "\n\tagniv close <project_name>\t\tUnmount a project "
      "project from /dev/shm/"
      "\n\tagniv gen-key\t\tGenerate a new master key"
      "\n\tagniv config <options>\t\tconfiguration");
}

void printInitUsage() { LOG_INFO("\nUsage:\n\tagniv init <project_name>"); }

void printUpdateUsage() {
  LOG_INFO("\nUsage:\n\tagniv update <old_project_name> <new_project_name>");
}

void printCloseUsage() { LOG_INFO("\nUsage:\n\tagniv close <project_name>"); }

int main(int argc, char **argv) {
  AgniVault::init_config();
  if (argc < 2) {
    printUsage();
    return 0;
  }

  std::string cmd = argv[1];
  AgniVault::Project::Project project;

  // ------------------------------- init -------------------------------
  if (cmd == "init") {
    if (argc < 3) {
      printInitUsage();
      return 0;
    }
    project.init_project(argv[2]);
    return 0;
  }

  // ------------------------------ update ------------------------------
  if (cmd == "update") {
    if (argc < 4) {
      printUpdateUsage();
      return 0;
    }
    project.update_project(argv[2], argv[3]);
    return 0;
  }

  // ------------------------------- list -------------------------------
  if (cmd == "list") {
    if (argc < 3) {
      project.list_project(); // list all
    } else {
      project.list_project(argv[2]); // list one
    }
    return 0;
  }

  // ------------------------------- open -------------------------------
  if (cmd == "open") {
    if (argc < 3) {
      LOG_INFO("\nUsage:\n\tagniv open <project_name>");
      return 0;
    }
    project.mount_project(argv[2]);
    return 0;
  }

  // ------------------------------- view -------------------------------
  if (cmd == "view") {
    LOG_INFO("Upcoming Feature!!");
  }
  // ------------------------------- close -------------------------------
  if (cmd == "close") {
    if (argc < 3) {
      // Umount all feature not yet ready
      // project.umount_project(); // umount all
      printCloseUsage();
    } else {
      project.umount_project(argv[2]); // umount one
    }
    return 0;
  }

  // ------------------------------- config -------------------------------
  if (cmd == "config") {
    if (argc < 3) {
      AgniVault::printConfigMsg();
      return 0;
    }

    std::string field = argv[2];

    // --------------------- config user.name ---------------------
    if (field == "user.name") {
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

    // --------------------- config user.email ---------------------
    if (field == "user.email") {
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

    // --------------------- unknown sub-command ---------------------
    LOG_ERROR("Unknown config key: " << field);
    LOG_INFO("Valid options:");
    LOG_INFO("\tagniv config user.name <value>");
    LOG_INFO("\tagniv config user.email <value>");
    return 0;
  }

  // --------------------------- gen-key -------------------------
  if (cmd == "gen-key") {
    if (std::filesystem::exists(AgniVault::getKeyPath())) {
      char ans;
      LOG_WARN("A master key already exists."
               "\n\tGenerating a new one will DELETE the old key permanently."
               "\n\tYou will NOT be able to recover it.");
      LOG_INFO("Do you want to continue? (y/N): ");

      std::cin >> ans;
      if (ans != 'y') {
        LOG_INFO("Terminating Key-Generation");
        return 0;
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

  // --------------------------- unknown command -------------------------
  LOG_INFO("Unknown command: " << cmd);
  printUsage();

  return 0;
}
