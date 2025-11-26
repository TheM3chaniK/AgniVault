#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>
namespace AgniVault {
namespace DB {
class Database {
private:
  static constexpr std::string DB_NAME = "agnivault.db";
  const std::string projectTableName = "projects";
  std::string DB_PATH = "~/.config/agnivault";
  sqlite3 *db = nullptr;

public:
  Database(sqlite3 *db);
  ~Database();
  static std::string getDBName();
  bool prpareStmt(sqlite3_stmt **stmt, const std::string &sql);
  bool openDB();
  bool createProjectTable();
  bool insertProject(std::string name,
                     std::vector<unsigned char> &encryptedProjKey);
  bool updateProject(std::string oldName, std::string newName);
};
} // namespace DB
} // namespace AgniVault
