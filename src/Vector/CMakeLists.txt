add_library(vector_lib INTERFACE)

target_include_directories(vector_lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(vector_lib INTERFACE iterator_lib)

if(ALLOC_BUILD_TESTS)
    add_subdirectory(tests)
endif()

