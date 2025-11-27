#pragma once
#include <string>

namespace AgniVault::Project {

struct ProjectRow {
  std::string id;
  std::string name;
  std::string created_at;
  std::string updated_at;
};

} // namespace AgniVault::Project
