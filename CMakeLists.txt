cmake_minimum_required(VERSION 3.24)
project(keyboard)

set(CMAKE_CXX_STANDARD 17)
add_link_options(-static)
add_executable(keyboard main.cpp)
