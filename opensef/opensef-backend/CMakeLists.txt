# opensef-backend: Display Backend (C++ Version)

project(opensef-backend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND REQUIRED wayland-client)
pkg_check_modules(XKB REQUIRED xkbcommon)

# Generate XDG shell protocol
find_program(WAYLAND_SCANNER wayland-scanner)

set(XDG_SHELL_PROTOCOL "/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml")
# NixOS path
if(NOT EXISTS ${XDG_SHELL_PROTOCOL})
    execute_process(
        COMMAND pkg-config --variable=pkgdatadir wayland-protocols
        OUTPUT_VARIABLE WAYLAND_PROTOCOLS_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(XDG_SHELL_PROTOCOL "${WAYLAND_PROTOCOLS_DIR}/stable/xdg-shell/xdg-shell.xml")
endif()

set(XDG_SHELL_C "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.c")
set(XDG_SHELL_H "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-client-protocol.h")

add_custom_command(
    OUTPUT ${XDG_SHELL_C}
    COMMAND ${WAYLAND_SCANNER} private-code ${XDG_SHELL_PROTOCOL} ${XDG_SHELL_C}
    DEPENDS ${XDG_SHELL_PROTOCOL}
)

add_custom_command(
    OUTPUT ${XDG_SHELL_H}
    COMMAND ${WAYLAND_SCANNER} client-header ${XDG_SHELL_PROTOCOL} ${XDG_SHELL_H}
    DEPENDS ${XDG_SHELL_PROTOCOL}
)

add_library(opensef-backend SHARED
    src/OSFBackend.cpp
    src/OSFWaylandSurface.cpp
    ${XDG_SHELL_C}
)

target_include_directories(opensef-backend PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}  # For generated headers
    ${WAYLAND_INCLUDE_DIRS}
)

target_link_libraries(opensef-backend PUBLIC
    opensef-base
    ${WAYLAND_LIBRARIES}
    ${XKB_LIBRARIES}
)

target_compile_options(opensef-backend PRIVATE
    -Wall -Wextra
)
