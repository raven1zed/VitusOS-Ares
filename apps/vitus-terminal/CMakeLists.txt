# VitusOS Terminal

project(vitus-terminal VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(vitus-terminal
    src/main.cpp
    src/TerminalApp.cpp
)

target_include_directories(vitus-terminal PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(vitus-terminal PRIVATE
    opensef-base
    opensef-appkit
    opensef-backend
    opensef-ui
)

install(TARGETS vitus-terminal DESTINATION bin)
install(FILES resources/vitus-terminal.desktop DESTINATION share/applications)
