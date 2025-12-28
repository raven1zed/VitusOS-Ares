# openSEF Framework
# Open SeagrEnv (Seamless Integration Environment) Framework
# Core GUI framework for VitusOS

# Build order matters - base first, then backend (no deps on appkit)
# then ui (uses base), then appkit (uses base+backend)
add_subdirectory(opensef-base)
add_subdirectory(opensef-backend)
add_subdirectory(opensef-ui)
add_subdirectory(opensef-appkit)

# Test programs (optional, only if building tests)
if(BUILD_TESTING)
    add_subdirectory(test)
endif()
