# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/joagz/Desktop/cpp_algorithms/source

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/joagz/Desktop/cpp_algorithms

# Include any dependencies generated for this target.
include CMakeFiles/A_star.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/A_star.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/A_star.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/A_star.dir/flags.make

CMakeFiles/A_star.dir/main.cc.o: CMakeFiles/A_star.dir/flags.make
CMakeFiles/A_star.dir/main.cc.o: /home/joagz/Desktop/cpp_algorithms/source/main.cc
CMakeFiles/A_star.dir/main.cc.o: CMakeFiles/A_star.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/joagz/Desktop/cpp_algorithms/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/A_star.dir/main.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/A_star.dir/main.cc.o -MF CMakeFiles/A_star.dir/main.cc.o.d -o CMakeFiles/A_star.dir/main.cc.o -c /home/joagz/Desktop/cpp_algorithms/source/main.cc

CMakeFiles/A_star.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/A_star.dir/main.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/joagz/Desktop/cpp_algorithms/source/main.cc > CMakeFiles/A_star.dir/main.cc.i

CMakeFiles/A_star.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/A_star.dir/main.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/joagz/Desktop/cpp_algorithms/source/main.cc -o CMakeFiles/A_star.dir/main.cc.s

# Object files for target A_star
A_star_OBJECTS = \
"CMakeFiles/A_star.dir/main.cc.o"

# External object files for target A_star
A_star_EXTERNAL_OBJECTS =

A_star: CMakeFiles/A_star.dir/main.cc.o
A_star: CMakeFiles/A_star.dir/build.make
A_star: pathfinder/libpathfinder.a
A_star: CMakeFiles/A_star.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/joagz/Desktop/cpp_algorithms/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable A_star"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/A_star.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/A_star.dir/build: A_star
.PHONY : CMakeFiles/A_star.dir/build

CMakeFiles/A_star.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/A_star.dir/cmake_clean.cmake
.PHONY : CMakeFiles/A_star.dir/clean

CMakeFiles/A_star.dir/depend:
	cd /home/joagz/Desktop/cpp_algorithms && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/joagz/Desktop/cpp_algorithms/source /home/joagz/Desktop/cpp_algorithms/source /home/joagz/Desktop/cpp_algorithms /home/joagz/Desktop/cpp_algorithms /home/joagz/Desktop/cpp_algorithms/CMakeFiles/A_star.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/A_star.dir/depend

