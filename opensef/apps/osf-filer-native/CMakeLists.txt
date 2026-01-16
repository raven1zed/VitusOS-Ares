# osf-filer-native - Native File Manager (C++ AppKit)

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

target_link_libraries(osf-filer-native PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Core5Compat
    -Wl,--whole-archive
    ${CMAKE_CURRENT_SOURCE_DIR}/../../opensef-gnustep/build/libopensef-gnustep-qtquick.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../opensef-gnustep/build/libopensef-gnustep.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../opensef-core/build/libopensef-core.a
    -Wl,--no-whole-archive
    ${CMAKE_CURRENT_SOURCE_DIR}/../../opensef-framework/build/libopensef-framework.so
    ${CMAKE_CURRENT_SOURCE_DIR}/../../opensef-base/build/libopensef-base.so
    ${CAIRO_LIBRARIES}
)

install(TARGETS osf-filer-native
    RUNTIME DESTINATION bin
)
