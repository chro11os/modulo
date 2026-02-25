#include "utils/create/createFile.h"
#include "utils/read/readFile.h"
#include "utils/write/writeFile.h"
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