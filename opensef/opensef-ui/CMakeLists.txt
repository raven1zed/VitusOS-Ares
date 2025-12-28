# opensef-ui: OS1 Design System
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

target_link_libraries(opensef-ui
    opensef-base
)
