/**
 * OSFWidgets.cpp - Additional widget implementations
 */

#include <iostream>
#include <opensef/OpenSEFAppKit.h>
#include <opensef/VulkanTextRenderer.h>


namespace opensef {

// Global Vulkan state
VkCommandBuffer g_CurrentVulkanCommandBuffer = VK_NULL_HANDLE;

void OSFAppKitInitializeVulkan(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue queue, VkCommandPool commandPool, VkRenderPass renderPass) {
    VulkanTextRenderer::shared().initialize(device, physicalDevice, queue, commandPool, renderPass);
}

// ============================================================================
// OSFLabel
// ============================================================================

OSFLabel::OSFLabel() = default;

OSFLabel::OSFLabel(const std::string &text) : text_(text) {}

OSFLabel::~OSFLabel() = default;

void OSFLabel::draw() {
  if (hidden_)
    return;

  // Retrieve current command buffer from a context/renderer singleton
  // The application/backend must set g_CurrentVulkanCommandBuffer before calling draw()
  VkCommandBuffer cmd = g_CurrentVulkanCommandBuffer;

  if (cmd != VK_NULL_HANDLE) {
      glm::vec4 colorVec(textColor_.r, textColor_.g, textColor_.b, textColor_.a);

      // Calculate position relative to window or parent (simplified)
      float x = static_cast<float>(frame_.x);
      float y = static_cast<float>(frame_.y + fontSize_); // Baseline approx

      VulkanTextRenderer::shared().drawText(cmd, text_, x, y, 1.0f, colorVec);
  }

  OSFView::draw();
}

// ============================================================================
// OSFGlassPanel
// ============================================================================

OSFGlassPanel::OSFGlassPanel() = default;

OSFGlassPanel::~OSFGlassPanel() = default;

std::shared_ptr<OSFGlassPanel> OSFGlassPanel::create(const OSFRect &frame) {
  auto panel = std::make_shared<OSFGlassPanel>();
  panel->setFrame(frame);
  return panel;
}

void OSFGlassPanel::draw() {
  if (hidden_)
    return;

  // TODO: Vulkan blur effect rendering
  // 1. Sample background behind panel
  // 2. Apply Gaussian blur
  // 3. Overlay tint color

  OSFView::draw();
}

} // namespace opensef
