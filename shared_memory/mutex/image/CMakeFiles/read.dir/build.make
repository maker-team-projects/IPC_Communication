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
CMAKE_SOURCE_DIR = /home/blakelee/programming/IPC_Communication/shared_memory/mutex/image

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/blakelee/programming/IPC_Communication/shared_memory/mutex/image

# Include any dependencies generated for this target.
include CMakeFiles/read.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/read.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/read.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/read.dir/flags.make

CMakeFiles/read.dir/read.cpp.o: CMakeFiles/read.dir/flags.make
CMakeFiles/read.dir/read.cpp.o: read.cpp
CMakeFiles/read.dir/read.cpp.o: CMakeFiles/read.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/blakelee/programming/IPC_Communication/shared_memory/mutex/image/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/read.dir/read.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/read.dir/read.cpp.o -MF CMakeFiles/read.dir/read.cpp.o.d -o CMakeFiles/read.dir/read.cpp.o -c /home/blakelee/programming/IPC_Communication/shared_memory/mutex/image/read.cpp

CMakeFiles/read.dir/read.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/read.dir/read.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/blakelee/programming/IPC_Communication/shared_memory/mutex/image/read.cpp > CMakeFiles/read.dir/read.cpp.i

CMakeFiles/read.dir/read.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/read.dir/read.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/blakelee/programming/IPC_Communication/shared_memory/mutex/image/read.cpp -o CMakeFiles/read.dir/read.cpp.s

# Object files for target read
read_OBJECTS = \
"CMakeFiles/read.dir/read.cpp.o"

# External object files for target read
read_EXTERNAL_OBJECTS =

read: CMakeFiles/read.dir/read.cpp.o
read: CMakeFiles/read.dir/build.make
read: /usr/local/lib/libopencv_gapi.so.4.4.0
read: /usr/local/lib/libopencv_stitching.so.4.4.0
read: /usr/local/lib/libopencv_alphamat.so.4.4.0
read: /usr/local/lib/libopencv_aruco.so.4.4.0
read: /usr/local/lib/libopencv_bgsegm.so.4.4.0
read: /usr/local/lib/libopencv_bioinspired.so.4.4.0
read: /usr/local/lib/libopencv_ccalib.so.4.4.0
read: /usr/local/lib/libopencv_cudabgsegm.so.4.4.0
read: /usr/local/lib/libopencv_cudafeatures2d.so.4.4.0
read: /usr/local/lib/libopencv_cudaobjdetect.so.4.4.0
read: /usr/local/lib/libopencv_cudastereo.so.4.4.0
read: /usr/local/lib/libopencv_dnn_objdetect.so.4.4.0
read: /usr/local/lib/libopencv_dnn_superres.so.4.4.0
read: /usr/local/lib/libopencv_dpm.so.4.4.0
read: /usr/local/lib/libopencv_face.so.4.4.0
read: /usr/local/lib/libopencv_freetype.so.4.4.0
read: /usr/local/lib/libopencv_fuzzy.so.4.4.0
read: /usr/local/lib/libopencv_hfs.so.4.4.0
read: /usr/local/lib/libopencv_img_hash.so.4.4.0
read: /usr/local/lib/libopencv_intensity_transform.so.4.4.0
read: /usr/local/lib/libopencv_line_descriptor.so.4.4.0
read: /usr/local/lib/libopencv_quality.so.4.4.0
read: /usr/local/lib/libopencv_rapid.so.4.4.0
read: /usr/local/lib/libopencv_reg.so.4.4.0
read: /usr/local/lib/libopencv_rgbd.so.4.4.0
read: /usr/local/lib/libopencv_saliency.so.4.4.0
read: /usr/local/lib/libopencv_stereo.so.4.4.0
read: /usr/local/lib/libopencv_structured_light.so.4.4.0
read: /usr/local/lib/libopencv_superres.so.4.4.0
read: /usr/local/lib/libopencv_surface_matching.so.4.4.0
read: /usr/local/lib/libopencv_tracking.so.4.4.0
read: /usr/local/lib/libopencv_videostab.so.4.4.0
read: /usr/local/lib/libopencv_xfeatures2d.so.4.4.0
read: /usr/local/lib/libopencv_xobjdetect.so.4.4.0
read: /usr/local/lib/libopencv_xphoto.so.4.4.0
read: /usr/local/lib/libopencv_shape.so.4.4.0
read: /usr/local/lib/libopencv_highgui.so.4.4.0
read: /usr/local/lib/libopencv_datasets.so.4.4.0
read: /usr/local/lib/libopencv_plot.so.4.4.0
read: /usr/local/lib/libopencv_text.so.4.4.0
read: /usr/local/lib/libopencv_dnn.so.4.4.0
read: /usr/local/lib/libopencv_ml.so.4.4.0
read: /usr/local/lib/libopencv_phase_unwrapping.so.4.4.0
read: /usr/local/lib/libopencv_cudacodec.so.4.4.0
read: /usr/local/lib/libopencv_videoio.so.4.4.0
read: /usr/local/lib/libopencv_cudaoptflow.so.4.4.0
read: /usr/local/lib/libopencv_cudalegacy.so.4.4.0
read: /usr/local/lib/libopencv_cudawarping.so.4.4.0
read: /usr/local/lib/libopencv_optflow.so.4.4.0
read: /usr/local/lib/libopencv_ximgproc.so.4.4.0
read: /usr/local/lib/libopencv_video.so.4.4.0
read: /usr/local/lib/libopencv_imgcodecs.so.4.4.0
read: /usr/local/lib/libopencv_objdetect.so.4.4.0
read: /usr/local/lib/libopencv_calib3d.so.4.4.0
read: /usr/local/lib/libopencv_features2d.so.4.4.0
read: /usr/local/lib/libopencv_flann.so.4.4.0
read: /usr/local/lib/libopencv_photo.so.4.4.0
read: /usr/local/lib/libopencv_cudaimgproc.so.4.4.0
read: /usr/local/lib/libopencv_cudafilters.so.4.4.0
read: /usr/local/lib/libopencv_imgproc.so.4.4.0
read: /usr/local/lib/libopencv_cudaarithm.so.4.4.0
read: /usr/local/lib/libopencv_core.so.4.4.0
read: /usr/local/lib/libopencv_cudev.so.4.4.0
read: CMakeFiles/read.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/blakelee/programming/IPC_Communication/shared_memory/mutex/image/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable read"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/read.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/read.dir/build: read
.PHONY : CMakeFiles/read.dir/build

CMakeFiles/read.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/read.dir/cmake_clean.cmake
.PHONY : CMakeFiles/read.dir/clean

CMakeFiles/read.dir/depend:
	cd /home/blakelee/programming/IPC_Communication/shared_memory/mutex/image && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/blakelee/programming/IPC_Communication/shared_memory/mutex/image /home/blakelee/programming/IPC_Communication/shared_memory/mutex/image /home/blakelee/programming/IPC_Communication/shared_memory/mutex/image /home/blakelee/programming/IPC_Communication/shared_memory/mutex/image /home/blakelee/programming/IPC_Communication/shared_memory/mutex/image/CMakeFiles/read.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/read.dir/depend

