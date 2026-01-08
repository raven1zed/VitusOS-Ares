# openSEF test programs

project(opensef-tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Phase 1 validation (exercises all foundation classes)
add_executable(phase1-validation
    phase1_validation.cpp
)

target_link_libraries(phase1-validation PRIVATE
    opensef-base
)

# Phase 2 validation (windowing integration)
add_executable(phase2-window
    phase2_window.cpp
)

target_link_libraries(phase2-window PRIVATE
    opensef-base
)

# Compile options
target_compile_options(phase1-validation PRIVATE -Wall -Wextra)
target_compile_options(phase2-window PRIVATE -Wall -Wextra)
