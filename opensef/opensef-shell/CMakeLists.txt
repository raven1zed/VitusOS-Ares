# opensef-shell: C++ UI Layer for openSEF Compositor
# Uses Cairo/Pango for rendering, wlr-layer-shell for integration

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

# Core framework library (from R&D research)
add_library(osf-core STATIC
    # src/core/OSFLayer.cpp
    # src/core/OSFTransaction.cpp
    # src/core/OSFDisplayLink.cpp
    # src/core/OSFResponder.cpp
    src/core/OSFAresTheme.cpp
)

target_include_directories(osf-core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CAIRO_INCLUDE_DIRS}
    ${PANGO_INCLUDE_DIRS}
)

# Rendering library
add_library(osf-render STATIC
    src/render/OSFSurface.cpp
    # src/render/OSFTextRenderer.cpp
    # src/render/OSFIconLoader.cpp
    ${LAYER_SHELL_C}
    ${LAYER_SHELL_H}
    ${XDG_OUTPUT_C}
    ${XDG_OUTPUT_H}
)

target_include_directories(osf-render PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${WAYLAND_CLIENT_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
    ${PANGO_INCLUDE_DIRS}
    ${RSVG_INCLUDE_DIRS}
)

target_link_libraries(osf-render PUBLIC
    osf-core
    ${WAYLAND_CLIENT_LIBRARIES}
    ${WAYLAND_CURSOR_LIBRARIES}
    ${CAIRO_LIBRARIES}
    ${PANGO_LIBRARIES}
    ${RSVG_LIBRARIES}
)

# Global Menu Panel
add_executable(osf-panel
    src/panel/main.cpp
    src/panel/OSFPanel.cpp
    # src/panel/OSFGlobalMenu.cpp
    # src/panel/OSFSystemTray.cpp
    # src/panel/OSFClock.cpp
)

target_link_libraries(osf-panel PRIVATE osf-render)

# Dock (Commented out until implemented)
# add_executable(osf-dock
#     src/dock/main.cpp
#     src/dock/OSFDock.cpp
#     src/dock/OSFDockItem.cpp
# )
# target_link_libraries(osf-dock PRIVATE osf-render)

# Install
install(TARGETS osf-panel osf-dock DESTINATION bin)
