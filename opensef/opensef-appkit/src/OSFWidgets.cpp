/**
 * OSFWidgets.cpp - Additional widget implementations
 */

#include <iostream>
#include <opensef/OpenSEFAppKit.h>


namespace opensef {

// ============================================================================
// OSFLabel
// ============================================================================

OSFLabel::OSFLabel() = default;

OSFLabel::OSFLabel(const std::string &text) : text_(text) {}

OSFLabel::~OSFLabel() = default;

void OSFLabel::draw() {
  if (hidden_)
    return;

  // TODO: Vulkan text rendering

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
