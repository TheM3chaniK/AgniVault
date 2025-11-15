#pragma once
#include <cstddef>
#include <fstream>
#include <vector>

namespace AgniVault {
namespace File {

bool readFile(std::string filePath, std::vector<unsigned char> &fileContent);
bool writeFile(const std::string filePath,
               const std::vector<unsigned char> &content);
size_t getFileSize(std::ifstream &fin);
} // namespace File
} // namespace AgniVault
