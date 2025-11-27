#include "file/database.hpp"
#include "helper.hpp"
#include "logger.hpp"
#include <sqlite3.h>

using namespace AgniVault::File;

Database::Database(sqlite3 *db) : db(db) {}

Database::~Database() {}

bool Database::prepareStmt(sqlite3_stmt **stmt, const std::string &sql) {
  int returnCode = sqlite3_prepare_v2(db, sql.data(), -1, stmt, nullptr);
  if (returnCode != SQLITE_OK) {
    LOG_DEBUG_ERROR("Sqlite Prepare Failed, ReturnCode: " << returnCode);
    return false;
  }
  return true;
}
