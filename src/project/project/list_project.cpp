#include "project/database.hpp"
#include "logger.hpp"
#include "project.hpp"
#include <memory>
#include <optional>
#include <sqlite3.h>

using namespace AgniVault::Project;

void Project::list_project(std::optional<std::string> projectName) {
  sqlite3 *db;
  std::unique_ptr<Database> database = std::make_unique<Database>(db);

  std::vector<ProjectRow> rowsOut;

  bool isFound = false;
  bool returnStatus = database->selectProject(projectName, isFound, rowsOut);
  if (!returnStatus) {

    LOG_ERROR("Project List Failed");
    return;
  }
  if (!isFound) {
    LOG_INFO("Project Name Not Found");
    return;
  }

  LOG_INFO("List:");
  for (const auto &row : rowsOut) {
    std::cout << "\n  ID: " << row.id << "\n  Name: " << row.name
              << "\n  Created: " << row.created_at
              << "\n  Updated: " << row.updated_at << std::endl;
  }
}
