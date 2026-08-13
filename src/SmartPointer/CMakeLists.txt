add_library(smart_pointer_lib INTERFACE)

target_include_directories(smart_pointer_lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(ALLOC_BUILD_TESTS)
    add_subdirectory(tests)
endif()
