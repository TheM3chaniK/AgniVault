#include "project/database.hpp"
#include "encryption.hpp"
#include "logger.hpp"
#include "project.hpp"
#include <memory>
#include <vector>

using namespace AgniVault::Project;
void Project::init_project(std::string name) {
  sqlite3 *db;
  std::unique_ptr<Database> database = std::make_unique<Database>(db);

  std::vector<unsigned char> projectEncryptionKey(
      Crypto::Encryption::Encryption::getKeyByteSize());
  Crypto::Encryption encryption;
  encryption.generateKey(projectEncryptionKey.data(),
                         projectEncryptionKey.size());
  if (database->insertProject(name)) {
    LOG_DEBUG_INFO("Project Successfully Created");
  } else {
    LOG_DEBUG_ERROR("Project Creation Failed!");
  }
}
