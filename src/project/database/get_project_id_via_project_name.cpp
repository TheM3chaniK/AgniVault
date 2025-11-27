#include "logger.hpp"
#include "project/database.hpp"
#include <sqlite3.h>

using namespace AgniVault::Project;

bool Database::getProjectIdViaProjectName(std::string &projName,
                                          std::string &projectId) {
  const std::string sql =
      "SELECT id FROM " + projectTableName + " WHERE name = :name";

  sqlite3_stmt *stmt = nullptr;

  if (!this->prpareStmt(&stmt, sql)) {
    return false;
  }

  int idxName = sqlite3_bind_parameter_index(stmt, ":name");
  sqlite3_bind_text(stmt, idxName, projName.c_str(), -1, SQLITE_TRANSIENT);

  int returnCode = sqlite3_step(stmt);

  if (returnCode == SQLITE_ROW) {
    // extract the ID from column index 0
    const unsigned char *id = sqlite3_column_text(stmt, 0);
    if (id) {
      projectId = reinterpret_cast<const char *>(id);
    }

    sqlite3_finalize(stmt);
    return true;
  }

  if (returnCode == SQLITE_DONE) {
    // no such project found
    LOG_WARN("No project found with name: " << projName);
    sqlite3_finalize(stmt);
    return false;
  }

  // any other error
  LOG_DEBUG_ERROR("SQLite getProjectIdViaProjectName() failed. ReturnCode = "
                  << returnCode);
  sqlite3_finalize(stmt);
  return false;
}
