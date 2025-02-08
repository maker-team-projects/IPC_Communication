cmake_minimum_required(VERSION 3.22)
project(opencv_test)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(read read.cpp)
target_link_libraries(read ${OpenCV_LIBS})

add_executable(write write.cpp)
target_link_libraries(write ${OpenCV_LIBS})

add_executable(just_read just_read.cpp)
target_link_libraries(just_read ${OpenCV_LIBS})

add_executable(just_write just_write.cpp)
target_link_libraries(just_write ${OpenCV_LIBS})