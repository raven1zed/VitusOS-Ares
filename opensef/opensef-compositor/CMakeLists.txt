# opensef-compositor: wlroots-based Wayland Compositor

cmake_minimum_required(VERSION 3.16)
project(opensef-compositor VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)

# wlroots and dependencies (using 0.17 which is C++ compatible)
pkg_check_modules(WLROOTS REQUIRED wlroots)

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

# ============================================================================
# Main compositor
# ============================================================================
add_executable(opensef-compositor
    src/main.cpp
    src/OSFCompositor.cpp
    src/OSFOutput.cpp
    src/OSFView.cpp
    src/OSFKeyboard.cpp
    src/OSFCursor.cpp
    src/OSFDecorations.cpp
    src/OSFAnimations.cpp
    src/OSFDock.cpp
    src/OSFMenuBar.cpp
    src/OSFWorkspaceView.cpp
    src/OSFXWayland.cpp
    src/OSFWallpaper.cpp
    src/OSFDesktopLayers.cpp
    src/OSFSceneWallpaper.cpp
    src/OSFSceneDock.cpp
    src/OSFSceneMenuBar.cpp
    src/OSFLayer.cpp
    src/OSFTransaction.cpp
    src/OSFDisplayLink.cpp
    src/OSFResponder.cpp
    src/OSFWindowDecorations.cpp
    src/OSFTextRenderer.cpp
    src/OSFLockScreen.cpp
    src/OSFShutdownScreen.cpp
    src/OSFWelcomeScreen.cpp
    src/wlr_compat.c
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

target_compile_definitions(opensef-compositor PRIVATE WLR_USE_UNSTABLE)

# GCC: -fpermissive converts C99 errors to warnings
# This is needed because wlroots headers use C99 [static 4] syntax
target_compile_options(opensef-compositor PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-fpermissive>
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-pedantic>
    -Wall
    -Wno-unused-parameter
    -Wno-missing-field-initializers
)
