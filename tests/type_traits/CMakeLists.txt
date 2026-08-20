add_executable(type_traits_tests
    type_traits_tests.cpp
)

target_link_libraries(type_traits_tests PRIVATE
    type_traits_lib
    GTest::gtest_main
)

gtest_discover_tests(type_traits_tests)
