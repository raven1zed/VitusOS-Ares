# SeaDrop for VitusOS
# openSEF Native Frontend + libseadrop Backend

project(seadrop-vitusos VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

# ============================================================================
# libseadrop (submodule from SeagrEnv)
# ============================================================================

# Option 1: Submodule (recommended for development)
# add_subdirectory(${CMAKE_SOURCE_DIR}/../SeagrEnv/libseadrop libseadrop)

# Option 2: Find installed library
find_library(LIBSEADROP seadrop HINTS /usr/local/lib)
find_path(LIBSEADROP_INCLUDE seadrop/seadrop.h HINTS /usr/local/include)

# ============================================================================
# SeaDrop VitusOS (openSEF frontend)
# ============================================================================

add_executable(seadrop-vitusos
    src/main.cpp
    src/SeaDropApp.cpp
    src/SeaDropWindow.cpp
    src/SeaDropTray.cpp
)

target_include_directories(seadrop-vitusos PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${LIBSEADROP_INCLUDE}
)

target_link_libraries(seadrop-vitusos
    # openSEF framework
    opensef-base
    opensef-appkit
    opensef-backend
    opensef-ui
    
    # libseadrop core
    ${LIBSEADROP}
    
    # libseadrop dependencies
    sodium      # libsodium encryption
    bluetooth   # BlueZ
)

# ============================================================================
# Install
# ============================================================================

install(TARGETS seadrop-vitusos DESTINATION /usr/bin RENAME seadrop)
install(FILES resources/seadrop.desktop DESTINATION /usr/share/applications)
install(FILES resources/seadrop.svg DESTINATION /usr/share/icons/hicolor/scalable/apps)

# ============================================================================
# Note: For full build, clone SeagrEnv as submodule
# ============================================================================
# git submodule add https://github.com/raven1zed/SeagrEnv external/SeagrEnv
