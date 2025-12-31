# opensef-compositor: Pure C Wayland Compositor
# VitusOS Ares Desktop Environment

cmake_minimum_required(VERSION 3.16)
project(opensef-compositor VERSION 0.2.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)

# wlroots and dependencies
# wlroots and dependencies (search for versioned pc files)
pkg_search_module(WLROOTS REQUIRED wlroots wlroots-0.18 wlroots-0.17 wlroots-0.16)
pkg_check_modules(WAYLAND_SERVER REQUIRED wayland-server)
pkg_check_modules(WAYLAND_PROTOCOLS REQUIRED wayland-protocols)
pkg_check_modules(XKB REQUIRED xkbcommon)
pkg_check_modules(PIXMAN REQUIRED pixman-1)
pkg_check_modules(DRM REQUIRED libdrm)
pkg_check_modules(INPUT REQUIRED libinput)
pkg_check_modules(XCB xcb)

# Find wayland-scanner
find_program(WAYLAND_SCANNER wayland-scanner REQUIRED)

# Get wayland-protocols path
pkg_get_variable(WAYLAND_PROTOCOLS_DIR wayland-protocols pkgdatadir)

# Generate XDG shell protocol
set(XDG_SHELL_XML "${WAYLAND_PROTOCOLS_DIR}/stable/xdg-shell/xdg-shell.xml")
set(XDG_SHELL_H "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.h")
set(XDG_SHELL_C "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.c")

add_custom_command(OUTPUT ${XDG_SHELL_H}
    COMMAND ${WAYLAND_SCANNER} server-header ${XDG_SHELL_XML} ${XDG_SHELL_H}
    DEPENDS ${XDG_SHELL_XML})

add_custom_command(OUTPUT ${XDG_SHELL_C}
    COMMAND ${WAYLAND_SCANNER} private-code ${XDG_SHELL_XML} ${XDG_SHELL_C}
    DEPENDS ${XDG_SHELL_XML})

# Main compositor executable
add_executable(opensef-compositor
    src/main.c
    src/server.c
    src/output.c
    src/view.c
    src/input.c
    src/layer_shell.c
    src/decorations.c
    ${XDG_SHELL_C}
    ${XDG_SHELL_H}
)

target_include_directories(opensef-compositor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${WLROOTS_INCLUDE_DIRS}
    ${WAYLAND_SERVER_INCLUDE_DIRS}
    ${XKB_INCLUDE_DIRS}
    ${PIXMAN_INCLUDE_DIRS}
)

target_link_libraries(opensef-compositor PRIVATE
    ${WLROOTS_LIBRARIES}
    ${WAYLAND_SERVER_LIBRARIES}
    ${XKB_LIBRARIES}
    ${PIXMAN_LIBRARIES}
    ${DRM_LIBRARIES}
    ${INPUT_LIBRARIES}
)

target_compile_definitions(opensef-compositor PRIVATE
    WLR_USE_UNSTABLE
)

target_compile_options(opensef-compositor PRIVATE
    -Wall
    -Wextra
    -Wno-unused-parameter
)

# Install
install(TARGETS opensef-compositor DESTINATION bin)
