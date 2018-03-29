cmake_minimum_required(VERSION 3.6)
project(big_integer)

SET (CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Wcast-align -Wunused -Woverloaded-virtual -Wsign-conversion -Wnull-dereference -pedantic -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -O2")

add_executable(big_integer_testing ${SOURCE_FILES} big_integer.cpp big_integer.h big_integer_testing.cpp
        gtest/gtest-all.cc gtest/gtest_main.cc gtest/gtest.h)

target_link_libraries(big_integer_testing -lgmp -lgmpxx -lpthread)