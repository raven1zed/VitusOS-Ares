/**
 * OSFWidgets.cpp - Stubbed for Build
 */

#include <array>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

// #include <vulkan/vulkan.h>
#include <opensef/OpenSEFAppKit.h>
#include <opensef/VulkanTextRenderer.h> // Header still needed for types if referenced

namespace opensef {

// Global Vulkan state
// VkCommandBuffer g_CurrentVulkanCommandBuffer = VK_NULL_HANDLE;

void OSFAppKitInitializeVulkan(VkDevice device, VkPhysicalDevice physicalDevice,
                               VkQueue queue, VkCommandPool commandPool,
                               VkRenderPass renderPass) {
  // VulkanTextRenderer::shared().initialize(device, physicalDevice, queue,
  // commandPool, renderPass);
}

// ============================================================================
// OSFLabel
// ============================================================================

void OSFLabel::draw() {
  if (hidden_)
    return;

  // Vulkan Text Rendering stubbed out
  /*
  VkCommandBuffer cmd = g_CurrentVulkanCommandBuffer;

  if (cmd != VK_NULL_HANDLE) {
      glm::vec4 colorVec(textColor_.r, textColor_.g, textColor_.b,
  textColor_.a); float x = static_cast<float>(frame_.x); float y =
  static_cast<float>(frame_.y + fontSize_);
      VulkanTextRenderer::shared().drawText(cmd, text_, x, y, 1.0f, colorVec);
  }
  */

  OSFView::draw();
}

// ============================================================================
// OSFGlassPanel
// ============================================================================

void OSFGlassPanel::draw() {
  if (hidden_)
    return;

  // Vulkan Blur stubbed out
  OSFView::draw();
}

} // namespace opensef
