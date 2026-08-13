include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2          # pinned: reproducible builds
)
FetchContent_MakeAvailable(googletest)

add_executable(smart_pointer_tests
    smart_pointer_tests.cpp
)

target_link_libraries(smart_pointer_tests PRIVATE
    smart_pointer_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(smart_pointer_tests
)
