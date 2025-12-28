cmake_minimum_required(VERSION 3.20)

project(VitusOS
    VERSION 0.1.0
    DESCRIPTION "VitusOS Ares - openSEF Framework"
    LANGUAGES C CXX OBJC
)

# ============================================================================
# openSEF: Open SeagrEnv (Seamless Integration Environment) Framework
# ============================================================================

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# openSEF Framework
add_subdirectory(opensef)

# SeaDrop (pre-installed, core to VitusOS)
# add_subdirectory(apps/seadrop)

# Desktop shell (vitus-shell)
# add_subdirectory(vitus-shell)

# ============================================================================
# Summary
# ============================================================================

message(STATUS "")
message(STATUS "╔════════════════════════════════════════════════════════╗")
message(STATUS "║              VitusOS Ares Build Summary                ║")
message(STATUS "╠════════════════════════════════════════════════════════╣")
message(STATUS "║  Version:    ${PROJECT_VERSION}")
message(STATUS "║  Framework:  openSEF (SeagrEnv Framework)")
message(STATUS "║  Compiler:   ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "║  Wayland:    ${WAYLAND_VERSION}")
message(STATUS "║  Vulkan:     ${Vulkan_VERSION}")
message(STATUS "╚════════════════════════════════════════════════════════╝")
message(STATUS "")
