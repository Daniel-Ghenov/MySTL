cmake_minimum_required(VERSION 3.31)
project(OS)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

option(ALLOC_BUILD_TESTS "Build the test suite" ON)
if(ALLOC_BUILD_TESTS)
    enable_testing()
endif()

add_subdirectory(src)
