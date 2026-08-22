add_library(any_lib
    Any.cpp
)

target_include_directories(any_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
