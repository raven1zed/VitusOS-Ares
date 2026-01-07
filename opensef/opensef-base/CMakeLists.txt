# opensef-base: Foundation Layer (C++ Version)

cmake_minimum_required(VERSION 3.16)
project(opensef-base VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND REQUIRED wayland-client)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)

# === XDG Shell Protocol Generation ===
# Required for OSFWindow (XDG surface support)
find_program(WAYLAND_SCANNER wayland-scanner REQUIRED)
pkg_get_variable(WAYLAND_PROTOCOLS_DIR wayland-protocols pkgdatadir)

set(XDG_SHELL_XML "${WAYLAND_PROTOCOLS_DIR}/stable/xdg-shell/xdg-shell.xml")
set(XDG_SHELL_H "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-client-protocol.h")
set(XDG_SHELL_C "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.c")

message(STATUS "XDG Shell XML: ${XDG_SHELL_XML}")

add_custom_command(OUTPUT ${XDG_SHELL_H}
    COMMAND ${WAYLAND_SCANNER} client-header ${XDG_SHELL_XML} ${XDG_SHELL_H}
    DEPENDS ${XDG_SHELL_XML}
    COMMENT "Generating XDG shell client header"
)

add_custom_command(OUTPUT ${XDG_SHELL_C}
    COMMAND ${WAYLAND_SCANNER} private-code ${XDG_SHELL_XML} ${XDG_SHELL_C}
    DEPENDS ${XDG_SHELL_XML}
    COMMENT "Generating XDG shell protocol code"
)

# === Library ===
add_library(opensef-base SHARED
    # Foundation classes (no Wayland dependency)
    src/OSFApplication.cpp
    src/OSFBundle.cpp
    src/OSFNotificationCenter.cpp
    src/OSFObject.cpp
    src/OSFRunLoop.cpp
    src/OSFView.cpp
    src/OSFStackView.cpp
    src/OSFShortcutManager.cpp
    # Windowing (Wayland dependency)
    src/OSFWindow.cpp
    ${XDG_SHELL_C}
    ${XDG_SHELL_H}
)

target_include_directories(opensef-base PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${WAYLAND_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
    ${XKBCOMMON_INCLUDE_DIRS}
)

target_link_libraries(opensef-base PUBLIC
    opensef-core
    ${WAYLAND_LIBRARIES}
    ${CAIRO_LIBRARIES}
    ${XKBCOMMON_LIBRARIES}
)

target_compile_options(opensef-base PRIVATE
    -Wall -Wextra -Wpedantic
)
