cmake_minimum_required(VERSION 3.5.0)
project(triangles VERSION 0.1.0 LANGUAGES C CXX)

# to remove warnings
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

option(BUILD_TESTING YES)
option(BUILD_CONSOLE_ONLY NO)

if (BUILD_TESTING)
  add_subdirectory(tests)
endif()

if (BUILD_CONSOLE_ONLY)
  add_executable(triangles ${CMAKE_SOURCE_DIR}/src/main.cpp)
  target_include_directories(triangles PRIVATE ${CMAKE_SOURCE_DIR}/src)
else()
  add_subdirectory(visualization)
endif()
