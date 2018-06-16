cmake_minimum_required(VERSION 3.10)
project(huffman)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -pedantic -std=c++11 -funsigned-char")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D __ENCRYPTER_EXEC=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/encrypter")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D __DECRYPTER_EXEC=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/decrypter")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -D __ENCRYPTER_EXEC=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/encrypter -D __DECRYPTER_EXEC=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/decrypter")

include_directories(include)

add_executable(encrypter encrypter.cpp utils.cpp)
add_executable(decrypter decrypter.cpp utils.cpp)

add_executable(huffman_cli_test tests/huffman_cli_tests.cpp utils.cpp)

target_link_libraries(encrypter huffman_lib)
target_link_libraries(decrypter huffman_lib)
target_link_libraries(huffman_cli_test huffman_lib gtest_lib -lpthread)
