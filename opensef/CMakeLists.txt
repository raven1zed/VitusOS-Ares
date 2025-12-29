# openSEF - Open SeagrEnv Framework (C++ Version)
# Build order matters due to dependencies

project(opensef)

# Components in dependency order
add_subdirectory(opensef-base)      # No dependencies
add_subdirectory(opensef-ui)        # Depends on base (interface only)
add_subdirectory(opensef-backend)   # Depends on base (Wayland client)
add_subdirectory(opensef-appkit)    # Depends on base, ui, backend
add_subdirectory(opensef-compositor) # wlroots Wayland compositor

# Test programs
if(BUILD_TESTING)
    add_subdirectory(test)
endif()

