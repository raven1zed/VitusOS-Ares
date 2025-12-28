# opensef-backend: Display Backend (Wayland-only)
# Surface management, input handling, Vulkan rendering
# NOTE: Built before appkit (no circular dependency)

project(opensef-backend)

add_library(opensef-backend SHARED
    src/OSFBackend.m
    src/OSFWaylandSurface.m
    src/OSFVulkanRenderer.m
)

target_include_directories(opensef-backend PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${WAYLAND_INCLUDE_DIRS}
    ${Vulkan_INCLUDE_DIRS}
)

target_compile_options(opensef-backend PRIVATE
    -fobjc-runtime=gnustep-2.0
    -fobjc-arc
    -fblocks
)

target_link_libraries(opensef-backend PUBLIC
    opensef-base
    ${WAYLAND_LIBRARIES}
    Vulkan::Vulkan
)
