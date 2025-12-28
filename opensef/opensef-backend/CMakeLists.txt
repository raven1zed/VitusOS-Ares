# opensef-backend: Display Backend (C++ Version)

project(opensef-backend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND REQUIRED wayland-client)
pkg_check_modules(XKB REQUIRED xkbcommon)

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

set(XDG_SHELL_XML "${WAYLAND_PROTOCOLS_DIR}/stable/xdg-shell/xdg-shell.xml")
set(XDG_SHELL_C "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.c")
set(XDG_SHELL_H "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-client-protocol.h")

# Generate XDG shell protocol files BEFORE building
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

# Create a target for the generated files
add_custom_target(xdg_shell_protocol DEPENDS ${XDG_SHELL_H} ${XDG_SHELL_C})

add_library(opensef-backend SHARED
    src/OSFBackend.cpp
    src/OSFWaylandSurface.cpp
    ${XDG_SHELL_C}
)

# Depend on protocol generation
add_dependencies(opensef-backend xdg_shell_protocol)

target_include_directories(opensef-backend PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(opensef-backend PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}  # For generated xdg-shell-client-protocol.h
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
