# openSEF - Open SeagrEnv Framework (C++ Version)
# Build order matters due to dependencies

project(opensef)

# Check for wlroots (compositor is optional)
find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(WLROOTS wlroots)
endif()

# Components in dependency order
add_subdirectory(opensef-base)      # No dependencies
add_subdirectory(opensef-ui)        # Depends on base (interface only)
add_subdirectory(opensef-backend)   # Depends on base (Wayland client)
add_subdirectory(opensef-appkit)    # Depends on base, ui, backend

# Compositor (only if wlroots found)
if(WLROOTS_FOUND)
    message(STATUS "wlroots found - building compositor")
    add_subdirectory(opensef-compositor)
else()
    message(STATUS "wlroots NOT found - skipping compositor (install wlroots to build)")
endif()

# Test programs
if(BUILD_TESTING)
    add_subdirectory(test)
endif()


