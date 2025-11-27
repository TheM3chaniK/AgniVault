#include "cli/commands/update.hpp"
#include "cli/usage.hpp"
#include "project.hpp"

int handleUpdate(int argc, char **argv, AgniVault::Project::Project &project) {
  if (argc < 4) {
    printUpdateUsage();
    return 1;
  }
  project.update_project(argv[2], argv[3]);
  return 0;
}