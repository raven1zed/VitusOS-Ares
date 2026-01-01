# opensef-appkit: Production GUI Widget Library
# Cairo/Pango-based widgets for VitusOS Ares

cmake_minimum_required(VERSION 3.16)
project(opensef-appkit VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(PANGO REQUIRED pangocairo)

find_package(Vulkan REQUIRED)
find_package(Freetype REQUIRED)
find_package(glm REQUIRED)

add_library(opensef-appkit STATIC
    src/OSFView.cpp
    src/OSFButton.cpp
    src/OSFLabel.cpp
    src/OSFTextField.cpp
    src/OSFGlassPanel.cpp
    src/OSFWidgets.cpp
    src/VulkanTextRenderer.cpp
    src/OSFTitleBar.cpp
    src/OSFWindowButton.cpp
)

target_include_directories(opensef-appkit PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CAIRO_INCLUDE_DIRS}
    ${PANGO_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
)

target_link_libraries(opensef-appkit PUBLIC
    opensef-base
    opensef-core
    Vulkan::Vulkan
    ${CAIRO_LIBRARIES}
    ${PANGO_LIBRARIES}
    Vulkan::Vulkan
    Freetype::Freetype
    glm::glm
)

target_compile_options(opensef-appkit PRIVATE
    -Wall -Wextra -Wno-unused-parameter
)

install(TARGETS opensef-appkit DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
