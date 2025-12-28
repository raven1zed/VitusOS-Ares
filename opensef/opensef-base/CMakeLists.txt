# opensef-base: Foundation Layer (C++ Version)

project(opensef-base)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(opensef-base SHARED
    src/OSFObject.cpp
)

target_include_directories(opensef-base PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(opensef-base PRIVATE
    -Wall -Wextra -Wpedantic
)
