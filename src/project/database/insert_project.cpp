#include "project/database.hpp"
#include "helper.hpp"
#include "logger.hpp"
#include <sqlite3.h>

using namespace AgniVault::Project;

bool Database::insertProject(std::string name) {

  const std::string sql = "INSERT INTO " + projectTableName +
                          "  (name, created_at, updated_at) "
                          " VALUES (:name, :created_at, :updated_at); ";
  sqlite3_stmt *stmt = nullptr;

  if (!this->prpareStmt(&stmt, sql)) {
    return false;
  }

  int idxName = sqlite3_bind_parameter_index(stmt, ":name");
  int idxCreatedAt = sqlite3_bind_parameter_index(stmt, ":created_at");
  int idxUpdatedAt = sqlite3_bind_parameter_index(stmt, ":updated_at");

  std::string currentTime = getCurrentIsoTime();
  sqlite3_bind_text(stmt, idxName, name.data(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, idxCreatedAt, currentTime.data(), -1,
                    SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, idxUpdatedAt, currentTime.data(), -1,
                    SQLITE_TRANSIENT);

  int returnCode = sqlite3_step(stmt);

  if (returnCode != SQLITE_DONE) {
    if (returnCode == SQLITE_CONSTRAINT ||
        returnCode == SQLITE_CONSTRAINT_UNIQUE) {
      LOG_INFO("Project name: " << name << " already exist");
      return false;

    } else {
      LOG_DEBUG_ERROR(
          "SQLITE Insert Failed at inertProject ReturnCode: " << returnCode);

      return false;
    }
  }
  sqlite3_finalize(stmt);
  return true;
}
