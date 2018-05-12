cmake_minimum_required(VERSION 3.10)
project(cpp_lab2)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

#add_library(cpp_lab2 SHARED vector.cpp include/vector.h)
add_executable(vector_testing ${SOURCE_FILES} include/vector.hpp tests/vector_testing.cpp
        tests/gtest/gtest_main.cc tests/gtest/gtest-all.cc tests/gtest/gtest.h)

target_link_libraries(vector_testing -Iinclude -lpthread)