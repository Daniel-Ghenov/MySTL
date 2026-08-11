include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2          # pinned: reproducible builds
)
FetchContent_MakeAvailable(googletest)

add_executable(unordered_map_tests
    unordered_map_tests.cpp
)

target_link_libraries(unordered_map_tests PRIVATE
    unordered_map_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unordered_map_tests
)
