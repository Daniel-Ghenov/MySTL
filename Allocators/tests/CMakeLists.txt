include(FetchContent)
 
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2          # pinned: reproducible builds
)
FetchContent_MakeAvailable(googletest)
 
add_executable(allocator_tests
    boundary_allocator_tests.cpp
    buddy_allocator_tests.cpp
)
 
target_link_libraries(allocator_tests PRIVATE
    allocators_lib
    GTest::gtest_main
)
 
target_compile_definitions(allocator_tests PRIVATE $<$<CONFIG:Release>:ALLOCATORS_TESTS_NEED_DEBUG>)

include(GoogleTest)
gtest_discover_tests(allocator_tests
)
