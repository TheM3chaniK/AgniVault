#include "file/database.hpp"

#include "file/database.hpp"
#include "logger.hpp"
#include <sqlite3.h>

using namespace AgniVault::File;

bool Database::deleteFile(int projectId, const std::string &path) {

  const std::string sql =
      "DELETE FROM files WHERE project_id = :pid AND path = :path;";

  sqlite3_stmt *stmt = nullptr;

  if (!this->prepareStmt(&stmt, sql)) {
    return false;
  }

  int idxPid = sqlite3_bind_parameter_index(stmt, ":pid");
  int idxPath = sqlite3_bind_parameter_index(stmt, ":path");

  sqlite3_bind_int(stmt, idxPid, projectId);
  sqlite3_bind_text(stmt, idxPath, path.c_str(), -1, SQLITE_TRANSIENT);

  int returnCode = sqlite3_step(stmt);

  if (returnCode != SQLITE_DONE) {
    LOG_DEBUG_ERROR("deleteFile step error, ReturnCode: " << returnCode);
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);

  // Optional: check if file actually deleted
  if (sqlite3_changes(db) == 0) {
    LOG_INFO("No file found to delete for path: " << path);
    return false;
  }

  LOG_INFO("File deleted: " << path);
  return true;
}
