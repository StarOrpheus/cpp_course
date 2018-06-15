cmake_minimum_required(VERSION 3.10)
project(huffman)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -pedantic -std=c++11 -funsigned-char")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories({$CMAKE_SOURCE_DIR}/src/lib/include)
include_directories({$CMAKE_SOURCE_DIR}/libs/gtest)

add_subdirectory(src/huffman_lib)
add_subdirectory(libs/gtest)

add_subdirectory(src/cli)

add_executable(huffman_testing src/huffman_lib/tests/h_lib_testing.cpp)

target_link_libraries(huffman_testing gtest_lib huffman_lib -lpthread)