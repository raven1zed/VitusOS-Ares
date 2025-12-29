# opensef-backend: Display Backend (C++ Version)

project(opensef-backend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND REQUIRED wayland-client wayland-cursor)
pkg_check_modules(XKB REQUIRED xkbcommon)
pkg_check_modules(FREETYPE REQUIRED freetype2)

# Find wayland-scanner
find_program(WAYLAND_SCANNER wayland-scanner REQUIRED)

# Find wayland-protocols
execute_process(
    COMMAND pkg-config --variable=pkgdatadir wayland-protocols
    OUTPUT_VARIABLE WAYLAND_PROTOCOLS_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Wayland protocols dir: ${WAYLAND_PROTOCOLS_DIR}")
message(STATUS "Wayland scanner: ${WAYLAND_SCANNER}")
message(STATUS "FreeType: ${FREETYPE_LIBRARIES}")

set(XDG_SHELL_XML "${WAYLAND_PROTOCOLS_DIR}/stable/xdg-shell/xdg-shell.xml")
set(XDG_SHELL_C "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.c")
set(XDG_SHELL_H "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-client-protocol.h")

# Generate XDG shell protocol files
add_custom_command(
    OUTPUT ${XDG_SHELL_H}
    COMMAND ${WAYLAND_SCANNER} client-header ${XDG_SHELL_XML} ${XDG_SHELL_H}
    DEPENDS ${XDG_SHELL_XML}
    COMMENT "Generating XDG shell client header"
)

add_custom_command(
    OUTPUT ${XDG_SHELL_C}
    COMMAND ${WAYLAND_SCANNER} private-code ${XDG_SHELL_XML} ${XDG_SHELL_C}
    DEPENDS ${XDG_SHELL_XML}
    COMMENT "Generating XDG shell protocol code"
)

add_custom_target(xdg_shell_protocol DEPENDS ${XDG_SHELL_H} ${XDG_SHELL_C})

add_library(opensef-backend SHARED
    src/OSFBackend.cpp
    src/OSFWaylandSurface.cpp
    src/OSFTextRenderer.cpp
    ${XDG_SHELL_C}
)

add_dependencies(opensef-backend xdg_shell_protocol)

target_include_directories(opensef-backend PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(opensef-backend PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${WAYLAND_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
)

target_link_libraries(opensef-backend PUBLIC
    opensef-base
    ${WAYLAND_LIBRARIES}
    ${XKB_LIBRARIES}
    ${FREETYPE_LIBRARIES}
)

target_compile_options(opensef-backend PRIVATE
    -Wall -Wextra
)
