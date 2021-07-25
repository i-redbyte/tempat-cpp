cmake_minimum_required(VERSION 3.19)
project(tempat_cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(tempat_cpp main.cpp patterns/visitor/visitor.h patterns/visitor/visitor_example.h)