#include "database.hpp"
#include "encryption.hpp"
#include "logger.hpp"
#include "project.hpp"
#include <memory>
#include <vector>

using namespace AgniVault::DB;
void Project::init_project(std::string name, std::vector<unsigned char> &key) {
  sqlite3 *db;
  std::unique_ptr<Database> database = std::make_unique<Database>(db);

  std::vector<unsigned char> projectEncryptionKey(
      Crypto::Encryption::Encryption::getKeyByteSize());
  Crypto::Encryption encryption;
  encryption.generateKey(projectEncryptionKey.data(),
                         projectEncryptionKey.size());
  if (database->insertProject(name, projectEncryptionKey)) {
    LOG_DEBUG_INFO("Project Successfully Created");
  } else {
    LOG_DEBUG_ERROR("Project Creation Failed!");
  }
}
