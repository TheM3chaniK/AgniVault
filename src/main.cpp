#include "encryption.hpp"
#include "file_handler.hpp"
#include "logger.hpp"
#include "project.hpp"
#include <string>
#include <vector>

int main(int argc, char **argv) {

  if (argc < 2) {
    LOG_INFO("Usage:");
    LOG_INFO("  init <project_name>");
    LOG_INFO("  update <old_project_name> <new_project_name>");
    LOG_INFO("  gen-key <keyfile>");
    LOG_INFO("  encrypt <file> <keyfile>");
    LOG_INFO("  decrypt <file> <keyfile>");
    return 0;
  }

  std::string cmd = argv[1];
  if (cmd == "init") {
    if (argc < 3) {
      LOG_INFO("Usage:");
      LOG_INFO("  init <project_name>");
      return 0;
    }
    std::string projectName = argv[2];
    AgniVault::DB::Project project;
    std::vector<unsigned char> key;
    project.init_project(projectName, key);
  }
  if (cmd == "update") {
    if (argc < 4) {
      LOG_INFO("Usage:");
      LOG_INFO("  update <old_project_name> <new_project_name>");
      return 0;
    }
    std::string oldName = argv[2];
    std::string newName = argv[3];
    AgniVault::DB::Project project;
    project.update_project(oldName, newName);
  }

  return 0;
}
