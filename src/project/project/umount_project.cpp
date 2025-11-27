#include "file/database.hpp"
#include "file_handler.hpp"
#include "helper/config.hpp"
#include "helper/file_helpers.hpp"
#include "logger.hpp"
#include "project.hpp"
#include "project/database.hpp"
#include <exception>
#include <filesystem>
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

  try {

    // List all avialable files in the proj dir

    // Encrypting All File in the disk
    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(dirToDelete)) {
      if (entry.is_regular_file()) {
        AgniVault::Crypto::FileHandler fileHandler;
        std::vector<unsigned char> key;
        if (!fetchKeyFromConfig(key)) {
          LOG_DEBUG_WARN("Key Not found in config; To generate new key, RUN: "
                         "agniv gen-key");
          return;
        }
        std::string name;
        std::string email;
        if (!getUsernameFromConfig(name) || !getEmailFromConfig(email)) {
          printConfigMsg();
          return;
        }
        std::string uname = name + email;
        // Encrypting file first
        fileHandler.encryptFile(entry.path().string(), key.data(), uname,
                                projectName.value());

        sqlite3 *db = nullptr;

        AgniVault::Project::Database projDatabase(db);
        std::string projectID;

        // getting project id
        if (!projDatabase.getProjectIdViaProjectName(projectName.value(),
                                                     projectID)) {
          return;
        }

        // Reading encrypted data from the file
        std::vector<unsigned char> encryptedData;

        File::readFile(entry.path().string(), encryptedData);

        // Inserting in database
        AgniVault::File::Database database(db);
        database.insertFile(std::stoi(projectID), entry.path().string(),
                            encryptedData);
      }
    }

    std::filesystem::remove_all(dirToDelete);
    LOG_INFO("Unmount Successfull");
  } catch (const std::exception &e) {
    LOG_ERROR("Unmount Unsuccessfull");
    LOG_DEBUG_ERROR("Umount Error: " << e.what());
  }
}
