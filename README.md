# C++ Data Structures

A personal study project focused on implementing classic data structures and algorithms from scratch using modern C++.

## Goals

- Deep understanding of Data Structures & Algorithms
- Modern C++ (C++20)
- Memory management
- Move semantics
- Performance-oriented programming
- Preparation for low-latency and trading systems development

## Implemented

- [x] Dynamic Array
- [x] Linked List
- [x] Stack
- [x] Queue
- [x] Priority Queue
- [x] Binary Search Tree
- [x] Hash Table

## Low Latency
- [x] Lock Free Single Producer Single Consumer Queue

## Build (Linux)

Recommended (CMake):

```bash
cmake -S . -B build
cmake --build build -j
```

Run:

```bash
./build/CppDataStructures
```

Manual compile (g++):

```bash
g++ -std=c++20 -O3 -Iinclude -pthread examples/main.cpp -o DSTest
```

Run:

```bash
./DSTest
```
