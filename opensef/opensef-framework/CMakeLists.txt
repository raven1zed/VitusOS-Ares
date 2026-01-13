cmake_minimum_required(VERSION 3.16)
project(opensef-framework VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(PANGO REQUIRED pango pangocairo)
pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)

# Framework library
add_library(opensef-framework SHARED
    src/OSFDesktop.cpp
    src/OSFEventBus.cpp
    src/OSFStateManager.cpp
    src/OSFWindowManager.cpp
    src/OSFServiceRegistry.cpp
    src/OSFResourceCache.cpp
    src/OSFThemeManager.cpp
    src/OSFFrameworkC.cpp
    src/OSFShortcutManager.cpp
    src/OSFClipboard.cpp
)

target_include_directories(opensef-framework PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CAIRO_INCLUDE_DIRS}
    ${PANGO_INCLUDE_DIRS}
    ${XKBCOMMON_INCLUDE_DIRS}
)

target_link_libraries(opensef-framework PUBLIC
    ${CAIRO_LIBRARIES}
    ${PANGO_LIBRARIES}
    ${XKBCOMMON_LIBRARIES}
    pthread
)

target_compile_options(opensef-framework PRIVATE
    -Wall -Wextra -Wpedantic
)

# Install
install(TARGETS opensef-framework DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/opensef-framework)
