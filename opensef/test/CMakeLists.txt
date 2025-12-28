# openSEF Tests

add_executable(hello-window
    hello-window.m
)

target_compile_options(hello-window PRIVATE
    -fobjc-runtime=gnustep-2.0
    -fobjc-arc
    -fblocks
)

target_include_directories(hello-window PRIVATE
    ${CMAKE_SOURCE_DIR}/opensef/include
    ${CMAKE_SOURCE_DIR}/opensef/opensef-base/include
    ${CMAKE_SOURCE_DIR}/opensef/opensef-appkit/include
    ${CMAKE_SOURCE_DIR}/opensef/opensef-backend/include
    ${CMAKE_SOURCE_DIR}/opensef/opensef-ui/include
)

target_link_libraries(hello-window
    opensef-base
    opensef-appkit
    opensef-backend
    opensef-ui
)
