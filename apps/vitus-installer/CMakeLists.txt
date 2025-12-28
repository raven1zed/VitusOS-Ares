# VitusOS Installer
# Modern, minimalist installation experience (macOS-style)

project(vitus-installer VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

add_executable(vitus-installer
    src/main.cpp
    src/InstallerApp.cpp
    src/WelcomePage.cpp
    src/DiskPage.cpp
    src/AccountPage.cpp
    src/CustomizePage.cpp
    src/InstallPage.cpp
)

target_include_directories(vitus-installer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(vitus-installer
    opensef-base
    opensef-appkit
    opensef-backend
    opensef-ui
)

# Install to live ISO
install(TARGETS vitus-installer DESTINATION /usr/bin)
