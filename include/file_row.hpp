#pragma once

#include <string>
#include <vector>

namespace AgniVault {

struct FileRow {
  std::string id;
  std::string project_id;
  std::string path;
  std::vector<unsigned char> encrypted_data;
  std::string last_modified;
};

} // namespace AgniVault
