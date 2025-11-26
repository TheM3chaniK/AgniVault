#include "database.hpp"
#include "helper.hpp"
#include "logger.hpp"
#include <sqlite3.h>

using namespace AgniVault::DB;

bool Database::updateProject(std::string oldName, std::string newName) {

  const std::string sql = "update " + projectTableName +
                          " SET updated_at = :updated_at, name "
                          "= :newName WHERE name = :oldName; ";
  sqlite3_stmt *stmt = nullptr;

  if (!this->prpareStmt(&stmt, sql)) {
    return false;
  }


  int idxUpdateAt = sqlite3_bind_parameter_index(stmt, ":updated_at");
  int idxOldName = sqlite3_bind_parameter_index(stmt, ":oldName");
  int idxNewName = sqlite3_bind_parameter_index(stmt, ":newName");

  std::string currentTime = getCurrentIsoTime();

  sqlite3_bind_text(stmt, idxUpdateAt, currentTime.data(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, idxOldName, oldName.data(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, idxNewName, newName.data(), -1, SQLITE_TRANSIENT);

  int returnCode = sqlite3_step(stmt);
  if (returnCode != SQLITE_DONE) {
    LOG_DEBUG_ERROR(
        "update Project Sqlite3 step error, ReturnCode: " << returnCode);
    return false;
  }

  sqlite3_finalize(stmt);

  return true;
}
