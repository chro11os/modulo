#include "readFile.h"
#include <fstream>
#include <iostream>

std::string readFile(std::string fileName) {
  std::ifstream fileIn(fileName);
  if (!fileIn.is_open()) {
    return "ERROR: Could not open file.";
  }

  std::string content;
  std::string line;
  while (std::getline(fileIn, line)) {
    content += line + "\n";
  }

  fileIn.close();
  return content;
}