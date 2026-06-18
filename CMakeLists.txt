cmake_minimum_required(VERSION 3.20)

project(CppDataStructures)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    CppDataStructures
    examples/main.cpp
    "include/DynamicArray.h"
    "include/LinkedList.h"
    "include/ListStack.h"
    "include/ArrayStack.h"
    "include/ArrayQueue.h"
    "include/ListQueue.h"
    "include/PriorityQueue.h"
    "include/BinarySearchTree.h"
    "include/HashTable/SeparateChainingHashTable.h"
 "include/HashTable/HashTableQuadraticProbing.h")

target_include_directories(
    CppDataStructures
    PRIVATE
    include
)