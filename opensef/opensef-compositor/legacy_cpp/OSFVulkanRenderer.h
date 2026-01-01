#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <memory>

namespace opensef {

struct OSFRect;
struct OSFColor;
struct OSFSize;

class OSFVulkanRenderer {
public:
    static OSFVulkanRenderer &shared();

    bool initialize();
    void shutdown();

    void beginFrame();
    void endFrame();

    void drawRect(const OSFRect &rect, const OSFColor &color);
    void drawRoundedRect(const OSFRect &rect, float radius, const OSFColor &color);
    void drawBlur(const OSFRect &rect, float radius);

private:
    OSFVulkanRenderer() = default;
    ~OSFVulkanRenderer() = default;

    OSFVulkanRenderer(const OSFVulkanRenderer&) = delete;
    OSFVulkanRenderer& operator=(const OSFVulkanRenderer&) = delete;

    VkInstance instance_ = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
    VkDevice device_ = VK_NULL_HANDLE;
    VkQueue graphicsQueue_ = VK_NULL_HANDLE;
    uint32_t graphicsQueueFamilyIndex_ = 0;

    bool available_ = false;
};

} // namespace opensef
