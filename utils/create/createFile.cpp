#include "createFile.h"
#include <filesystem>
#include <fstream>
#include <iostream>

std::string createFile(std::string fileName) {
  if (fileName.empty())
    return "";

  std::filesystem::path folderPath = "../output_data/";
  if (!std::filesystem::exists(folderPath)) {
    std::filesystem::create_directories(folderPath);
  }

  std::filesystem::path fullPath = folderPath / (fileName + ".txt");
  std::ofstream newFile(fullPath);

  if (newFile) {
    newFile << "File created on: 2/24/26\n";
    newFile.close();
    return fullPath.string();
  } else {
    return "";
  }
}