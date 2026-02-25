#include "writeFile.h"
#include <fstream>
#include <iostream>

int writeFile(std::string fileName) {
  std::ofstream outFile(fileName, std::ios::app);

  if (!outFile.is_open()) {
    std::cerr << "Error opening file for writing: " << fileName << std::endl;
    return 1;
  }

  std::string inputString;
  std::cout << "Enter text to add to " << fileName << ": ";
  std::cin.ignore();
  std::getline(std::cin, inputString);

  outFile << inputString << std::endl;
  outFile.close();
  return 0;
}