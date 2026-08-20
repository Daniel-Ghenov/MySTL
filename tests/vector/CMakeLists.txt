add_executable(vector_tests
    vector_tests.cpp
)

target_link_libraries(vector_tests PRIVATE
    vector_lib
    GTest::gtest_main
)

gtest_discover_tests(vector_tests)
