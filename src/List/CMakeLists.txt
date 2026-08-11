add_library(list_lib INTERFACE)

target_include_directories(list_lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(ALLOC_BUILD_TESTS)
    add_subdirectory(tests)
endif()
