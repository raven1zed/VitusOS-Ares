add_executable(osf-terminal src/main.cpp)

target_include_directories(osf-terminal PRIVATE include)

# Link AppKit and Base
target_link_libraries(osf-terminal PRIVATE opensef-appkit opensef-base opensef-framework opensef-core)

install(TARGETS osf-terminal DESTINATION bin)
