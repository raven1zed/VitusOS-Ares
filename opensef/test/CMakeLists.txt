# openSEF Tests

add_executable(hello-window
    hello-window.m
)

target_link_libraries(hello-window
    opensef-base
    opensef-appkit
    opensef-backend
    opensef-ui
)
