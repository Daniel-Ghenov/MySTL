add_executable(smart_pointer_tests
    smart_pointer_tests.cpp
)

target_link_libraries(smart_pointer_tests PRIVATE
    smart_pointer_lib
    GTest::gtest_main
)

gtest_discover_tests(smart_pointer_tests)
