# SeaDrop - Clipboard sync for VitusOS

project(seadrop)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(seadrop
    src/main.cpp
    src/SeaDropApp.cpp
    src/SeaDropWindow.cpp
    src/SeaDropTray.cpp
)

target_include_directories(seadrop PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(seadrop PRIVATE
    opensef-base
    opensef-ui
    opensef-appkit
    opensef-backend
)

target_compile_options(seadrop PRIVATE
    -Wall -Wextra
)

# Install
install(TARGETS seadrop RUNTIME DESTINATION bin)
install(FILES resources/seadrop.desktop DESTINATION share/applications)
