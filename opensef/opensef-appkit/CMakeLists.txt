# opensef-appkit: GUI Widget Layer (forked from gnustep-gui)
# Windows, views, buttons, text fields

project(opensef-appkit)

add_library(opensef-appkit SHARED
    src/OSFWindow.m
    src/OSFView.m
    src/OSFButton.m
)

target_include_directories(opensef-appkit PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(opensef-appkit
    opensef-base
    opensef-backend
)
