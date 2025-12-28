# opensef-ui: Design System (C++ Version)

project(opensef-ui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Header-only for now (inline static methods)
add_library(opensef-ui INTERFACE)

target_include_directories(opensef-ui INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(opensef-ui INTERFACE
    opensef-base
)
