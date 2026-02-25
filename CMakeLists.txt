cmake_minimum_required(VERSION 4.1)
project(modulo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(modulo main.cpp
        utils/create/createFile.cpp
        utils/create/createFile.h
        utils/read/readFile.cpp
        utils/read/readFile.h
        utils/write/writeFile.cpp
        utils/write/writeFile.h
)
