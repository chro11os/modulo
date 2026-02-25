#include "createFile.h"
#include <filesystem>
#include <fstream>
#include <iostream>


std::string createFile() {
  namespace fs = std::filesystem;
  std::string fileName;
  std::cout << "Enter file name: ";
  std::cin >> fileName;

  fs::path folderPath = "../output_data/";
  if (!fs::exists(folderPath)) {
    fs::create_directories(folderPath);
  }

  fs::path fullPath = folderPath / (fileName + ".txt");
  std::ofstream newFile(fullPath);

  if (newFile) {
    newFile << "File created on: 2/24/26\n";
    std::cout << "Success! Path: " << fullPath << std::endl;
    newFile.close();
    return fullPath.string();
  } else {
    std::cerr << "Failed to create file at: " << fullPath << std::endl;
    return "";
  }
}