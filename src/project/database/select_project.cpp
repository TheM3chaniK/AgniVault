#include "project/database.hpp"
#include "logger.hpp"
#include "project_row.hpp"
#include <optional>
#include <sqlite3.h>
#include <string>

using namespace AgniVault::Project;

bool Database::selectProject(std::optional<std::string> projectName,
                             bool &isFound, std::vector<ProjectRow> &rowsOut) {
  std::string sql =
      projectName ? "SELECT * FROM " + projectTableName + " WHERE name = :name;"
                  : "SELECT * FROM " + projectTableName + ";";

  sqlite3_stmt *stmt = nullptr;
  if (!this->prpareStmt(&stmt, sql)) {
    return false;
  }

  if (projectName) {
    int idxName = sqlite3_bind_parameter_index(stmt, ":name");
    sqlite3_bind_text(stmt, idxName, projectName->c_str(), -1,
                      SQLITE_TRANSIENT);
  }

  int returnCode;
  bool found = false;

  auto safe = [](const unsigned char *val) {
    return val ? reinterpret_cast<const char *>(val) : "NULL";
  };

  while ((returnCode = sqlite3_step(stmt)) == SQLITE_ROW) {
    found = true;

    ProjectRow row;
    row.id = safe(sqlite3_column_text(stmt, COL_ID));
    row.name = safe(sqlite3_column_text(stmt, COL_NAME));
    row.created_at = safe(sqlite3_column_text(stmt, COL_CREATED_AT));
    row.updated_at = safe(sqlite3_column_text(stmt, COL_UPDATED_AT));

    rowsOut.push_back(row);
  }

  isFound = found;

  if (returnCode != SQLITE_DONE) {
    LOG_DEBUG_ERROR(
        "sqlite3_step error Select Project, ReturnCode: " << returnCode);
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}
