#include "project/database.hpp"
#include "logger.hpp"
#include "project.hpp"
#include "project_row.hpp"
#include <exception>
#include <filesystem>
#include <memory>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace AgniVault::Project;

void Project::mount_project(std::string projectName) {

  sqlite3 *db;
  std::unique_ptr<Database> database = std::make_unique<Database>(db);
  std::vector<ProjectRow> rowsOut;
  bool isFound = false;
  bool returnStatus = database->selectProject(projectName, isFound, rowsOut);

  if (!returnStatus) {

    LOG_ERROR("Project Mount failed");
    return;
  }
  if (!isFound) {
    LOG_INFO("Project Name Not Found");
    return;
  }
  std::string projDir = PROJECT_BASE_DIR_TO_MOUNT + "/" + projectName;

  try {

    if (!std::filesystem::exists(PROJECT_BASE_DIR_TO_MOUNT)) {
      std::filesystem::create_directories(PROJECT_BASE_DIR_TO_MOUNT);
    }

    if (!std::filesystem::create_directories(projDir)) {
      std::filesystem::create_directories(projDir);
    }
    LOG_INFO(projectName << " Successfully Mounted at " << projDir << "\n\t RUN: cd "
                         << projDir);
    std::filesystem::current_path(projDir);
  } catch (const std::exception &e) {
    LOG_DEBUG_ERROR("Error mounting the project: " << e.what());
  }
}
