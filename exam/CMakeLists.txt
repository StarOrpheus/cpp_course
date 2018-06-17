cmake_minimum_required(VERSION 3.8)
project(cpp_exam)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11 -funsigned-char")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -D NDEBUG")

add_subdirectory(libs/gtest)
include_directories(libs/gtest)

add_library(myset include/set.h impl/set_impl.h)
include_directories(include)
include_directories(impl)
set_target_properties(myset PROPERTIES LINKER_LANGUAGE CXX)

add_executable(myset_test test/myset_test.cpp)
target_link_libraries(myset_test myset gtest)