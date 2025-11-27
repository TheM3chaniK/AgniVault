#include "cli/commands/view.hpp"
#include "logger.hpp"

int handleView(int /*argc*/, char ** /*argv*/,
               AgniVault::Project::Project & /*project*/) {
  LOG_INFO("Upcoming Feature!!");
  return 0;
}