# VitusOS Filer - File Manager

project(vitus-filer VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(vitus-filer
    src/main.cpp
    src/FilerApp.cpp
)

target_include_directories(vitus-filer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(vitus-filer PRIVATE
    opensef-base
    opensef-appkit
    opensef-backend
    opensef-ui
)

install(TARGETS vitus-filer DESTINATION bin)
install(FILES resources/vitus-filer.desktop DESTINATION share/applications)
