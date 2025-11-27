#include "cli/usage.hpp"

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

void printOpenUsage() { LOG_INFO("\nUsage:\n\tagniv open <project_name>"); }

void printCloseUsage() { LOG_INFO("\nUsage:\n\tagniv close <project_name>"); }

