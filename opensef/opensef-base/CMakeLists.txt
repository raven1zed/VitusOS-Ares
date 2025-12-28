# opensef-base: Foundation layer (forked from gnustep-base)
# Core data types, networking, file I/O

project(opensef-base)

add_library(opensef-base SHARED
    src/OSFObject.m
    src/OSFString.m
    src/OSFArray.m
)

target_include_directories(opensef-base PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(opensef-base
    ${GNUSTEP_BASE_LIBRARIES}
)

# Install headers
install(DIRECTORY include/opensef
    DESTINATION include
)
