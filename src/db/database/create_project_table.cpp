#include "database.hpp"
#include "logger.hpp"

using namespace AgniVault::DB;

bool Database::createProjectTable() {
  const std::string sql = "CREATE TABLE " + projectTableName +
                          " ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "name TEXT UNIQUE NOT NULL,"
                          "created_at TEXT NOT NULL,"
                          "updated_at TEXT NOT NULL,"
                          "encrypted_project_key BLOB NOT NULL"
                          ");";
  char *errmsg = nullptr;
  int returnCode = sqlite3_exec(db, sql.data(), nullptr, nullptr, &errmsg);

  if (returnCode != SQLITE_OK) {
    LOG_DEBUG_ERROR(
        "Can't Create Project Table / Table Already Exist, ReturnCode "
        << returnCode);
    return false;
  }
  return true;
}
