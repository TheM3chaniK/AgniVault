#pragma once

#include <string>
#include <vector>
namespace AgniVault {
namespace DB {

class Project {
private:
public:
  void init_project(std::string name, std::vector<unsigned char> &key);
  void update_project(std::string oldName, std::string newName);
};

} // namespace DB
} // namespace AgniVault
