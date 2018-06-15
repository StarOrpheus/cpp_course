cmake_minimum_required(VERSION 3.10)
project(huffman_lib)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -pedantic -std=c++11 -funsigned-char")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_library(huffman_lib include/decrypter.h decrypter.cpp
        include/encrypter.h encrypter.cpp
        include/h_tree.h h_tree.cpp
        include/h_tree_DFA.h h_tree_DFA.cpp
        include/utils.h utils.cpp
        include/dynamic_bitset.h dynamic_bitset.cpp)