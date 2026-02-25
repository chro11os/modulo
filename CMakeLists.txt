cmake_minimum_required(VERSION 4.1)
project(modulo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(modulo main.cpp
        utils/writeFile.cpp
        utils/writeFile.h
        utils/readFile.cpp
        utils/readFile.h
        utils/createFile.cpp
        utils/createFile.h
)
