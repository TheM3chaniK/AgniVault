#include "file/database.hpp"
#include "helper.hpp"
#include "logger.hpp"
#include "project.hpp"
#include "project/database.hpp"
#include <memory>
#include <vector>

using namespace AgniVault::Project;
void Project::init_project(std::string name) {
  sqlite3 *db = nullptr;
  std::string dbPath = AgniVault::getDBPath();
  int rc = sqlite3_open(dbPath.c_str(), &db);

  if (rc != SQLITE_OK) {
    LOG_DEBUG_ERROR("Can't open database: " << sqlite3_errmsg(db));
    sqlite3_close(db);
    return;
  }

  Database projectDB(db);
  AgniVault::File::Database fileDB(db);

  if (!projectDB.createProjectTable()) {
    LOG_DEBUG_ERROR("Failed to create project table.");
    sqlite3_close(db);
    return;
  }
  if (!fileDB.createFileTable()) {
    LOG_DEBUG_ERROR("Failed to create file table.");
    sqlite3_close(db);
    return;
  }

  if (projectDB.insertProject(name)) {
    LOG_DEBUG_INFO("Project Successfully Created");
  } else {
    LOG_DEBUG_ERROR("Project Creation Failed!");
  }

  sqlite3_close(db);
}
