# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_SOURCE_DIR = "/home/deepak/temp2/Systray/Notification Center Tooggle"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/deepak/temp2/Systray/Notification Center Tooggle/build"

# Utility rule file for nc-toggle-tray_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/nc-toggle-tray_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/nc-toggle-tray_autogen.dir/progress.make

CMakeFiles/nc-toggle-tray_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir="/home/deepak/temp2/Systray/Notification Center Tooggle/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target nc-toggle-tray"
	/usr/bin/cmake -E cmake_autogen "/home/deepak/temp2/Systray/Notification Center Tooggle/build/CMakeFiles/nc-toggle-tray_autogen.dir/AutogenInfo.json" ""

nc-toggle-tray_autogen: CMakeFiles/nc-toggle-tray_autogen
nc-toggle-tray_autogen: CMakeFiles/nc-toggle-tray_autogen.dir/build.make
.PHONY : nc-toggle-tray_autogen

# Rule to build all files generated by this target.
CMakeFiles/nc-toggle-tray_autogen.dir/build: nc-toggle-tray_autogen
.PHONY : CMakeFiles/nc-toggle-tray_autogen.dir/build

CMakeFiles/nc-toggle-tray_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/nc-toggle-tray_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/nc-toggle-tray_autogen.dir/clean

CMakeFiles/nc-toggle-tray_autogen.dir/depend:
	cd "/home/deepak/temp2/Systray/Notification Center Tooggle/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/deepak/temp2/Systray/Notification Center Tooggle" "/home/deepak/temp2/Systray/Notification Center Tooggle" "/home/deepak/temp2/Systray/Notification Center Tooggle/build" "/home/deepak/temp2/Systray/Notification Center Tooggle/build" "/home/deepak/temp2/Systray/Notification Center Tooggle/build/CMakeFiles/nc-toggle-tray_autogen.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/nc-toggle-tray_autogen.dir/depend

