cmake_minimum_required(VERSION 3.8)
project(huffman_lib)

set(CMAKE_CXX_STANDARD 11)

add_library(huffman_lib include/decrypter.h decrypter.cpp
        include/encrypter.h encrypter.cpp
        include/h_tree.h h_tree.cpp
        include/h_tree_dfa.h h_tree_dfa.cpp
        include/utils.h
        include/dynamic_bitset.h dynamic_bitset.cpp
        include/frequency_counter.h frequency_counter.cpp)