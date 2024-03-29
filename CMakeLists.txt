cmake_minimum_required(VERSION 3.14)
project(cppargparser)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define your test executable, including the source files for the tests
add_executable(tests tests/tests.cpp)
# Link the Google Test main libraries to your test executable
target_link_libraries(tests gtest_main)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# Add the tests to CMake's testing framework
include(GoogleTest)
gtest_discover_tests(tests)

if(DEFINED ENV{CPPARGPARSER_DIR})
    include_directories($ENV{CPPARGPARSER_DIR})
else()
    message(FATAL_ERROR "CPPARGPARSER_DIR environment variable is not set.")
endif()

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

