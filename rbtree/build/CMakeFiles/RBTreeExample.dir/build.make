# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/andrew/Private/sems/sweet_boy/rbtree

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/andrew/Private/sems/sweet_boy/rbtree/build

# Include any dependencies generated for this target.
include CMakeFiles/RBTreeExample.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/RBTreeExample.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/RBTreeExample.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RBTreeExample.dir/flags.make

CMakeFiles/RBTreeExample.dir/source/main.cpp.o: CMakeFiles/RBTreeExample.dir/flags.make
CMakeFiles/RBTreeExample.dir/source/main.cpp.o: ../source/main.cpp
CMakeFiles/RBTreeExample.dir/source/main.cpp.o: CMakeFiles/RBTreeExample.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrew/Private/sems/sweet_boy/rbtree/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/RBTreeExample.dir/source/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RBTreeExample.dir/source/main.cpp.o -MF CMakeFiles/RBTreeExample.dir/source/main.cpp.o.d -o CMakeFiles/RBTreeExample.dir/source/main.cpp.o -c /home/andrew/Private/sems/sweet_boy/rbtree/source/main.cpp

CMakeFiles/RBTreeExample.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RBTreeExample.dir/source/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andrew/Private/sems/sweet_boy/rbtree/source/main.cpp > CMakeFiles/RBTreeExample.dir/source/main.cpp.i

CMakeFiles/RBTreeExample.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RBTreeExample.dir/source/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andrew/Private/sems/sweet_boy/rbtree/source/main.cpp -o CMakeFiles/RBTreeExample.dir/source/main.cpp.s

# Object files for target RBTreeExample
RBTreeExample_OBJECTS = \
"CMakeFiles/RBTreeExample.dir/source/main.cpp.o"

# External object files for target RBTreeExample
RBTreeExample_EXTERNAL_OBJECTS =

RBTreeExample: CMakeFiles/RBTreeExample.dir/source/main.cpp.o
RBTreeExample: CMakeFiles/RBTreeExample.dir/build.make
RBTreeExample: libTree_lib.a
RBTreeExample: CMakeFiles/RBTreeExample.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/andrew/Private/sems/sweet_boy/rbtree/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable RBTreeExample"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RBTreeExample.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RBTreeExample.dir/build: RBTreeExample
.PHONY : CMakeFiles/RBTreeExample.dir/build

CMakeFiles/RBTreeExample.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RBTreeExample.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RBTreeExample.dir/clean

CMakeFiles/RBTreeExample.dir/depend:
	cd /home/andrew/Private/sems/sweet_boy/rbtree/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andrew/Private/sems/sweet_boy/rbtree /home/andrew/Private/sems/sweet_boy/rbtree /home/andrew/Private/sems/sweet_boy/rbtree/build /home/andrew/Private/sems/sweet_boy/rbtree/build /home/andrew/Private/sems/sweet_boy/rbtree/build/CMakeFiles/RBTreeExample.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RBTreeExample.dir/depend

