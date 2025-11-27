#pragma once

#include <optional>
#include <string>
namespace AgniVault {
namespace Project {

class Project {
private:
  const std::string PROJECT_BASE_DIR_TO_MOUNT = "/dev/shm/agnivault";

public:
  void init_project(std::string name);
  void update_project(std::string oldName, std::string newName);
  void list_project(std::optional<std::string> projectName = std::nullopt);
  void mount_project(std::string projectName);
  void umount_project(std::optional<std::string> projectName = std::nullopt);
};

} // namespace Project
} // namespace AgniVault
