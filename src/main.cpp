#include "helper/config.hpp"
#include "logger.hpp"
#include "project.hpp"
#include <functional>
#include <map>
#include <string>

#include "cli/usage.hpp"
#include "cli/commands/init.hpp"
#include "cli/commands/update.hpp"
#include "cli/commands/list.hpp"
#include "cli/commands/open.hpp"
#include "cli/commands/view.hpp"
#include "cli/commands/close.hpp"
#include "cli/commands/gen_key.hpp"
#include "cli/commands/config.hpp"


int main(int argc, char **argv) {
  AgniVault::init_config();
  if (argc < 2) {
    printUsage();
    return 1;
  }

  std::string cmd = argv[1];
  AgniVault::Project::Project project;

  std::map<std::string, std::function<int(int, char **, AgniVault::Project::Project &)>>
      commands;
  commands["init"] = handleInit;
  commands["update"] = handleUpdate;
  commands["list"] = handleList;
  commands["open"] = handleOpen;
  commands["view"] = handleView;
  commands["close"] = handleClose;
  commands["gen-key"] = handleGenKey;
  commands["config"] = handleConfig;

  auto it = commands.find(cmd);
  if (it != commands.end()) {
    return it->second(argc, argv, project);
  } else {
    LOG_INFO("Unknown command: " << cmd);
    printUsage();
    return 1;
  }
}
