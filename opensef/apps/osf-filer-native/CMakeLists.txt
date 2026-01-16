# osf-filer-native - Native File Manager (C++ AppKit)
# Always-running service that hosts Pathfinder

cmake_minimum_required(VERSION 3.16)
project(osf-filer-native VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick Core5Compat)

add_executable(osf-filer-native
    main.cpp
)

# Find Cairo for core graphics
find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)

target_include_directories(osf-filer-native PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../opensef-gnustep/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../opensef-gnustep/bridge
    ${CMAKE_CURRENT_SOURCE_DIR}/../../opensef-framework/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../opensef-base/include
)

# Link to CMake targets (built in current build directory)
target_link_libraries(osf-filer-native PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Core5Compat
    -Wl,--whole-archive
    opensef-gnustep-qtquick
    opensef-gnustep
    opensef-core
    -Wl,--no-whole-archive
    opensef-framework
    opensef-base
    ${CAIRO_LIBRARIES}
)

install(TARGETS osf-filer-native
    RUNTIME DESTINATION bin
)
