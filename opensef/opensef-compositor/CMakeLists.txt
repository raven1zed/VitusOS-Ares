# opensef-compositor: wlroots-based Wayland Compositor

cmake_minimum_required(VERSION 3.16)
project(opensef-compositor VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)

# wlroots and dependencies
# NOTE: wlroots 0.19 uses versioned pkg-config name "wlroots-0.19"
pkg_check_modules(WLROOTS REQUIRED IMPORTED_TARGET wlroots-0.19)
pkg_check_modules(WAYLAND REQUIRED IMPORTED_TARGET wayland-server)
pkg_check_modules(XKB REQUIRED IMPORTED_TARGET xkbcommon)
pkg_check_modules(PIXMAN REQUIRED IMPORTED_TARGET pixman-1)
pkg_check_modules(DRM REQUIRED IMPORTED_TARGET libdrm)
pkg_check_modules(INPUT REQUIRED IMPORTED_TARGET libinput)



# XWayland support (optional)
pkg_check_modules(XWAYLAND xcb xcb-composite)

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
    # Core framework (from R&D research)
    src/OSFLayer.cpp
    src/OSFTransaction.cpp
    src/OSFDisplayLink.cpp
    src/OSFResponder.cpp
    # UI Components (Ares theme)
    src/OSFWindowDecorations.cpp
    src/OSFTextRenderer.cpp
    # UI Screens (Ares theme)
    src/OSFLockScreen.cpp
    src/OSFShutdownScreen.cpp
    src/OSFWelcomeScreen.cpp
)

target_include_directories(opensef-compositor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${WLROOTS_INCLUDE_DIRS}
    ${WAYLAND_INCLUDE_DIRS}
    ${XKB_INCLUDE_DIRS}
    ${PIXMAN_INCLUDE_DIRS}
)

target_link_libraries(opensef-compositor PRIVATE
    ${WLROOTS_LIBRARIES}
    ${WAYLAND_LIBRARIES}
    ${XKB_LIBRARIES}
    ${PIXMAN_LIBRARIES}
    ${DRM_LIBRARIES}
    ${INPUT_LIBRARIES}
)

# Required for wlroots
target_compile_definitions(opensef-compositor PRIVATE
    WLR_USE_UNSTABLE
)

target_compile_options(opensef-compositor PRIVATE
    -Wall -Wextra
)
