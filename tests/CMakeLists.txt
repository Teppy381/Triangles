include(CTest)
include(FetchContent)

message(STATUS "Fetching googletest...")
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        58d77fa8070e8cec2dc1ed015d66b454c8d78850 # release-1.12.1
  FIND_PACKAGE_ARGS NAMES GTest
)

FetchContent_MakeAvailable(googletest)

add_executable(my_test my_tests.cpp)
target_include_directories(my_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(my_test PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(my_test)
