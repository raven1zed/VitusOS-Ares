# VitusOS Installer

project(vitus-installer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(vitus-installer
    src/main.cpp
    src/InstallerApp.cpp
)

target_include_directories(vitus-installer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(vitus-installer PRIVATE
    opensef-base
    opensef-ui
    opensef-backend
)

target_compile_options(vitus-installer PRIVATE
    -Wall -Wextra
)

install(TARGETS vitus-installer RUNTIME DESTINATION bin)
