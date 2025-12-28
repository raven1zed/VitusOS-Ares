# openSEF hello-window test

project(hello-window)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hello-window
    hello-window.cpp
)

target_link_libraries(hello-window PRIVATE
    opensef-base
    opensef-ui
    opensef-appkit
    opensef-backend
)

target_compile_options(hello-window PRIVATE
    -Wall -Wextra
)
