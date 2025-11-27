#include "file/database.hpp"
#include "helper.hpp"
#include "logger.hpp"
#include <sqlite3.h>

using namespace AgniVault::File;

bool Database::updateFile(int projectId, const std::string &path,
                          const std::vector<unsigned char> &encryptedData) {
  const std::string sql =
      "UPDATE files "
      "SET encrypted_data = :data, last_modified = :modified "
      "WHERE project_id = :pid AND path = :path;";

  sqlite3_stmt *stmt = nullptr;

  if (!this->prepareStmt(&stmt, sql)) {
    return false;
  }

  int idxPid = sqlite3_bind_parameter_index(stmt, ":pid");
  int idxPath = sqlite3_bind_parameter_index(stmt, ":path");
  int idxData = sqlite3_bind_parameter_index(stmt, ":data");
  int idxModified = sqlite3_bind_parameter_index(stmt, ":modified");

  std::string currentTime = getCurrentIsoTime();

  sqlite3_bind_int(stmt, idxPid, projectId);
  sqlite3_bind_text(stmt, idxPath, path.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_blob(stmt, idxData, encryptedData.data(), encryptedData.size(),
                    SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, idxModified, currentTime.c_str(), -1,
                    SQLITE_TRANSIENT);

  int returnCode = sqlite3_step(stmt);

  if (returnCode != SQLITE_DONE) {
    LOG_DEBUG_ERROR("updateFile error, ReturnCode: " << returnCode);
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}
