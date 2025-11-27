#include "file/database.hpp"
#include "logger.hpp"

using namespace AgniVault::File;

bool Database::createFileTable() {

  const std::string sql = "CREATE TABLE IF NOT EXISTS " + filesTable +
                          " ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"

                          "project_id INTEGER NOT NULL,"
                          "path TEXT NOT NULL,"

                          "encrypted_data BLOB NOT NULL,"
                          "last_modified TEXT NOT NULL,"
                          ""
                          "FOREIGN KEY(project_id) REFERENCES projects(id)"
                          ");";
  char *errmsg = nullptr;
  int returnCode = sqlite3_exec(db, sql.data(), nullptr, nullptr, &errmsg);

  if (returnCode != SQLITE_OK) {
    LOG_DEBUG_ERROR("Can't Create File Table. ReturnCode: "
                    << returnCode
                    << " ErrorMsg: " << (errmsg ? errmsg : "NULL"));
    sqlite3_free(errmsg);
    return false;
  }
  return true;
}
