
#include "helper/file_helpers.hpp"
#include "helper/resolve_path.hpp"
#include "logger.hpp"
#include <vector>

namespace AgniVault {
namespace File {

size_t getFileSize(std::ifstream &fin) {

  fin.seekg(0, std::ios::end);
  size_t fileSize = fin.tellg();
  fin.seekg(0, std::ios::beg);

  return fileSize;
}

bool readFile(std::string filePath, std::vector<unsigned char> &fileContent) {

  if (filePath.empty()) {
    LOG_DEBUG_ERROR("File Path Can't be Empty");
    return false;
  }
  std::ifstream fin(filePath, std::ios::binary);
  if (!fin) {
    LOG_DEBUG_ERROR("File Can't be opened");
    return false;
  }
  size_t fileSize = AgniVault::File::getFileSize(fin);
  fileContent.resize(fileSize);
  fin.read(reinterpret_cast<char *>(fileContent.data()), fileContent.size());

  if (fin.bad()) {
    LOG_DEBUG_ERROR("Stream Got courrpted I/O Error");
    return false;
  } else if (fin.fail()) {
    LOG_DEBUG_ERROR("General Failure, Read Error");
    return false;
  }

  return true;
}

bool writeFile(const std::string filePath,
               const std::vector<unsigned char> &content) {

  std::ofstream fout(AgniVault::resolve_path(filePath), std::ios::binary);
  if (!fout) {
    LOG_DEBUG_ERROR("File can't be found");
    return false;
  }
  fout.write(reinterpret_cast<const char *>(content.data()), content.size());

  if (fout.bad()) {
    LOG_DEBUG_ERROR("Stream Got courrpted, I/O Error");
    return false;
  } else if (fout.fail()) {
    LOG_DEBUG_ERROR("General Failure, Write Error");
    return false;
  }

  return true;
}
} // namespace File
} // namespace AgniVault
