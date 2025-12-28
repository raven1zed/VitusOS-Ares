# opensef-appkit: GUI Widget Layer (forked from gnustep-gui)
# Windows, views, buttons, text fields

project(opensef-appkit)

add_library(opensef-appkit SHARED
    src/OSFWindow.m
    src/OSFView.m
    src/OSFButton.m
    src/OSFMenu.m
    src/OSFWidgets.m
)

target_include_directories(opensef-appkit PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(opensef-appkit PRIVATE
    -fobjc-runtime=gnustep-2.0
    -fobjc-arc
    -fblocks
)

# Link order: base first, then backend
target_link_libraries(opensef-appkit PUBLIC
    opensef-base
    opensef-backend
)
