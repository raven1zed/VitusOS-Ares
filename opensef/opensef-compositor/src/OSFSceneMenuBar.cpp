/**
 * OSFSceneMenuBar.cpp - Scene-based Menu Bar Implementation
 */

#include "OSFSceneMenuBar.h"
#include "OSFCompositor.h"
#include "OSFDecorations.h"
#include "OSFDesktopLayers.h"


#include <iostream>

namespace opensef {

using namespace AresColors;

OSFSceneMenuBar::OSFSceneMenuBar(OSFCompositor *compositor,
                                 OSFDesktopLayers *layers)
    : compositor_(compositor), layers_(layers) {}

OSFSceneMenuBar::~OSFSceneMenuBar() {
  // Scene nodes cleaned up by scene tree
}

void OSFSceneMenuBar::create(int screenWidth) {
  screenWidth_ = screenWidth;

  // Menu bar background (semi-transparent gray)
  float bgColor[4] = {
      ((LunarGray >> 16) & 0xFF) / 255.0f, ((LunarGray >> 8) & 0xFF) / 255.0f,
      (LunarGray & 0xFF) / 255.0f,
      0.90f // 90% opacity
  };

  background_ =
      layers_->createRect(DesktopLayer::Overlay, screenWidth, Height, bgColor);
  if (background_) {
    wlr_scene_node_set_position(&background_->node, 0, 0);
  }

  // Multitask button (orange circle in top-left)
  float buttonColor[4] = {((SpaceOrange >> 16) & 0xFF) / 255.0f,
                          ((SpaceOrange >> 8) & 0xFF) / 255.0f,
                          (SpaceOrange & 0xFF) / 255.0f, 1.0f};

  // Note: wlr_scene_rect is a rectangle, not a circle
  // For now we use a square; proper circle would need custom rendering
  multitaskButton_ =
      layers_->createRect(DesktopLayer::Overlay, MultitaskButtonSize,
                          MultitaskButtonSize, buttonColor);
  if (multitaskButton_) {
    int buttonX = MultitaskButtonMargin;
    int buttonY = (Height - MultitaskButtonSize) / 2;
    wlr_scene_node_set_position(&multitaskButton_->node, buttonX, buttonY);
  }

  std::cout << "[openSEF] Menu bar created (width: " << screenWidth << "px)"
            << std::endl;
}

void OSFSceneMenuBar::resize(int screenWidth) {
  if (screenWidth_ == screenWidth) {
    return;
  }
  screenWidth_ = screenWidth;

  // Resize background
  if (background_) {
    wlr_scene_rect_set_size(background_, screenWidth, Height);
  }
}

} // namespace opensef
