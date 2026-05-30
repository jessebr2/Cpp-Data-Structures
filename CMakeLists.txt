
cmake_minimum_required(VERSION 3.20)

project(CppDataStructures)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    CppDataStructures
    src/main.cpp
)

target_include_directories(
    CppDataStructures
    PRIVATE
    include
)