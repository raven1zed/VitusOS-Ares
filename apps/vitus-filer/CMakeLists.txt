# VitusOS Filer - File Manager

project(vitus-filer)

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
    opensef-ui
    opensef-backend
)

target_compile_options(vitus-filer PRIVATE
    -Wall -Wextra
)

install(TARGETS vitus-filer RUNTIME DESTINATION bin)
install(FILES resources/vitus-filer.desktop DESTINATION share/applications)
