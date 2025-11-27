#include "cli/commands/init.hpp"
#include "cli/usage.hpp"
#include "project.hpp"

int handleInit(int argc, char **argv, AgniVault::Project::Project &project) {
  if (argc < 3) {
    printInitUsage();
    return 1;
  }
  project.init_project(argv[2]);
  return 0;
}