cmake_minimum_required(VERSION 3.5.0)
project(triangles VERSION 0.1.0 LANGUAGES C CXX)

add_executable(triangles main.cpp geometry.hpp float_cmp.hpp)
add_executable(examples examples.cpp geometry.hpp float_cmp.hpp)

