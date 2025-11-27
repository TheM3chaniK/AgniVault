#include "cli/commands/close.hpp"
#include "cli/usage.hpp"
#include "project.hpp"

int handleClose(int argc, char **argv, AgniVault::Project::Project &project) {
  if (argc < 3) {
    // Umount all feature not yet ready
    // project.umount_project(); // umount all
    printCloseUsage();
    return 1;
  } else {
    project.umount_project(argv[2]); // umount one
  }
  return 0;
}