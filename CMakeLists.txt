cmake_minimum_required(VERSION 3.19)
project(tempat_cpp)

set(CMAKE_CXX_STANDARD 17)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/include/patterns/*.h)
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/include/patterns/*.cpp)

add_executable(tempat_cpp main.cpp ${HEADERS} ${SOURCES})