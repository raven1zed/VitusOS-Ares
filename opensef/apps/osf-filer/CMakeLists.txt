add_executable(osf-filer src/main.cpp)

target_include_directories(osf-filer PRIVATE include)

target_link_libraries(osf-filer PRIVATE opensef-appkit opensef-base opensef-core)

install(TARGETS osf-filer DESTINATION bin)
