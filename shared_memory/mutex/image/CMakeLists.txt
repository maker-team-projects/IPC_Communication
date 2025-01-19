cmake_minimum_required(VERSION 3.22)
project(opencv_test)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(read read.cpp)
target_link_libraries(read ${OpenCV_LIBS})

add_executable(write write.cpp)
target_link_libraries(write ${OpenCV_LIBS})