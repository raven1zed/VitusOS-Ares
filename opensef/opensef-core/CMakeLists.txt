# opensef-core: Animation Framework
# Core animation primitives for openSEF UI

cmake_minimum_required(VERSION 3.16)
project(opensef-core VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)

# Core animation library
add_library(opensef-core STATIC
    src/OSFAnimation.cpp
    src/OSFLayer.cpp
)

target_include_directories(opensef-core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/opensef/opensef-base/include
    ${CAIRO_INCLUDE_DIRS}
)

target_link_libraries(opensef-core PUBLIC
    ${CAIRO_LIBRARIES}
)

# Install
install(TARGETS opensef-core DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/opensef)
