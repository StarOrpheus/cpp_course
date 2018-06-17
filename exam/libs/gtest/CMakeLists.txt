cmake_minimum_required(VERSION 3.8)
project(huffman)

set(CMAKE_CXX_STANDARD 11)

add_library(gtest gtest.h gtest-all.cc gtest_main.cc)

target_link_libraries(gtest -lpthread)