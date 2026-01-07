add_executable(osf-settings src/main.cpp)

target_include_directories(osf-settings PRIVATE include)

target_link_libraries(osf-settings PRIVATE opensef-appkit opensef-base opensef-framework opensef-core)

install(TARGETS osf-settings DESTINATION bin)
