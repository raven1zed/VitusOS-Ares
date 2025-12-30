# opensef-compositor: wlroots-based Wayland Compositor

cmake_minimum_required(VERSION 3.16)
project(opensef-compositor VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)

# wlroots and dependencies
pkg_check_modules(WLROOTS REQUIRED IMPORTED_TARGET wlroots-0.19)
pkg_check_modules(WAYLAND REQUIRED IMPORTED_TARGET wayland-server)
pkg_check_modules(WAYLAND_PROTOCOLS REQUIRED wayland-protocols)
pkg_check_modules(XKB REQUIRED IMPORTED_TARGET xkbcommon)
pkg_check_modules(PIXMAN REQUIRED IMPORTED_TARGET pixman-1)
pkg_check_modules(DRM REQUIRED IMPORTED_TARGET libdrm)
pkg_check_modules(INPUT REQUIRED IMPORTED_TARGET libinput)

# XWayland support (optional)
pkg_check_modules(XWAYLAND xcb xcb-composite)

# Find wayland-scanner for generating protocol code
find_program(WAYLAND_SCANNER wayland-scanner REQUIRED)

# Get wayland-protocols path
pkg_get_variable(WAYLAND_PROTOCOLS_DIR wayland-protocols pkgdatadir)

# Generate XDG shell protocol
set(XDG_SHELL_PROTOCOL "${WAYLAND_PROTOCOLS_DIR}/stable/xdg-shell/xdg-shell.xml")
set(XDG_SHELL_HEADER "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.h")
set(XDG_SHELL_CODE "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.c")

add_custom_command(
    OUTPUT ${XDG_SHELL_HEADER}
    COMMAND ${WAYLAND_SCANNER} server-header ${XDG_SHELL_PROTOCOL} ${XDG_SHELL_HEADER}
    DEPENDS ${XDG_SHELL_PROTOCOL}
    COMMENT "Generating xdg-shell-protocol.h"
)

add_custom_command(
    OUTPUT ${XDG_SHELL_CODE}
    COMMAND ${WAYLAND_SCANNER} private-code ${XDG_SHELL_PROTOCOL} ${XDG_SHELL_CODE}
    DEPENDS ${XDG_SHELL_PROTOCOL}
    COMMENT "Generating xdg-shell-protocol.c"
)

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
    # Scene-based rendering
    src/OSFDesktopLayers.cpp
    src/OSFSceneWallpaper.cpp
    src/OSFSceneDock.cpp
    src/OSFSceneMenuBar.cpp
    # Core framework
    src/OSFLayer.cpp
    src/OSFTransaction.cpp
    src/OSFDisplayLink.cpp
    src/OSFResponder.cpp
    # UI Components
    src/OSFWindowDecorations.cpp
    src/OSFTextRenderer.cpp
    # UI Screens
    src/OSFLockScreen.cpp
    src/OSFShutdownScreen.cpp
    src/OSFWelcomeScreen.cpp
    # Generated protocol code
    ${XDG_SHELL_CODE}
    ${XDG_SHELL_HEADER}
)

target_include_directories(opensef-compositor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}  # For generated headers
    ${WLROOTS_INCLUDE_DIRS}
    ${WAYLAND_INCLUDE_DIRS}
    ${XKB_INCLUDE_DIRS}
    ${PIXMAN_INCLUDE_DIRS}
)

target_link_libraries(opensef-compositor PRIVATE
    PkgConfig::WLROOTS
    PkgConfig::WAYLAND
    PkgConfig::XKB
    PkgConfig::PIXMAN
    PkgConfig::DRM
    PkgConfig::INPUT
)

# Required for wlroots
target_compile_definitions(opensef-compositor PRIVATE
    WLR_USE_UNSTABLE
)

# Allow C99 extensions for wlroots headers (they use static array syntax)
target_compile_options(opensef-compositor PRIVATE
    -Wall -Wextra
    -Wno-c99-extensions
)
