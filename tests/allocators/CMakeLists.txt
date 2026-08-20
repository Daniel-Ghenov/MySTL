add_executable(allocator_tests
    boundary_allocator_tests.cpp
    buddy_allocator_tests.cpp
)

target_link_libraries(allocator_tests PRIVATE
    allocators_lib
    GTest::gtest_main
)

target_compile_definitions(allocator_tests PRIVATE $<$<CONFIG:Release>:ALLOCATORS_TESTS_NEED_DEBUG>)

gtest_discover_tests(allocator_tests)
