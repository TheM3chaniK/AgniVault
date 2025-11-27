#include "file/database.hpp"
#include "file_row.hpp"
#include "encryption.hpp"
#include "file_handler.hpp"
#include "helper/config.hpp"
#include "helper/file_helpers.hpp"
#include "project/database.hpp"
#include "helper.hpp"
#include "logger.hpp"
#include "project.hpp"
#include "project_row.hpp"
#include <exception>
#include <filesystem>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace AgniVault::Project;

void Project::mount_project(std::string projectName) {

  sqlite3 *db = nullptr;
  std::string dbPath = AgniVault::getDBPath();
  int rc = sqlite3_open(dbPath.c_str(), &db);

  if (rc != SQLITE_OK) {
    LOG_DEBUG_ERROR("Can't open database: " << sqlite3_errmsg(db));
    sqlite3_close(db);
    return;
  }

  Database projectDB(db);
  std::vector<ProjectRow> rowsOut;
  bool isFound = false;
  bool returnStatus = projectDB.selectProject(projectName, isFound, rowsOut);

  if (!returnStatus) {
    LOG_ERROR("Project Mount failed");
    sqlite3_close(db);
    return;
  }
  if (!isFound) {
    LOG_INFO("Project Name Not Found");
    sqlite3_close(db);
    return;
  }
  std::string projDir = PROJECT_BASE_DIR_TO_MOUNT + "/" + projectName;

  try {

    if (!std::filesystem::exists(PROJECT_BASE_DIR_TO_MOUNT)) {
      std::filesystem::create_directories(PROJECT_BASE_DIR_TO_MOUNT);
    }

    if (!std::filesystem::create_directories(projDir)) {
      std::filesystem::create_directories(projDir);
    }
    
    // Log success before checking for files, as directory creation is successful
    LOG_INFO(projectName << " Successfully Mounted at " << projDir
                         << "\n\t RUN: cd " << projDir);
    std::filesystem::current_path(projDir);

    // Get project ID
    std::string projectID_str;
    if (!projectDB.getProjectIdViaProjectName(projectName, projectID_str)) {
      LOG_DEBUG_ERROR("Project ID not found for mount operation.");
      sqlite3_close(db);
      return;
    }
    int projectID = std::stoi(projectID_str);

    AgniVault::File::Database fileDB(db);
    std::vector<AgniVault::FileRow> filesToMount;

    if (!fileDB.selectFilesByProjectId(projectID, filesToMount)) {
      LOG_DEBUG_ERROR("Failed to retrieve files for project: " << projectName);
      sqlite3_close(db);
      return;
    }

    if (filesToMount.empty()) {
      // No files to mount, but directory already created and success logged
      sqlite3_close(db);
      return;
    }

    // Decrypt and write files
    AgniVault::Crypto::Encryption encryption;
    AgniVault::Crypto::FileHandler fileHandler; 

    std::vector<unsigned char> key;
    if (!AgniVault::fetchKeyFromConfig(key)) {
      LOG_DEBUG_WARN("Key Not found in config; To generate new key, RUN: agniv gen-key");
      sqlite3_close(db);
      return;
    }
    std::string name;
    std::string email;
    if (!AgniVault::getUsernameFromConfig(name) || !AgniVault::getEmailFromConfig(email)) {
      AgniVault::printConfigMsg();
      sqlite3_close(db);
      return;
    }
    std::string uname = name + email;


    for (const auto &fileRow : filesToMount) {
      LOG_DEBUG("Processing file: " << fileRow.path);
      std::vector<unsigned char> fullEncryptedData = fileRow.encrypted_data;
      size_t nonceSize = AgniVault::Crypto::Encryption::getNonceByteSize();

      LOG_DEBUG("Full Encrypted Data Size: " << fullEncryptedData.size());
      LOG_DEBUG("Nonce Size: " << nonceSize);

      if (fullEncryptedData.size() < nonceSize) {
        LOG_DEBUG_ERROR("Encrypted data size is less than nonce size for file: " << fileRow.path << ". Skipping.");
        continue;
      }

      std::vector<unsigned char> nonce(fullEncryptedData.begin() + fullEncryptedData.size() - nonceSize, fullEncryptedData.end());
      std::vector<unsigned char> cipherBytes(fullEncryptedData.begin(), fullEncryptedData.end() - nonceSize);
      std::vector<unsigned char> decryptedData;

      LOG_DEBUG("Cipher Bytes Size: " << cipherBytes.size());

      bool decryptSuccess = encryption.decryptBuffer(decryptedData, key.data(), nonce, uname, projectName, cipherBytes);
      LOG_DEBUG("Decrypt Buffer Success: " << (decryptSuccess ? "true" : "false"));
      LOG_DEBUG("Decrypted Data Size: " << decryptedData.size());

      if (!decryptSuccess) {
        LOG_DEBUG_ERROR("Failed to decrypt file: " << fileRow.path << ". Skipping.");
        continue;
      }
      
      std::string fullPath = projDir + "/" + fileRow.path; // Construct full path for writing
      
      // Ensure parent directories exist for the file
      std::filesystem::path parentPath = std::filesystem::path(fullPath).parent_path();
      if (!parentPath.empty() && !std::filesystem::exists(parentPath)) {
          std::filesystem::create_directories(parentPath);
      }

      if (!AgniVault::File::writeFile(fullPath, decryptedData)) {
        LOG_DEBUG_ERROR("Failed to write decrypted data to file: " << fullPath << ". Skipping.");
        continue;
      }
    }


    LOG_INFO(projectName << " Successfully Mounted at " << projDir
                         << "\n\t RUN: cd " << projDir);
    std::filesystem::current_path(projDir);
  } catch (const std::exception &e) {
    LOG_DEBUG_ERROR("Error mounting the project: " << e.what());
  }

  sqlite3_close(db);
}
