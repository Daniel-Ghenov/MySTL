include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2          # pinned: reproducible builds
)
FetchContent_MakeAvailable(googletest)

add_executable(pair_tests
    pair_tests.cpp
)

target_link_libraries(pair_tests PRIVATE
    tuple_pair
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(pair_tests
)
