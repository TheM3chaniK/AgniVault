#include "file/database.hpp"
#include "file_handler.hpp"
#include "helper/config.hpp"
#include "helper/file_helpers.hpp"
#include "helper.hpp"
#include "logger.hpp"
#include "project.hpp"
#include "project/database.hpp"
#include <exception>
#include <filesystem>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace AgniVault::Project;

// Umount Project Currently Works only for single umount project not all project
// umount
void Project::umount_project(std::optional<std::string> projectName) {

  std::string dirToDelete = PROJECT_BASE_DIR_TO_MOUNT;
  if (projectName) {
    std::string projDir = PROJECT_BASE_DIR_TO_MOUNT + "/" + projectName.value();
    dirToDelete = projDir;
  }

  sqlite3 *db = nullptr;
  std::string dbPath = AgniVault::getDBPath();
  int rc = sqlite3_open(dbPath.c_str(), &db);

  if (rc != SQLITE_OK) {
    LOG_DEBUG_ERROR("Can't open database: " << sqlite3_errmsg(db));
    sqlite3_close(db);
    return;
  }

  AgniVault::Project::Database projDatabase(db);
  AgniVault::File::Database fileDatabase(db);
  std::string projectID_str;

  if (!projectName ||
      !projDatabase.getProjectIdViaProjectName(projectName.value(), projectID_str)) {
    LOG_DEBUG_ERROR("Project ID not found for umount operation.");
    sqlite3_close(db);
    return;
  }
  int projectID = std::stoi(projectID_str);

  try {
    // 1. Retrieve existing files from database
    std::vector<AgniVault::FileRow> existingDbFilesVector;
    if (!fileDatabase.selectFilesByProjectId(projectID, existingDbFilesVector)) {
        LOG_DEBUG_ERROR("Failed to retrieve existing files from database for project " << projectName.value());
        sqlite3_close(db);
        return;
    }

    std::map<std::string, AgniVault::FileRow> existingDbFilesMap;
    for (const auto& fileRow : existingDbFilesVector) {
        existingDbFilesMap[fileRow.path] = fileRow;
    }

    // 2. Process files on disk (new/modified)
    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(dirToDelete)) {
      if (entry.is_regular_file()) {
        AgniVault::Crypto::FileHandler fileHandler;
        std::vector<unsigned char> key;
        if (!fetchKeyFromConfig(key)) {
          LOG_DEBUG_WARN("Key Not found in config; To generate new key, RUN: "
                         "agniv gen-key");
          sqlite3_close(db);
          return;
        }
        std::string name;
        std::string email;
        if (!getUsernameFromConfig(name) || !getEmailFromConfig(email)) {
          printConfigMsg();
          sqlite3_close(db);
          return;
        }
        std::string uname = name + email;
        
        // Encrypting file
        if (!fileHandler.encryptFile(entry.path().string(), key.data(), uname,
                                projectName.value())) {
          LOG_DEBUG_ERROR("Failed to encrypt file: " << entry.path().string() << ". Skipping.");
          continue;
        }

        // Reading encrypted data from the file
        std::vector<unsigned char> encryptedData;
        File::readFile(entry.path().string(), encryptedData);

        // Calculate relative path
        std::filesystem::path fullFilePath = entry.path();
        std::filesystem::path basePath = dirToDelete;
        
        std::string relativePath = std::filesystem::relative(fullFilePath, basePath).string();

        if (existingDbFilesMap.count(relativePath)) {
            // File exists in DB, so update it
            if (!fileDatabase.updateFile(projectID, relativePath, encryptedData)) {
                LOG_DEBUG_ERROR("Failed to update file in DB: " << relativePath);
            }
            existingDbFilesMap.erase(relativePath); // Mark as processed
        } else {
            // New file, insert it
            if (!fileDatabase.insertFile(projectID, relativePath, encryptedData)) {
                LOG_DEBUG_ERROR("Failed to insert new file in DB: " << relativePath);
            }
        }
      }
    }

    // 3. Delete files from database (no longer on disk)
    for (const auto& pair : existingDbFilesMap) {
        const std::string& relativePathToDelete = pair.first;
        if (!fileDatabase.deleteFile(projectID, relativePathToDelete)) {
            LOG_DEBUG_ERROR("Failed to delete file from DB: " << relativePathToDelete);
        } else {
            LOG_INFO("Deleted file from DB: " << relativePathToDelete);
        }
    }

    std::filesystem::remove_all(dirToDelete);
    LOG_INFO("Unmount Successfull");
  } catch (const std::exception &e) {
    LOG_ERROR("Unmount Unsuccessfull");
    LOG_DEBUG_ERROR("Umount Error: " << e.what());
  }

  sqlite3_close(db);
}
