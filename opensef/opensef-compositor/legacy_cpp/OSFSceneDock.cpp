/**
 * OSFSceneDock.cpp - Scene-based Dock Implementation
 */

#include "OSFSceneDock.h"
#include "OSFCompositor.h"
#include "OSFDecorations.h"
#include "OSFDesktopLayers.h"


#include <iostream>

namespace opensef {

using namespace AresColors;

OSFSceneDock::OSFSceneDock(OSFCompositor *compositor, OSFDesktopLayers *layers)
    : compositor_(compositor), layers_(layers) {}

OSFSceneDock::~OSFSceneDock() {
  // Scene nodes cleaned up by scene tree
}

void OSFSceneDock::create(int screenWidth, int screenHeight) {
  screenWidth_ = screenWidth;
  screenHeight_ = screenHeight;

  createDefaultItems();

  // Calculate dock dimensions
  int itemCount = static_cast<int>(items_.size());
  int dockWidth =
      Padding * 2 + itemCount * IconSize + (itemCount - 1) * IconSpacing;
  int dockX = (screenWidth - dockWidth) / 2;
  int dockY = screenHeight - Height - BottomMargin;

  // Create dock background (semi-transparent gray)
  float bgColor[4] = {
      ((LunarGray >> 16) & 0xFF) / 255.0f, ((LunarGray >> 8) & 0xFF) / 255.0f,
      (LunarGray & 0xFF) / 255.0f,
      0.85f // 85% opacity
  };

  background_ =
      layers_->createRect(DesktopLayer::Top, dockWidth, Height, bgColor);
  if (background_) {
    wlr_scene_node_set_position(&background_->node, dockX, dockY);
  }

  // Create icon placeholders
  int iconX = dockX + Padding;
  int iconY = dockY + (Height - IconSize) / 2;

  // Icon colors (placeholder - would be app icons)
  uint32_t iconColors[] = {
      MissionBlue, // Files
      0x1E1E1E,    // Terminal
      SpaceOrange, // Browser
      0x6B6B6B     // Settings
  };

  for (size_t i = 0; i < items_.size() && i < 4; i++) {
    float iconColor[4] = {((iconColors[i] >> 16) & 0xFF) / 255.0f,
                          ((iconColors[i] >> 8) & 0xFF) / 255.0f,
                          (iconColors[i] & 0xFF) / 255.0f, 1.0f};

    items_[i].iconRect =
        layers_->createRect(DesktopLayer::Top, IconSize, IconSize, iconColor);
    if (items_[i].iconRect) {
      wlr_scene_node_set_position(&items_[i].iconRect->node, iconX, iconY);
    }

    // Running indicator (small orange dot)
    if (items_[i].isRunning) {
      float indicatorColor[4] = {((SpaceOrange >> 16) & 0xFF) / 255.0f,
                                 ((SpaceOrange >> 8) & 0xFF) / 255.0f,
                                 (SpaceOrange & 0xFF) / 255.0f, 1.0f};

      items_[i].indicatorRect =
          layers_->createRect(DesktopLayer::Top, 6, 6, indicatorColor);
      if (items_[i].indicatorRect) {
        wlr_scene_node_set_position(&items_[i].indicatorRect->node,
                                    iconX + IconSize / 2 - 3,
                                    dockY + Height - 10);
      }
    }

    iconX += IconSize + IconSpacing;
  }

  std::cout << "[openSEF] Dock created with " << items_.size() << " items"
            << std::endl;
}

void OSFSceneDock::resize(int screenWidth, int screenHeight) {
  if (screenWidth_ == screenWidth && screenHeight_ == screenHeight) {
    return;
  }

  // For now, just recreate
  // TODO: Proper repositioning
  screenWidth_ = screenWidth;
  screenHeight_ = screenHeight;
}

void OSFSceneDock::setAppRunning(const std::string &appId, bool running) {
  for (auto &item : items_) {
    if (item.appId == appId) {
      item.isRunning = running;
      break;
    }
  }
}

void OSFSceneDock::createDefaultItems() {
  items_.clear();
  items_.push_back({"finder", "Files", false, nullptr, nullptr});
  items_.push_back({"terminal", "Terminal", true, nullptr, nullptr}); // Running
  items_.push_back({"browser", "Firefox", false, nullptr, nullptr});
  items_.push_back({"settings", "Settings", false, nullptr, nullptr});
}

} // namespace opensef
