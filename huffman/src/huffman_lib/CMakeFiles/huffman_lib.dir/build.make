# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/starorpheus/Devel/cpp_course/huffman

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/starorpheus/Devel/cpp_course/huffman

# Include any dependencies generated for this target.
include src/huffman_lib/CMakeFiles/huffman_lib.dir/depend.make

# Include the progress variables for this target.
include src/huffman_lib/CMakeFiles/huffman_lib.dir/progress.make

# Include the compile flags for this target's objects.
include src/huffman_lib/CMakeFiles/huffman_lib.dir/flags.make

src/huffman_lib/CMakeFiles/huffman_lib.dir/decrypter.cpp.o: src/huffman_lib/CMakeFiles/huffman_lib.dir/flags.make
src/huffman_lib/CMakeFiles/huffman_lib.dir/decrypter.cpp.o: src/huffman_lib/decrypter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/starorpheus/Devel/cpp_course/huffman/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/huffman_lib/CMakeFiles/huffman_lib.dir/decrypter.cpp.o"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/huffman_lib.dir/decrypter.cpp.o -c /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/decrypter.cpp

src/huffman_lib/CMakeFiles/huffman_lib.dir/decrypter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/huffman_lib.dir/decrypter.cpp.i"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/decrypter.cpp > CMakeFiles/huffman_lib.dir/decrypter.cpp.i

src/huffman_lib/CMakeFiles/huffman_lib.dir/decrypter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/huffman_lib.dir/decrypter.cpp.s"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/decrypter.cpp -o CMakeFiles/huffman_lib.dir/decrypter.cpp.s

src/huffman_lib/CMakeFiles/huffman_lib.dir/encrypter.cpp.o: src/huffman_lib/CMakeFiles/huffman_lib.dir/flags.make
src/huffman_lib/CMakeFiles/huffman_lib.dir/encrypter.cpp.o: src/huffman_lib/encrypter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/starorpheus/Devel/cpp_course/huffman/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/huffman_lib/CMakeFiles/huffman_lib.dir/encrypter.cpp.o"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/huffman_lib.dir/encrypter.cpp.o -c /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/encrypter.cpp

src/huffman_lib/CMakeFiles/huffman_lib.dir/encrypter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/huffman_lib.dir/encrypter.cpp.i"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/encrypter.cpp > CMakeFiles/huffman_lib.dir/encrypter.cpp.i

src/huffman_lib/CMakeFiles/huffman_lib.dir/encrypter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/huffman_lib.dir/encrypter.cpp.s"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/encrypter.cpp -o CMakeFiles/huffman_lib.dir/encrypter.cpp.s

src/huffman_lib/CMakeFiles/huffman_lib.dir/h_tree.cpp.o: src/huffman_lib/CMakeFiles/huffman_lib.dir/flags.make
src/huffman_lib/CMakeFiles/huffman_lib.dir/h_tree.cpp.o: src/huffman_lib/h_tree.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/starorpheus/Devel/cpp_course/huffman/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/huffman_lib/CMakeFiles/huffman_lib.dir/h_tree.cpp.o"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/huffman_lib.dir/h_tree.cpp.o -c /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/h_tree.cpp

src/huffman_lib/CMakeFiles/huffman_lib.dir/h_tree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/huffman_lib.dir/h_tree.cpp.i"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/h_tree.cpp > CMakeFiles/huffman_lib.dir/h_tree.cpp.i

src/huffman_lib/CMakeFiles/huffman_lib.dir/h_tree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/huffman_lib.dir/h_tree.cpp.s"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/h_tree.cpp -o CMakeFiles/huffman_lib.dir/h_tree.cpp.s

src/huffman_lib/CMakeFiles/huffman_lib.dir/h_tree_DFA.cpp.o: src/huffman_lib/CMakeFiles/huffman_lib.dir/flags.make
src/huffman_lib/CMakeFiles/huffman_lib.dir/h_tree_DFA.cpp.o: src/huffman_lib/h_tree_DFA.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/starorpheus/Devel/cpp_course/huffman/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/huffman_lib/CMakeFiles/huffman_lib.dir/h_tree_DFA.cpp.o"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/huffman_lib.dir/h_tree_DFA.cpp.o -c /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/h_tree_DFA.cpp

src/huffman_lib/CMakeFiles/huffman_lib.dir/h_tree_DFA.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/huffman_lib.dir/h_tree_DFA.cpp.i"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/h_tree_DFA.cpp > CMakeFiles/huffman_lib.dir/h_tree_DFA.cpp.i

src/huffman_lib/CMakeFiles/huffman_lib.dir/h_tree_DFA.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/huffman_lib.dir/h_tree_DFA.cpp.s"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/h_tree_DFA.cpp -o CMakeFiles/huffman_lib.dir/h_tree_DFA.cpp.s

