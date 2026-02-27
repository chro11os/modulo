#include "writeFile.h"
#include <fstream>
#include <iostream>

int writeFile(std::string fileName, std::string content) {
  std::ofstream outFile(fileName, std::ios::app);

  if (!outFile.is_open()) {
    return 1;
  }

  outFile << content << std::endl;
  outFile.close();
  return 0;
}