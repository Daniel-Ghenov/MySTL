cmake_minimum_required(VERSION 3.31)
project(OS)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

option(ALLOC_BUILD_TESTS "Build the test suite" ON)

add_subdirectory(src)

if(ALLOC_BUILD_TESTS)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.15.2          # pinned: reproducible builds
    )
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)

    add_subdirectory(tests)
endif()
