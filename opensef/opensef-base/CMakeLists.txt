# opensef-base: Foundation layer (forked from gnustep-base)
# Core data types, object system

project(opensef-base)

add_library(opensef-base SHARED
    src/OSFObject.m
    src/OSFString.m
    src/OSFArray.m
)

# GNUStep includes and flags
target_include_directories(opensef-base PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GNUSTEP_BASE_INCLUDE_DIRS}
)

target_compile_options(opensef-base PRIVATE
    ${GNUSTEP_BASE_CFLAGS}
    -fobjc-runtime=gnustep-2.0
    -fobjc-arc
    -fblocks
)

target_link_libraries(opensef-base PUBLIC
    ${GNUSTEP_BASE_LIBRARIES}
    objc  # libobjc2
)

# Export include directory for other components
set(OPENSEF_BASE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
