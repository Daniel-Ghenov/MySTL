add_executable(algorithm_tests
    algorithm_tests.cpp
)

target_link_libraries(algorithm_tests PRIVATE
    algorithm_lib
    GTest::gtest_main
)

gtest_discover_tests(algorithm_tests)
