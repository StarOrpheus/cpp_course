cmake_minimum_required(VERSION 3.2)
project(big_integer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Wcast-align -Wunused -Woverloaded-virtual -Wsign-conversion -pedantic -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

add_executable(big_integer_testing ${SOURCE_FILES} big_integer.cpp big_integer.h big_integer_testing.cpp
        gtest/gtest-all.cc gtest/gtest_main.cc gtest/gtest.h)

target_link_libraries(big_integer_testing -lpthread)
