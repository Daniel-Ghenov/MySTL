add_library(string_lib
    Helper.cpp
    MyString.cpp
    RollingHash/RollingHash.cpp
    RollingHash/StringAlg.cpp
)

target_include_directories(string_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/RollingHash
)

