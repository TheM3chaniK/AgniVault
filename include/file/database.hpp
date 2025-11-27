#pragma once

#include "file_row.hpp"
#include <sqlite3.h>
#include <string>
#include <vector>
namespace AgniVault {
namespace File {
class Database {
private:
  const std::string filesTable = "files";
  sqlite3 *db = nullptr;
  bool prepareStmt(sqlite3_stmt **stmt, const std::string &sql);

public:
  Database(sqlite3 *db);
  ~Database();
  bool createFileTable();

  bool insertFile(int projectId, const std::string &path,
                  const std::vector<unsigned char> &encryptedData);
  bool updateFile(int projectId, const std::string &path,
                  const std::vector<unsigned char> &encryptedData);
  bool deleteFile(int projectId, const std::string &path);
  bool selectFilesByProjectId(int projectId, std::vector<FileRow> &filesOut);
};
} // namespace File
} // namespace AgniVault
