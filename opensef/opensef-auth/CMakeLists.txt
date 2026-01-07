# opensef-auth - Authentication library
cmake_minimum_required(VERSION 3.20)

project(opensef-auth LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find PAM
find_package(PkgConfig REQUIRED)
pkg_check_modules(PAM REQUIRED pam)

# Source files
set(SOURCES
    src/OSFAuth.cpp
)

# Create static library
add_library(opensef-auth STATIC ${SOURCES})

target_include_directories(opensef-auth
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${PAM_INCLUDE_DIRS}
)

target_link_libraries(opensef-auth
    PRIVATE
        ${PAM_LIBRARIES}
)

target_compile_options(opensef-auth PRIVATE
    -Wall -Wextra -Wpedantic
)
