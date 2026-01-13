# opensef-gnustep - C++ AppKit with Qt Quick Rendering
# GNUstep converted to C++, no Objective-C, no Cairo

cmake_minimum_required(VERSION 3.16)
project(opensef-gnustep VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick)

# C++ AppKit library (no Objective-C, no Cairo)
add_library(opensef-gnustep STATIC
    src/NSView.cpp
    src/NSControl.cpp
    src/NSButton.cpp
)

target_include_directories(opensef-gnustep PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(opensef-gnustep PUBLIC
    opensef-framework
    opensef-core
    Qt6::Core
)

target_compile_options(opensef-gnustep PRIVATE
    -Wall -Wextra -Wno-unused-parameter
)

# Qt Quick rendering bridge
add_library(opensef-gnustep-qtquick STATIC
    bridge/QQuickNSButton.cpp
)

target_include_directories(opensef-gnustep-qtquick PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/bridge
)

target_link_libraries(opensef-gnustep-qtquick PUBLIC
    opensef-gnustep
    Qt6::Quick
)

target_compile_options(opensef-gnustep-qtquick PRIVATE
    -Wall -Wextra -Wno-unused-parameter
)

# Install
install(TARGETS opensef-gnustep opensef-gnustep-qtquick
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/opensef DESTINATION include)
