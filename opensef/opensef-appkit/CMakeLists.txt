# opensef-appkit: GUI Widget Layer (C++ Version)

project(opensef-appkit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(opensef-appkit SHARED
    src/OSFView.cpp
    src/OSFWindow.cpp
    src/OSFButton.cpp
    src/OSFWidgets.cpp
    src/OSFWindowButton.cpp
    src/OSFTitleBar.cpp
    src/OSFTilingManager.cpp
)

target_include_directories(opensef-appkit PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(opensef-appkit PUBLIC
    opensef-base
    opensef-ui
)

target_compile_options(opensef-appkit PRIVATE
    -Wall -Wextra
)
