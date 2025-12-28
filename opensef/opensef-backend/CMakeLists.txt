# opensef-backend: Display Backend (C++ Version)

project(opensef-backend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND REQUIRED wayland-client)
pkg_check_modules(XKB REQUIRED xkbcommon)

add_library(opensef-backend SHARED
    src/OSFBackend.cpp
    src/OSFVulkanRenderer.cpp
)

target_include_directories(opensef-backend PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
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
