
cmake_minimum_required(VERSION 3.20)

project(CppDataStructures)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    CppDataStructures
    src/main.cpp
 "include/ArrayStack.h" "include/ListQueue.h" "include/ArrayQueue.h" "include/PriorityQueue.h" "include/BinarySearchTree.h")

target_include_directories(
    CppDataStructures
    PRIVATE
    include
)