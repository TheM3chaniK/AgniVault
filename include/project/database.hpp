#pragma once

#include "project_row.hpp"
#include <optional>
#include <sqlite3.h>
#include <string>
#include <vector>
namespace AgniVault {
namespace Project {
class Database {
private:
  const std::string projectTableName = "projects";
  sqlite3 *db = nullptr;
  enum ProjectColumns {
    COL_ID = 0,
    COL_NAME = 1,
    COL_CREATED_AT = 2,
    COL_UPDATED_AT = 3
  };

  bool prpareStmt(sqlite3_stmt **stmt, const std::string &sql);

public:
  Database(sqlite3 *db);
  ~Database();
  bool createProjectTable();
  bool insertProject(std::string name);
  bool updateProject(std::string oldName, std::string newName);
  bool selectProject(std::optional<std::string> projectName, bool &isFound,
                     std::vector<ProjectRow> &rowsOut);
  bool getProjectIdViaProjectName(std::string &projName, std::string &projId);
};
} // namespace Project
} // namespace AgniVault
