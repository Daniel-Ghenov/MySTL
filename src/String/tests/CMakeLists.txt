include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2          # pinned: reproducible builds
)
FetchContent_MakeAvailable(googletest)

add_executable(string_tests
    string_tests.cpp
)

target_link_libraries(string_tests PRIVATE
    string_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(string_tests
)
