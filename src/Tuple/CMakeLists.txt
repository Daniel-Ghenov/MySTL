add_library(tuple_pair INTERFACE)

target_include_directories(tuple_pair
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(ALLOC_BUILD_TESTS)
    add_subdirectory(tests)
endif()
