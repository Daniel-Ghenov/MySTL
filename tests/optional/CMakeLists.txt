add_executable(optional_tests
    optional_tests.cpp
)

target_link_libraries(optional_tests PRIVATE
    optional_lib
    GTest::gtest_main
)

gtest_discover_tests(optional_tests)
