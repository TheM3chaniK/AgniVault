#include "logger.hpp"
#include "project/database.hpp"

using namespace AgniVault::Project;

bool Database::createProjectTable() {
  const std::string sql = "CREATE TABLE IF NOT EXISTS " + projectTableName +
                          " ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "name TEXT UNIQUE NOT NULL,"
                          "created_at TEXT NOT NULL,"
                          "updated_at TEXT NOT NULL"
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
