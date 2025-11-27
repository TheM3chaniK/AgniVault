#include "file/database.hpp"
#include "logger.hpp"
#include <sqlite3.h>

using namespace AgniVault::File;

bool Database::selectFilesByProjectId(int projectId,
                                      std::vector<FileRow> &filesOut) {
  const std::string sql =
      "SELECT id, project_id, path, encrypted_data, last_modified FROM " +
      filesTable + " WHERE project_id = :projectId;";

  sqlite3_stmt *stmt = nullptr;
  if (!this->prepareStmt(&stmt, sql)) {
    return false;
  }

  int idxProjectId = sqlite3_bind_parameter_index(stmt, ":projectId");
  sqlite3_bind_int(stmt, idxProjectId, projectId);

  int returnCode;
  while ((returnCode = sqlite3_step(stmt)) == SQLITE_ROW) {
    FileRow row;
    row.id = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
    row.project_id =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    row.path = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));

    const unsigned char *encryptedDataBlob =
        static_cast<const unsigned char *>(sqlite3_column_blob(stmt, 3));
    int encryptedDataSize = sqlite3_column_bytes(stmt, 3);
    row.encrypted_data.assign(encryptedDataBlob,
                              encryptedDataBlob + encryptedDataSize);

    row.last_modified =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));

    filesOut.push_back(row);
  }

  if (returnCode != SQLITE_DONE) {
    LOG_DEBUG_ERROR("SQLite selectFilesByProjectId() failed. ReturnCode = "
                    << returnCode);
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}
