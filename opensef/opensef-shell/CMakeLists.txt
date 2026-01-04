# opensef-shell: C++ UI Layer for openSEF Compositor
# Uses Cairo/Pango for rendering, wlr-layer-shell for integration
# Now integrated with opensef-core (Animation) and opensef-appkit (Widgets)

cmake_minimum_required(VERSION 3.16)
project(opensef-shell VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)

# Wayland client
pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client)
pkg_check_modules(WAYLAND_CURSOR REQUIRED wayland-cursor)

# Rendering
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(PANGO REQUIRED pangocairo)
pkg_check_modules(RSVG REQUIRED librsvg-2.0)

# Generate layer-shell protocol
find_program(WAYLAND_SCANNER wayland-scanner REQUIRED)
pkg_get_variable(WAYLAND_PROTOCOLS_DIR wayland-protocols pkgdatadir)

# wlr-layer-shell protocol
set(LAYER_SHELL_XML "${CMAKE_CURRENT_SOURCE_DIR}/protocols/wlr-layer-shell-unstable-v1.xml")
set(LAYER_SHELL_H "${CMAKE_CURRENT_BINARY_DIR}/wlr-layer-shell-unstable-v1-client-protocol.h")
set(LAYER_SHELL_C "${CMAKE_CURRENT_BINARY_DIR}/wlr-layer-shell-unstable-v1-protocol.c")

add_custom_command(OUTPUT ${LAYER_SHELL_H}
    COMMAND ${WAYLAND_SCANNER} client-header ${LAYER_SHELL_XML} ${LAYER_SHELL_H}
    DEPENDS ${LAYER_SHELL_XML})

add_custom_command(OUTPUT ${LAYER_SHELL_C}
    COMMAND ${WAYLAND_SCANNER} private-code ${LAYER_SHELL_XML} ${LAYER_SHELL_C}
    DEPENDS ${LAYER_SHELL_XML})

# XDG output protocol
set(XDG_OUTPUT_XML "${WAYLAND_PROTOCOLS_DIR}/unstable/xdg-output/xdg-output-unstable-v1.xml")
set(XDG_OUTPUT_H "${CMAKE_CURRENT_BINARY_DIR}/xdg-output-unstable-v1-client-protocol.h")
set(XDG_OUTPUT_C "${CMAKE_CURRENT_BINARY_DIR}/xdg-output-unstable-v1-protocol.c")

add_custom_command(OUTPUT ${XDG_OUTPUT_H}
    COMMAND ${WAYLAND_SCANNER} client-header ${XDG_OUTPUT_XML} ${XDG_OUTPUT_H}
    DEPENDS ${XDG_OUTPUT_XML})

add_custom_command(OUTPUT ${XDG_OUTPUT_C}
    COMMAND ${WAYLAND_SCANNER} private-code ${XDG_OUTPUT_XML} ${XDG_OUTPUT_C}
    DEPENDS ${XDG_OUTPUT_XML})

# XDG shell protocol (Required for xdg_popup_interface symbols)
set(XDG_SHELL_XML "${WAYLAND_PROTOCOLS_DIR}/stable/xdg-shell/xdg-shell.xml")
set(XDG_SHELL_H "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-client-protocol.h")
set(XDG_SHELL_C "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.c")

add_custom_command(OUTPUT ${XDG_SHELL_H}
    COMMAND ${WAYLAND_SCANNER} client-header ${XDG_SHELL_XML} ${XDG_SHELL_H}
    DEPENDS ${XDG_SHELL_XML})

add_custom_command(OUTPUT ${XDG_SHELL_C}
    COMMAND ${WAYLAND_SCANNER} private-code ${XDG_SHELL_XML} ${XDG_SHELL_C}
    DEPENDS ${XDG_SHELL_XML})

# Rendering library (uses opensef-core and opensef-appkit)
add_library(osf-shell-lib STATIC
    src/render/OSFSurface.cpp
    src/core/OSFAresTheme.cpp
    src/panel/OSFPanel.cpp
    src/dock/OSFDock.cpp
    src/wallpaper/OSFWallpaper.cpp
    ${LAYER_SHELL_C}
    ${LAYER_SHELL_H}
    ${XDG_OUTPUT_C}
    ${XDG_OUTPUT_H}
    ${XDG_SHELL_C}
    ${XDG_SHELL_H}
)

target_include_directories(osf-shell-lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}
    ${WAYLAND_CLIENT_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
    ${PANGO_INCLUDE_DIRS}
    ${RSVG_INCLUDE_DIRS}
)

target_link_libraries(osf-shell-lib PUBLIC
    opensef-core
    opensef-appkit
    opensef-base
    ${WAYLAND_CLIENT_LIBRARIES}
    ${WAYLAND_CURSOR_LIBRARIES}
    ${CAIRO_LIBRARIES}
    ${PANGO_LIBRARIES}
    ${RSVG_LIBRARIES}
)

# Main Desktop Shell Executable
add_executable(osf-shell
    src/main.cpp
)

target_link_libraries(osf-shell PRIVATE osf-shell-lib)

# Install
install(TARGETS osf-shell DESTINATION bin)
