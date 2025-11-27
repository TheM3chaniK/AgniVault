#include "project/database.hpp"
#include "helper.hpp"
#include "logger.hpp"
#include "project.hpp"
#include <memory>
#include <optional>
#include <sqlite3.h>

using namespace AgniVault::Project;

void Project::list_project(std::optional<std::string> projectName) {
  sqlite3 *db = nullptr;
  std::string dbPath = AgniVault::getDBPath();
  int rc = sqlite3_open(dbPath.c_str(), &db);

  if (rc != SQLITE_OK) {
    LOG_DEBUG_ERROR("Can't open database: " << sqlite3_errmsg(db));
    sqlite3_close(db);
    return;
  }

  Database projectDB(db);

  std::vector<ProjectRow> rowsOut;

  bool isFound = false;
  bool returnStatus = projectDB.selectProject(projectName, isFound, rowsOut);
  if (!returnStatus) {
    LOG_ERROR("Project List Failed");
    sqlite3_close(db);
    return;
  }
  if (!isFound) {
    LOG_INFO("Project Name Not Found");
    sqlite3_close(db);
    return;
  }

  LOG_INFO("List:");
  for (const auto &row : rowsOut) {
    std::cout << "\n  ID: " << row.id << "\n  Name: " << row.name
              << "\n  Created: " << row.created_at
              << "\n  Updated: " << row.updated_at << std::endl;
  }

  sqlite3_close(db);
}
