# VitusOS Terminal

project(vitus-terminal)

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
    opensef-ui
    opensef-backend
)

target_compile_options(vitus-terminal PRIVATE
    -Wall -Wextra
)

install(TARGETS vitus-terminal RUNTIME DESTINATION bin)
install(FILES resources/vitus-terminal.desktop DESTINATION share/applications)
