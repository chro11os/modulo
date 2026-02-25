#include "readFile.h"
#include <fstream>
#include <iostream>

int readFile(std::string fileName) {
  std::ifstream fileIn(fileName);
  if (!fileIn.is_open()) {
    std::cerr << "Error opening file for reading: " << fileName << std::endl;
    return 1;
  }

  std::string line;
  std::cout << "\n--- Reading File Contents ---" << std::endl;
  while (std::getline(fileIn, line)) {
    std::cout << line << std::endl;
  }
  std::cout << "-----------------------------\n" << std::endl;

  fileIn.close();
  return 0;
}