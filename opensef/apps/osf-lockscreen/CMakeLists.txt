# osf-lockscreen - VitusOS Lock Screen
cmake_minimum_required(VERSION 3.20)

project(osf-lockscreen LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(WAYLAND REQUIRED wayland-client)

# Source files
set(SOURCES
    main.cpp
)

# Create executable
add_executable(osf-lockscreen ${SOURCES})

target_include_directories(osf-lockscreen PRIVATE
    ${CMAKE_SOURCE_DIR}/opensef/opensef-shell/include
    ${CMAKE_SOURCE_DIR}/opensef/opensef-auth/include
    ${CMAKE_SOURCE_DIR}/opensef/opensef-base/include
    ${CAIRO_INCLUDE_DIRS}
    ${WAYLAND_INCLUDE_DIRS}
)

target_link_libraries(osf-lockscreen PRIVATE
    opensef-auth
    ${CMAKE_BINARY_DIR}/opensef/opensef-shell/CMakeFiles/osf-shell.dir/src/render/OSFSurface.cpp.o
    osf-protocols
    ${CAIRO_LIBRARIES}
    ${WAYLAND_LIBRARIES}
    wayland-client
    xkbcommon
    rsvg-2
)

target_compile_options(osf-lockscreen PRIVATE
    -Wall -Wextra -Wpedantic
)

# Install
install(TARGETS osf-lockscreen
    RUNTIME DESTINATION bin
)
