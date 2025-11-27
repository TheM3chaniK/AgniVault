#include "file/database.hpp"
#include "helper.hpp"
#include "logger.hpp"
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace AgniVault::File;

bool Database::insertFile(int projectId, const std::string &path,
                          const std::vector<unsigned char> &encryptedData) {

  const std::string sql =
      "INSERT INTO files (project_id, path, encrypted_data, last_modified) "
      "VALUES (:project_id, :path, :encrypted_data, :last_modified);";

  sqlite3_stmt *stmt = nullptr;

  if (!this->prepareStmt(&stmt, sql)) {
    return false;
  }

  int idxProjectId = sqlite3_bind_parameter_index(stmt, ":project_id");
  int idxPath = sqlite3_bind_parameter_index(stmt, ":path");
  int idxEncData = sqlite3_bind_parameter_index(stmt, ":encrypted_data");
  int idxLastModified = sqlite3_bind_parameter_index(stmt, ":last_modified");

  sqlite3_bind_int(stmt, idxProjectId, projectId);
  sqlite3_bind_text(stmt, idxPath, path.data(), -1, SQLITE_TRANSIENT);

  sqlite3_bind_blob(stmt, idxEncData, encryptedData.data(),
                    encryptedData.size(), SQLITE_TRANSIENT);

  std::string lastModified = getCurrentIsoTime();
  sqlite3_bind_text(stmt, idxLastModified, lastModified.data(), -1,
                    SQLITE_TRANSIENT);

  int returnCode = sqlite3_step(stmt);

  if (returnCode != SQLITE_DONE) {
    LOG_DEBUG_ERROR("SQLite insertFile() failed. ReturnCode = " << returnCode);
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}
