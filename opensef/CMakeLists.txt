# openSEF - Open SeagrEnv Framework
# Build order matters due to dependencies

cmake_minimum_required(VERSION 3.16)
project(opensef VERSION 0.2.0 LANGUAGES C CXX)

# Check for wlroots (compositor is optional)
find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(WLROOTS wlroots)
endif()

# Components in dependency order
add_subdirectory(opensef-base)      # Foundation classes
add_subdirectory(opensef-core)      # Animation & Layer framework
add_subdirectory(opensef-appkit)    # Widget library (Buttons, Windows)

# Compositor (only if wlroots found)
if(WLROOTS_FOUND)
    message(STATUS "wlroots found - building compositor")
    add_subdirectory(opensef-compositor)
else()
    message(STATUS "wlroots NOT found - skipping compositor")
endif()

# Shell (Desktop UI)
add_subdirectory(opensef-shell)

# Test programs
if(BUILD_TESTING)
    add_subdirectory(test)
endif()


