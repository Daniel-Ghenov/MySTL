include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2          # pinned: reproducible builds
)
FetchContent_MakeAvailable(googletest)

add_executable(type_traits_tests
    type_traits_tests.cpp
)

target_link_libraries(type_traits_tests PRIVATE
    type_traits_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(type_traits_tests
)
