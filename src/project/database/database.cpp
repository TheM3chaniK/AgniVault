#include "project/database.hpp"
#include "helper.hpp"
#include "logger.hpp"
#include <sqlite3.h>
#include <string>

using namespace AgniVault::Project;

Database::Database(sqlite3 *db) {
  this->db = db;
  openDB();
  createProjectTable();
}

Database::~Database() { sqlite3_close(db); }

bool Database::openDB() {
  int returnCode = sqlite3_open(
      reinterpret_cast<char *>(getDBPath().data()),
      &db);
  if (returnCode != SQLITE_OK) {
    LOG_DEBUG_ERROR("Can't Open the AgniVault DB");
    return false;
  }
  LOG_DEBUG("Database Created / Open Successfully");

  return true;
}

bool Database::prpareStmt(sqlite3_stmt **stmt, const std::string &sql) {

  int returnCode = sqlite3_prepare_v2(db, sql.data(), -1, stmt, nullptr);
  if (returnCode != SQLITE_OK) {
    LOG_DEBUG_ERROR("Sqlite Prepare Failed, ReturnCode: " << returnCode);
    return false;
  }
  return true;
}
