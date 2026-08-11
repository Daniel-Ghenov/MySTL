include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2          # pinned: reproducible builds
)
FetchContent_MakeAvailable(googletest)

add_executable(vector_tests
    vector_tests.cpp
)

target_link_libraries(vector_tests PRIVATE
    vector_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(vector_tests
)
