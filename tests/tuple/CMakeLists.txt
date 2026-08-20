add_executable(pair_tests
    pair_tests.cpp
)

target_link_libraries(pair_tests PRIVATE
    tuple_pair
    GTest::gtest_main
)

gtest_discover_tests(pair_tests)
