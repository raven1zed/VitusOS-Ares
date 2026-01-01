# opensef-base: Foundation Layer (C++ Version)

project(opensef-base)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND REQUIRED wayland-client)
pkg_check_modules(CAIRO REQUIRED cairo)

# Generate XDG shell protocol
find_program(WAYLAND_SCANNER wayland-scanner)

# Find wayland-protocols path using pkg-config (works on NixOS and standard distros)
pkg_get_variable(WAYLAND_PROTOCOLS_DIR wayland-protocols pkgdatadir)
if(NOT WAYLAND_PROTOCOLS_DIR)
    # Fallback to standard path
    set(WAYLAND_PROTOCOLS_DIR "/usr/share/wayland-protocols")
endif()

set(XDG_SHELL_XML "${WAYLAND_PROTOCOLS_DIR}/stable/xdg-shell/xdg-shell.xml")
set(XDG_SHELL_HEADER "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-client-protocol.h")
set(XDG_SHELL_CODE "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.c")

message(STATUS "Looking for XDG shell protocol at: ${XDG_SHELL_XML}")

if(EXISTS ${XDG_SHELL_XML})
    add_custom_command(
        OUTPUT ${XDG_SHELL_HEADER}
        COMMAND ${WAYLAND_SCANNER} client-header ${XDG_SHELL_XML} ${XDG_SHELL_HEADER}
        DEPENDS ${XDG_SHELL_XML}
        COMMENT "Generating XDG shell client header"
    )
    add_custom_command(
        OUTPUT ${XDG_SHELL_CODE}
        COMMAND ${WAYLAND_SCANNER} private-code ${XDG_SHELL_XML} ${XDG_SHELL_CODE}
        DEPENDS ${XDG_SHELL_XML}
        COMMENT "Generating XDG shell protocol code"
    )
    set(XDG_SHELL_SOURCES ${XDG_SHELL_CODE})
else()
    message(WARNING "XDG shell protocol not found at ${XDG_SHELL_XML}")
    message(WARNING "OSFWindow will not be built. Install wayland-protocols package.")
    set(XDG_SHELL_SOURCES "")
endif()
if(EXISTS ${XDG_SHELL_XML})
    set(OSFWINDOW_SOURCES src/OSFWindow.cpp ${XDG_SHELL_CODE})
else()
    set(OSFWINDOW_SOURCES "")
endif()

add_library(opensef-base SHARED
    src/OSFApplication.cpp
    src/OSFBundle.cpp
    src/OSFNotificationCenter.cpp
    src/OSFObject.cpp
    src/OSFRunLoop.cpp
    ${OSFWINDOW_SOURCES}
)

target_include_directories(opensef-base PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${WAYLAND_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
)

target_link_libraries(opensef-base PUBLIC
    ${WAYLAND_LIBRARIES}
    ${CAIRO_LIBRARIES}
)

target_compile_options(opensef-base PRIVATE
    -Wall -Wextra -Wpedantic
)

