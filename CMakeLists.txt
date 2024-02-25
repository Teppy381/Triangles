cmake_minimum_required(VERSION 3.5.0)
project(triangles VERSION 0.1.0 LANGUAGES C CXX)

# to remove warnings
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

include(CTest)
include(FetchContent)

message(STATUS "Fetching googletest...")
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        58d77fa8070e8cec2dc1ed015d66b454c8d78850 # release-1.12.1
#   FIND_PACKAGE_ARGS NAMES GTest
)

FetchContent_MakeAvailable(googletest)

option(BUILD_TESTING YES)
option(BUILD_CONSOLE_ONLY NO)

if (BUILD_TESTING)
  add_subdirectory(tests)
endif()

if (NOT BUILD_CONSOLE_ONLY)
  # vulkan and glfw ???
endif()

add_executable(triangles src/main.cpp)