src/huffman_lib/CMakeFiles/huffman_lib.dir/utils.cpp.o: src/huffman_lib/CMakeFiles/huffman_lib.dir/flags.make
src/huffman_lib/CMakeFiles/huffman_lib.dir/utils.cpp.o: src/huffman_lib/utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/starorpheus/Devel/cpp_course/huffman/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/huffman_lib/CMakeFiles/huffman_lib.dir/utils.cpp.o"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/huffman_lib.dir/utils.cpp.o -c /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/utils.cpp

src/huffman_lib/CMakeFiles/huffman_lib.dir/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/huffman_lib.dir/utils.cpp.i"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/utils.cpp > CMakeFiles/huffman_lib.dir/utils.cpp.i

src/huffman_lib/CMakeFiles/huffman_lib.dir/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/huffman_lib.dir/utils.cpp.s"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/utils.cpp -o CMakeFiles/huffman_lib.dir/utils.cpp.s

src/huffman_lib/CMakeFiles/huffman_lib.dir/dynamic_bitset.cpp.o: src/huffman_lib/CMakeFiles/huffman_lib.dir/flags.make
src/huffman_lib/CMakeFiles/huffman_lib.dir/dynamic_bitset.cpp.o: src/huffman_lib/dynamic_bitset.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/starorpheus/Devel/cpp_course/huffman/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/huffman_lib/CMakeFiles/huffman_lib.dir/dynamic_bitset.cpp.o"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/huffman_lib.dir/dynamic_bitset.cpp.o -c /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/dynamic_bitset.cpp

src/huffman_lib/CMakeFiles/huffman_lib.dir/dynamic_bitset.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/huffman_lib.dir/dynamic_bitset.cpp.i"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/dynamic_bitset.cpp > CMakeFiles/huffman_lib.dir/dynamic_bitset.cpp.i

src/huffman_lib/CMakeFiles/huffman_lib.dir/dynamic_bitset.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/huffman_lib.dir/dynamic_bitset.cpp.s"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/dynamic_bitset.cpp -o CMakeFiles/huffman_lib.dir/dynamic_bitset.cpp.s

# Object files for target huffman_lib
huffman_lib_OBJECTS = \
"CMakeFiles/huffman_lib.dir/decrypter.cpp.o" \
"CMakeFiles/huffman_lib.dir/encrypter.cpp.o" \
"CMakeFiles/huffman_lib.dir/h_tree.cpp.o" \
"CMakeFiles/huffman_lib.dir/h_tree_DFA.cpp.o" \
"CMakeFiles/huffman_lib.dir/utils.cpp.o" \
"CMakeFiles/huffman_lib.dir/dynamic_bitset.cpp.o"

# External object files for target huffman_lib
huffman_lib_EXTERNAL_OBJECTS =

src/huffman_lib/libhuffman_lib.a: src/huffman_lib/CMakeFiles/huffman_lib.dir/decrypter.cpp.o
src/huffman_lib/libhuffman_lib.a: src/huffman_lib/CMakeFiles/huffman_lib.dir/encrypter.cpp.o
src/huffman_lib/libhuffman_lib.a: src/huffman_lib/CMakeFiles/huffman_lib.dir/h_tree.cpp.o
src/huffman_lib/libhuffman_lib.a: src/huffman_lib/CMakeFiles/huffman_lib.dir/h_tree_DFA.cpp.o
src/huffman_lib/libhuffman_lib.a: src/huffman_lib/CMakeFiles/huffman_lib.dir/utils.cpp.o
src/huffman_lib/libhuffman_lib.a: src/huffman_lib/CMakeFiles/huffman_lib.dir/dynamic_bitset.cpp.o
src/huffman_lib/libhuffman_lib.a: src/huffman_lib/CMakeFiles/huffman_lib.dir/build.make
src/huffman_lib/libhuffman_lib.a: src/huffman_lib/CMakeFiles/huffman_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/starorpheus/Devel/cpp_course/huffman/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX static library libhuffman_lib.a"
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && $(CMAKE_COMMAND) -P CMakeFiles/huffman_lib.dir/cmake_clean_target.cmake
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/huffman_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/huffman_lib/CMakeFiles/huffman_lib.dir/build: src/huffman_lib/libhuffman_lib.a

.PHONY : src/huffman_lib/CMakeFiles/huffman_lib.dir/build

src/huffman_lib/CMakeFiles/huffman_lib.dir/clean:
	cd /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib && $(CMAKE_COMMAND) -P CMakeFiles/huffman_lib.dir/cmake_clean.cmake
.PHONY : src/huffman_lib/CMakeFiles/huffman_lib.dir/clean

src/huffman_lib/CMakeFiles/huffman_lib.dir/depend:
	cd /home/starorpheus/Devel/cpp_course/huffman && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/starorpheus/Devel/cpp_course/huffman /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib /home/starorpheus/Devel/cpp_course/huffman /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib /home/starorpheus/Devel/cpp_course/huffman/src/huffman_lib/CMakeFiles/huffman_lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/huffman_lib/CMakeFiles/huffman_lib.dir/depend
