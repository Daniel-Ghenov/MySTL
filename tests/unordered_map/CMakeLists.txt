add_executable(unordered_map_tests
    unordered_map_tests.cpp
)

target_link_libraries(unordered_map_tests PRIVATE
    unordered_map_lib
    GTest::gtest_main
)

gtest_discover_tests(unordered_map_tests)
