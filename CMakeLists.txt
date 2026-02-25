cmake_minimum_required(VERSION 3.14)
project(modulo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)

FetchContent_MakeAvailable(ftxui)

add_executable(modulo main.cpp
        utils/create/createFile.cpp
        utils/create/createFile.h
        utils/read/readFile.cpp
        utils/read/readFile.h
        utils/write/writeFile.cpp
        utils/write/writeFile.h
        process/read_drive.cpp
        process/read_drive.h
        ui/terminal_ui.cpp
        ui/terminal_ui.h
)

target_link_libraries(modulo PRIVATE ftxui::screen ftxui::dom ftxui::component)
