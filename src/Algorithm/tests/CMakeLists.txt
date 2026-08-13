include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2          # pinned: reproducible builds
)
FetchContent_MakeAvailable(googletest)

add_executable(algorithm_tests
    algorithm_tests.cpp
)

target_link_libraries(algorithm_tests PRIVATE
    algorithm_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(algorithm_tests
)
