cmake_minimum_required(VERSION 3.12)
project(leetcode)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(pybind11)
pybind11_add_module(leetcode  main.cpp)
#add_executable(leetcode main.cpp)