#include "database.hpp"
#include "logger.hpp"
#include "project.hpp"
#include <memory>

using namespace AgniVault::DB;

void Project::update_project(std::string oldName, std::string newName) {
  sqlite3 *db;
  std::unique_ptr<Database> database = std::make_unique<Database>(db);

  if (database->updateProject(oldName, newName)) {
    LOG_INFO("Project Name updated Successfully");
  }else{
    LOG_INFO("Project Name update failed");
  }

}
