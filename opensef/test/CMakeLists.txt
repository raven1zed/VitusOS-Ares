# openSEF test programs

project(opensef-tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Hello Window - basic Wayland window
add_executable(hello-window
    hello-window.cpp
)

target_link_libraries(hello-window PRIVATE
    opensef-base
    opensef-ui
    opensef-appkit
    opensef-backend
)

# Welcome Screen - multilingual greeting
add_executable(welcome-screen
    welcome-screen.cpp
)

target_link_libraries(welcome-screen PRIVATE
    opensef-base
    opensef-ui
    opensef-backend
)

# Compile options
target_compile_options(hello-window PRIVATE -Wall -Wextra)
target_compile_options(welcome-screen PRIVATE -Wall -Wextra)
