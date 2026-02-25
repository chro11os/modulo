#include "utils/createFile.h"
#include "utils/readFile.h"
#include "utils/writeFile.h"
#include <iostream>
#include <string>

int main() {
  std::string activeFile = createFile();

  if (!activeFile.empty()) {
    writeFile(activeFile);
    readFile(activeFile);
  } else {
    std::cerr << "Exiting: File creation was unsuccessful." << std::endl;
    return 1;
  }

  return 0;
}