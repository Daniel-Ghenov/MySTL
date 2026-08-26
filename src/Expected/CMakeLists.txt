add_library(expected_lib INTERFACE)

target_include_directories(expected_lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
