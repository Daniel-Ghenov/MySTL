add_executable(string_tests
    string_tests.cpp
)

target_link_libraries(string_tests PRIVATE
    string_lib
    GTest::gtest_main
)

gtest_discover_tests(string_tests)
