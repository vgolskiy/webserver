# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/cwindom/Desktop/webserver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/cwindom/Desktop/webserver/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/webserver.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/webserver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/webserver.dir/flags.make

CMakeFiles/webserver.dir/main.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/cwindom/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/webserver.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/main.cpp.o -c /Users/cwindom/Desktop/webserver/main.cpp

CMakeFiles/webserver.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/cwindom/Desktop/webserver/main.cpp > CMakeFiles/webserver.dir/main.cpp.i

CMakeFiles/webserver.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/cwindom/Desktop/webserver/main.cpp -o CMakeFiles/webserver.dir/main.cpp.s

CMakeFiles/webserver.dir/convertConfig.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/convertConfig.cpp.o: ../convertConfig.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/cwindom/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/webserver.dir/convertConfig.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/convertConfig.cpp.o -c /Users/cwindom/Desktop/webserver/convertConfig.cpp

CMakeFiles/webserver.dir/convertConfig.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/convertConfig.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/cwindom/Desktop/webserver/convertConfig.cpp > CMakeFiles/webserver.dir/convertConfig.cpp.i

CMakeFiles/webserver.dir/convertConfig.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/convertConfig.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/cwindom/Desktop/webserver/convertConfig.cpp -o CMakeFiles/webserver.dir/convertConfig.cpp.s

CMakeFiles/webserver.dir/server_tools.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/server_tools.cpp.o: ../server_tools.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/cwindom/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/webserver.dir/server_tools.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/server_tools.cpp.o -c /Users/cwindom/Desktop/webserver/server_tools.cpp

CMakeFiles/webserver.dir/server_tools.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/server_tools.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/cwindom/Desktop/webserver/server_tools.cpp > CMakeFiles/webserver.dir/server_tools.cpp.i

CMakeFiles/webserver.dir/server_tools.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/server_tools.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/cwindom/Desktop/webserver/server_tools.cpp -o CMakeFiles/webserver.dir/server_tools.cpp.s

CMakeFiles/webserver.dir/utils.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/utils.cpp.o: ../utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/cwindom/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/webserver.dir/utils.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/utils.cpp.o -c /Users/cwindom/Desktop/webserver/utils.cpp

CMakeFiles/webserver.dir/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/utils.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/cwindom/Desktop/webserver/utils.cpp > CMakeFiles/webserver.dir/utils.cpp.i

CMakeFiles/webserver.dir/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/utils.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/cwindom/Desktop/webserver/utils.cpp -o CMakeFiles/webserver.dir/utils.cpp.s

CMakeFiles/webserver.dir/readConfig.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/readConfig.cpp.o: ../readConfig.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/cwindom/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/webserver.dir/readConfig.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/readConfig.cpp.o -c /Users/cwindom/Desktop/webserver/readConfig.cpp

CMakeFiles/webserver.dir/readConfig.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/readConfig.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/cwindom/Desktop/webserver/readConfig.cpp > CMakeFiles/webserver.dir/readConfig.cpp.i

CMakeFiles/webserver.dir/readConfig.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/readConfig.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/cwindom/Desktop/webserver/readConfig.cpp -o CMakeFiles/webserver.dir/readConfig.cpp.s

CMakeFiles/webserver.dir/Request.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/Request.cpp.o: ../Request.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/cwindom/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/webserver.dir/Request.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/Request.cpp.o -c /Users/cwindom/Desktop/webserver/Request.cpp

CMakeFiles/webserver.dir/Request.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/Request.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/cwindom/Desktop/webserver/Request.cpp > CMakeFiles/webserver.dir/Request.cpp.i

CMakeFiles/webserver.dir/Request.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/Request.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/cwindom/Desktop/webserver/Request.cpp -o CMakeFiles/webserver.dir/Request.cpp.s

CMakeFiles/webserver.dir/Server.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/Server.cpp.o: ../Server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/cwindom/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/webserver.dir/Server.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/Server.cpp.o -c /Users/cwindom/Desktop/webserver/Server.cpp

CMakeFiles/webserver.dir/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/Server.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/cwindom/Desktop/webserver/Server.cpp > CMakeFiles/webserver.dir/Server.cpp.i

CMakeFiles/webserver.dir/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/Server.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/cwindom/Desktop/webserver/Server.cpp -o CMakeFiles/webserver.dir/Server.cpp.s

# Object files for target webserver
webserver_OBJECTS = \
"CMakeFiles/webserver.dir/main.cpp.o" \
"CMakeFiles/webserver.dir/convertConfig.cpp.o" \
"CMakeFiles/webserver.dir/server_tools.cpp.o" \
"CMakeFiles/webserver.dir/utils.cpp.o" \
"CMakeFiles/webserver.dir/readConfig.cpp.o" \
"CMakeFiles/webserver.dir/Request.cpp.o" \
"CMakeFiles/webserver.dir/Server.cpp.o"

# External object files for target webserver
webserver_EXTERNAL_OBJECTS =

webserver: CMakeFiles/webserver.dir/main.cpp.o
webserver: CMakeFiles/webserver.dir/convertConfig.cpp.o
webserver: CMakeFiles/webserver.dir/server_tools.cpp.o
webserver: CMakeFiles/webserver.dir/utils.cpp.o
webserver: CMakeFiles/webserver.dir/readConfig.cpp.o
webserver: CMakeFiles/webserver.dir/Request.cpp.o
webserver: CMakeFiles/webserver.dir/Server.cpp.o
webserver: CMakeFiles/webserver.dir/build.make
webserver: CMakeFiles/webserver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/cwindom/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable webserver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/webserver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/webserver.dir/build: webserver

.PHONY : CMakeFiles/webserver.dir/build

CMakeFiles/webserver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/webserver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/webserver.dir/clean

CMakeFiles/webserver.dir/depend:
	cd /Users/cwindom/Desktop/webserver/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/cwindom/Desktop/webserver /Users/cwindom/Desktop/webserver /Users/cwindom/Desktop/webserver/cmake-build-debug /Users/cwindom/Desktop/webserver/cmake-build-debug /Users/cwindom/Desktop/webserver/cmake-build-debug/CMakeFiles/webserver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/webserver.dir/depend

