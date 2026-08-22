add_executable(any_tests
    any_tests.cpp
)

target_link_libraries(any_tests PRIVATE
    any_lib
    GTest::gtest_main
)

gtest_discover_tests(any_tests)
