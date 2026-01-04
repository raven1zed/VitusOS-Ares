cmake_minimum_required(VERSION 3.20)

project(VitusOS
    VERSION 0.1.0
    DESCRIPTION "VitusOS Ares - openSEF Framework"
    LANGUAGES C CXX OBJC
)

# ============================================================================
# openSEF: Open SeagrEnv (Seamless Integration Environment) Framework
# Theme: Ares / The Martian
# ============================================================================

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build options
option(BUILD_TESTING "Build test programs" ON)
option(BUILD_APPS "Build VitusOS applications" OFF)

# Clang is required for Objective-C 2.0 features
if(NOT CMAKE_C_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR "Clang is required for openSEF. Found: ${CMAKE_C_COMPILER_ID}")
endif()

# Objective-C flags for GNUStep 2.0 runtime
set(OBJC_FLAGS "-fobjc-runtime=gnustep-2.0 -fobjc-arc -fblocks -fobjc-nonfragile-abi")
set(CMAKE_OBJC_FLAGS "${CMAKE_OBJC_FLAGS} ${OBJC_FLAGS}")
set(CMAKE_OBJCXX_FLAGS "${CMAKE_OBJCXX_FLAGS} ${OBJC_FLAGS}")

# ============================================================================
# Dependencies
# ============================================================================

find_package(PkgConfig REQUIRED)

# Wayland (VitusOS is Wayland-only)
pkg_check_modules(WAYLAND REQUIRED
    wayland-client
    wayland-protocols
    xkbcommon
)

# Vulkan
find_package(Vulkan REQUIRED)

# GNUStep (libobjc2)
pkg_check_modules(GNUSTEP_BASE gnustep-base)

# ============================================================================
# Subdirectories
# ============================================================================

# openSEF Framework (always build)
add_subdirectory(opensef)

# VitusOS Applications (optional)
if(BUILD_APPS)
    add_subdirectory(apps/seadrop)
    add_subdirectory(apps/vitus-installer)
endif()

# ============================================================================
# Summary
# ============================================================================

message(STATUS "")
message(STATUS "╔════════════════════════════════════════════════════════╗")
message(STATUS "║              VitusOS Ares Build Summary                ║")
message(STATUS "╠════════════════════════════════════════════════════════╣")
message(STATUS "║  Version:    ${PROJECT_VERSION}")
message(STATUS "║  Framework:  openSEF (SeagrEnv Framework)")
message(STATUS "║  Theme:      Ares (The Martian)")
message(STATUS "║  Compiler:   ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "║  Wayland:    ${WAYLAND_VERSION}")
message(STATUS "║  Vulkan:     ${Vulkan_VERSION}")
message(STATUS "║  Tests:      ${BUILD_TESTING}")
message(STATUS "║  Apps:       ${BUILD_APPS}")
message(STATUS "╚════════════════════════════════════════════════════════╝")
message(STATUS "")
