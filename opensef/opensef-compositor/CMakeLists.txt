# opensef-compositor: wlroots-based Wayland Compositor

project(opensef-compositor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

# wlroots and dependencies
pkg_check_modules(WLROOTS REQUIRED wlroots-0.18)
pkg_check_modules(WAYLAND REQUIRED wayland-server)
pkg_check_modules(XKB REQUIRED xkbcommon)
pkg_check_modules(PIXMAN REQUIRED pixman-1)
pkg_check_modules(DRM REQUIRED libdrm)
pkg_check_modules(INPUT REQUIRED libinput)

# XWayland support (optional)
pkg_check_modules(XWAYLAND xcb xcb-composite)

add_executable(opensef-compositor
    src/main.cpp
    src/OSFCompositor.cpp
    src/OSFOutput.cpp
    src/OSFView.cpp
    src/OSFKeyboard.cpp
    src/OSFCursor.cpp
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
