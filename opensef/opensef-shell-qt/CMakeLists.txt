# opensef-shell-qt - Qt Quick Shell UI
# GPU-accelerated shell with Vulkan/OpenGL rendering

cmake_minimum_required(VERSION 3.16)
project(opensef-shell-qt VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Gui 
    Quick 
    QuickControls2
    DBus
    WaylandClient
)

# Source files
set(SOURCES
    src/main.cpp
    src/PanelController.cpp
    src/DockController.cpp
    src/MultitaskController.cpp
    src/SystemTrayController.cpp
    src/DBusMenuImporter.cpp
)

set(HEADERS
    src/PanelController.h
    src/DockController.h
    src/MultitaskController.h
    src/SystemTrayController.h
    src/DBusMenuImporter.h
)

# QML resources
qt_add_resources(QML_RESOURCES resources.qrc)

# Executable
add_executable(osf-shell-qt
    ${SOURCES}
    ${HEADERS}
    ${QML_RESOURCES}
)

# Include paths
target_include_directories(osf-shell-qt PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/opensef-framework/include
    ${CMAKE_SOURCE_DIR}/opensef-base/include
)

# Link libraries
target_link_libraries(osf-shell-qt PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::DBus
    Qt6::WaylandClient
    opensef-framework
    opensef-base
)

# Install
install(TARGETS osf-shell-qt
    RUNTIME DESTINATION bin
)
