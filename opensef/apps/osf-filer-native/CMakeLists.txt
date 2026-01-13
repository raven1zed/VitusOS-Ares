# osf-filer-native - Native File Manager (C++ AppKit)

cmake_minimum_required(VERSION 3.16)
project(osf-filer-native VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick)

add_executable(osf-filer-native
    main.cpp
)

target_include_directories(osf-filer-native PRIVATE
    ${CMAKE_SOURCE_DIR}/opensef-gnustep/include
    ${CMAKE_SOURCE_DIR}/opensef-gnustep/bridge
    ${CMAKE_SOURCE_DIR}/opensef-framework/include
)

target_link_libraries(osf-filer-native PRIVATE
    Qt6::Core
    Qt6::Quick
    opensef-gnustep
    opensef-gnustep-qtquick
    opensef-framework
)

install(TARGETS osf-filer-native
    RUNTIME DESTINATION bin
)
