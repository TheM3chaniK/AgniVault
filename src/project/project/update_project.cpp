#include "project/database.hpp"
#include "helper.hpp"
#include "logger.hpp"
#include "project.hpp"
#include <memory>
#include <sqlite3.h>

using namespace AgniVault::Project;

void Project::update_project(std::string oldName, std::string newName) {
  sqlite3 *db = nullptr;
  std::string dbPath = AgniVault::getDBPath();
  int rc = sqlite3_open(dbPath.c_str(), &db);

  if (rc != SQLITE_OK) {
    LOG_DEBUG_ERROR("Can't open database: " << sqlite3_errmsg(db));
    sqlite3_close(db);
    return;
  }

  Database projectDB(db);

  if (projectDB.updateProject(oldName, newName)) {
    LOG_INFO("Project Name updated Successfully");
  } else {
    LOG_INFO("Project Name update failed");
  }

  sqlite3_close(db);
}
