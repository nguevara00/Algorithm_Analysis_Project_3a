# Project 03a: 2-3 Tree Index
Nick Guevara
William Dappen

CS 415 - Gill

4/22/2026

Goals of the project

    Implement 2-3 Tree to build an index from a text document. 
    Compare the complexity of searching for a string using a 2-3 tree with that of a BST tree.

## Program Instructions ##



```bash
make
./index <input_file>
```



## cmakelists.txt ##

cmake_minimum_required(VERSION 4.0)
project(Algorithm_Analysis_Project_3a)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Algorithm_Analysis_Project_3a
bst.cpp
bst.h
main.cpp
TwoThreeTree.cpp
TwoThreeTree.h)
