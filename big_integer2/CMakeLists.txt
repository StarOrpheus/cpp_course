cmake_minimum_required(VERSION 3.6)
project(cpp_hw)

SET (CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")


#add_executable(big_integer ${SOURCE_FILES} big_integer.cpp big_integer.h)

add_executable(big_integer_testing ${SOURCE_FILES} big_integer.cpp big_integer.h tests/big_integer_testing.cpp
        tests/gtest/gtest-all.cc tests/gtest/gtest_main.cc tests/gtest/gtest.h
        big_int_storage.cpp big_int_storage.h)

target_link_libraries(big_integer_testing -lgmp -lgmpxx -lpthread)