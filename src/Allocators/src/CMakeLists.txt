add_library(allocators_lib
    BoundaryAllocator/BoundaryAllocator.cpp
    BuddyAllocator/BuddyAllocator.cpp
    BaseAllocator.h
)

target_include_directories(allocators_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/BuddyAllocator
        ${CMAKE_CURRENT_SOURCE_DIR}/BoundaryAllocator
)

add_executable(Allocators main.cpp)
target_link_libraries(Allocators PRIVATE allocators_lib)

