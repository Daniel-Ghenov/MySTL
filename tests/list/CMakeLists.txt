add_executable(list_tests
    list_tests.cpp
)

target_link_libraries(list_tests PRIVATE
    list_lib
    GTest::gtest_main
)

gtest_discover_tests(list_tests)
