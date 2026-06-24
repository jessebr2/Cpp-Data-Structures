cmake_minimum_required(VERSION 3.20)

project(CppDataStructures)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For single-config generators (e.g., Makefiles/Ninja), default to Release.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

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
 "include/HashTable/HashTableQuadraticProbing.h" "include/UnionFind.h" "include/LowLatency/LockFreeSPSCQueue.h" "include/LowLatency/LLConstants.h" )

target_include_directories(
    CppDataStructures
    PRIVATE
    include
)

target_compile_options(
    CppDataStructures
    PRIVATE
    $<$<CONFIG:Release>:-O3>
)