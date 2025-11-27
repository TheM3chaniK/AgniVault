#include "cli/commands/open.hpp"
#include "cli/usage.hpp"
#include "project.hpp"

int handleOpen(int argc, char **argv, AgniVault::Project::Project &project) {
  if (argc < 3) {
    printOpenUsage();
    return 1;
  }
  project.mount_project(argv[2]);
  return 0;
}