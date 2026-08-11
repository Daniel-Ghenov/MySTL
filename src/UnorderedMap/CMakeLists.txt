add_library(unordered_map_lib INTERFACE)

target_include_directories(unordered_map_lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(unordered_map_lib INTERFACE vector_lib list_lib)


