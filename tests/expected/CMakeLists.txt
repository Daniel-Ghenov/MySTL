add_executable(expected_tests
    expected_tests.cpp
)

target_link_libraries(expected_tests PRIVATE
    expected_lib
    GTest::gtest_main
)

gtest_discover_tests(expected_tests)
