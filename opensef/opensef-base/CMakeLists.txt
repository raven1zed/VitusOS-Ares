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
set(XDG_SHELL_XML "/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml")
set(XDG_SHELL_HEADER "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-client-protocol.h")
set(XDG_SHELL_CODE "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.c")

if(EXISTS ${XDG_SHELL_XML})
    add_custom_command(
        OUTPUT ${XDG_SHELL_HEADER}
        COMMAND ${WAYLAND_SCANNER} client-header ${XDG_SHELL_XML} ${XDG_SHELL_HEADER}
        DEPENDS ${XDG_SHELL_XML}
    )
    add_custom_command(
        OUTPUT ${XDG_SHELL_CODE}
        COMMAND ${WAYLAND_SCANNER} private-code ${XDG_SHELL_XML} ${XDG_SHELL_CODE}
        DEPENDS ${XDG_SHELL_XML}
    )
else()
    # Fallback: Try NixOS path
    set(XDG_SHELL_XML_NIX "$ENV{XDG_SHELL_PROTOCOL}")
    if(XDG_SHELL_XML_NIX)
        set(XDG_SHELL_XML ${XDG_SHELL_XML_NIX})
        add_custom_command(
            OUTPUT ${XDG_SHELL_HEADER}
            COMMAND ${WAYLAND_SCANNER} client-header ${XDG_SHELL_XML} ${XDG_SHELL_HEADER}
            DEPENDS ${XDG_SHELL_XML}
        )
        add_custom_command(
            OUTPUT ${XDG_SHELL_CODE}
            COMMAND ${WAYLAND_SCANNER} private-code ${XDG_SHELL_XML} ${XDG_SHELL_CODE}
            DEPENDS ${XDG_SHELL_XML}
        )
    endif()
endif()

add_library(opensef-base SHARED
    src/OSFApplication.cpp
    src/OSFBundle.cpp
    src/OSFNotificationCenter.cpp
    src/OSFObject.cpp
    src/OSFRunLoop.cpp
    src/OSFWindow.cpp
    ${XDG_SHELL_CODE}
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

