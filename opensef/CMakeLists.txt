# openSEF - Open SeagrEnv Framework
# Build order matters due to dependencies

cmake_minimum_required(VERSION 3.16)
project(opensef VERSION 0.2.0 LANGUAGES C CXX)

option(BUILD_TESTING "Build test programs" ON)

# Check for wlroots (compositor is optional)
find_package(PkgConfig)
if(PkgConfig_FOUND)
    # Search for versioned packages (NixOS/Arch often use wlroots-0.18 etc.)
    pkg_search_module(WLROOTS wlroots wlroots-0.19 wlroots-0.18 wlroots-0.17 wlroots-0.16)
endif()

# Components in dependency order
add_subdirectory(opensef-base)      # Foundation classes
add_subdirectory(opensef-core)      # Animation & Layer framework
add_subdirectory(opensef-appkit)    # Widget library (Buttons, Windows)
add_subdirectory(opensef-auth)      # Authentication (PAM)
add_subdirectory(opensef-framework) # Unified Desktop Environment Framework

# Compositor (only if wlroots found)
if(WLROOTS_FOUND)
    message(STATUS "wlroots found - building compositor")
    add_subdirectory(opensef-compositor)
else()
    message(STATUS "wlroots NOT found - skipping compositor")
endif()

# Qt Quick Shell (requires Qt6) - PRODUCTION SHELL
find_package(Qt6 QUIET COMPONENTS Core Gui Quick)
if(Qt6_FOUND)
    message(STATUS "Qt6 found - building Qt Quick shell")
    add_subdirectory(opensef-shell-qt)
else()
    message(STATUS "Qt6 NOT found - skipping Qt Quick shell")
endif()

# Native Apps (Linux additions)
add_subdirectory(apps)

# Test programs
if(BUILD_TESTING)
    add_subdirectory(test)
endif()


