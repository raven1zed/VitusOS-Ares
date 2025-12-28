# SeaDrop for VitusOS
# openSEF Native Frontend + libseadrop Backend (from SeagrEnv submodule)

project(seadrop-vitusos VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================================
# libseadrop (from SeagrEnv submodule)
# ============================================================================

set(SEAGRENV_DIR ${CMAKE_SOURCE_DIR}/external/SeagrEnv)
set(LIBSEADROP_DIR ${SEAGRENV_DIR}/libseadrop)

if(EXISTS ${LIBSEADROP_DIR}/CMakeLists.txt)
    message(STATUS "Found libseadrop at ${LIBSEADROP_DIR}")
    add_subdirectory(${LIBSEADROP_DIR} ${CMAKE_BINARY_DIR}/libseadrop)
    set(LIBSEADROP_FOUND TRUE)
else()
    message(WARNING "libseadrop not found. Run: git submodule update --init --recursive")
    set(LIBSEADROP_FOUND FALSE)
endif()

# ============================================================================
# SeaDrop VitusOS (openSEF frontend)
# ============================================================================

add_executable(seadrop
    src/main.cpp
    src/SeaDropApp.cpp
    src/SeaDropWindow.cpp
    src/SeaDropTray.cpp
    src/SeaDropSettings.cpp
)

target_include_directories(seadrop PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link openSEF framework
target_link_libraries(seadrop PRIVATE
    opensef-base
    opensef-appkit
    opensef-backend
    opensef-ui
)

# Link libseadrop if available
if(LIBSEADROP_FOUND)
    target_include_directories(seadrop PRIVATE
        ${LIBSEADROP_DIR}/include
    )
    target_link_libraries(seadrop PRIVATE
        seadrop  # libseadrop library
    )
    target_compile_definitions(seadrop PRIVATE HAVE_LIBSEADROP=1)
else()
    # Stub mode - UI only, no sync
    target_compile_definitions(seadrop PRIVATE HAVE_LIBSEADROP=0)
endif()

# ============================================================================
# Install
# ============================================================================

install(TARGETS seadrop DESTINATION bin)
install(FILES resources/seadrop.desktop DESTINATION share/applications)
install(FILES resources/seadrop.svg DESTINATION share/icons/hicolor/scalable/apps)
