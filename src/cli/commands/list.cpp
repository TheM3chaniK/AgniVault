#include "cli/commands/list.hpp"
#include "project.hpp"

int handleList(int argc, char **argv, AgniVault::Project::Project &project) {
  if (argc < 3) {
    project.list_project(); // list all
  } else {
    project.list_project(argv[2]); // list one
  }
  return 0;
}