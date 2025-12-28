# opensef-ui: Ares Design System
# Colors, typography, animations, themes

project(opensef-ui)

add_library(opensef-ui SHARED
    src/OSFTheme.m
    src/OSFColors.m
    src/OSFTypography.m
)

target_include_directories(opensef-ui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(opensef-ui PRIVATE
    -fobjc-runtime=gnustep-2.0
    -fobjc-arc
    -fblocks
)

target_link_libraries(opensef-ui PUBLIC
    opensef-base
)
