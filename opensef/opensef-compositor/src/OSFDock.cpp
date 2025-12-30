/**
 * OSFDock.cpp - VitusOS Dock Implementation
 */

#include "OSFDock.h"
#include "OSFCompositor.h"
#include "OSFDecorations.h"

#include <algorithm>
#include <cmath>

namespace opensef {

using namespace AresDock;
using namespace AresColors;

OSFDock::OSFDock(OSFCompositor *compositor) : compositor_(compositor) {

  // Add default items
  items_.push_back({"finder", "Files", "", false, false, 1.0f});
  items_.push_back({"terminal", "Terminal", "", false, false, 1.0f});
  items_.push_back({"browser", "Firefox", "", false, false, 1.0f});
  items_.push_back({"settings", "Settings", "", false, false, 1.0f});
}

void OSFDock::render(uint32_t *buffer, int screenW, int screenH) {
  calculateGeometry(screenW, screenH);
  drawBackground(buffer, screenW, screenH);

  // Draw each item
  int itemX = dockX_ + Padding;
  for (size_t i = 0; i < items_.size(); i++) {
    auto &item = items_[i];

    // Apply hover magnification
    float targetScale =
        (static_cast<int>(i) == hoveredIndex_) ? MagnifyScale : 1.0f;
    item.scale += (targetScale - item.scale) * 0.2f; // Smooth animation

    int scaledSize = static_cast<int>(IconSize * item.scale);
    int offsetY = static_cast<int>((scaledSize - IconSize) / 2);

    drawItem(buffer, screenW, screenH, item,
             itemX + (IconSize - scaledSize) / 2,
             dockY_ + (Height - scaledSize) / 2 - offsetY);

    // Draw running indicator
    if (item.isRunning) {
      drawIndicator(buffer, screenW, screenH, itemX + IconSize / 2,
                    dockY_ + Height - IndicatorSize - 4);
    }

    itemX += IconSize + IconSpacing;
  }
}

void OSFDock::drawBackground(uint32_t *buf, int sw, int sh) {
  // Semi-transparent dock background (Lunar Gray with transparency)
  uint32_t dockColor = (0xE0 << 24) | LunarGray; // 88% opacity

  // Draw rounded rectangle
  for (int y = dockY_; y < dockY_ + Height && y < sh; y++) {
    for (int x = dockX_; x < dockX_ + dockWidth_ && x < sw; x++) {
      // Check if in rounded corners
      int cornerX = 0, cornerY = 0;
      bool inCorner = false;

      if (x < dockX_ + CornerRadius && y < dockY_ + CornerRadius) {
        cornerX = dockX_ + CornerRadius;
        cornerY = dockY_ + CornerRadius;
        inCorner = true;
      } else if (x >= dockX_ + dockWidth_ - CornerRadius &&
                 y < dockY_ + CornerRadius) {
        cornerX = dockX_ + dockWidth_ - CornerRadius;
        cornerY = dockY_ + CornerRadius;
        inCorner = true;
      } else if (x < dockX_ + CornerRadius &&
                 y >= dockY_ + Height - CornerRadius) {
        cornerX = dockX_ + CornerRadius;
        cornerY = dockY_ + Height - CornerRadius;
        inCorner = true;
      } else if (x >= dockX_ + dockWidth_ - CornerRadius &&
                 y >= dockY_ + Height - CornerRadius) {
        cornerX = dockX_ + dockWidth_ - CornerRadius;
        cornerY = dockY_ + Height - CornerRadius;
        inCorner = true;
      }

      if (inCorner) {
        float dist = std::sqrt((x - cornerX) * (x - cornerX) +
                               (y - cornerY) * (y - cornerY));
        if (dist > CornerRadius)
          continue;
      }

      if (x >= 0 && x < sw && y >= 0 && y < sh) {
        buf[y * sw + x] = dockColor;
      }
    }
  }
}

void OSFDock::drawItem(uint32_t *buf, int sw, int sh, const DockItem &item,
                       int x, int y) {
  // Draw icon placeholder (colored square for now, will be replaced with actual
  // icons)
  int size = static_cast<int>(IconSize * item.scale);

  // Different color per app type
  uint32_t iconColor = 0xFF000000 | MissionBlue; // Default blue
  if (item.appId == "terminal")
    iconColor = 0xFF000000 | 0x1E1E1E;
  if (item.appId == "browser")
    iconColor = 0xFF000000 | SpaceOrange;
  if (item.appId == "settings")
    iconColor = 0xFF000000 | 0x6B6B6B;

  // Draw rounded icon
  int iconRadius = 8;
  for (int iy = 0; iy < size; iy++) {
    for (int ix = 0; ix < size; ix++) {
      int px = x + ix;
      int py = y + iy;

      // Rounded corners
      bool draw = true;
      if (ix < iconRadius && iy < iconRadius) {
        float d = std::sqrt((ix - iconRadius) * (ix - iconRadius) +
                            (iy - iconRadius) * (iy - iconRadius));
        if (d > iconRadius)
          draw = false;
      }
      if (ix >= size - iconRadius && iy < iconRadius) {
        float d =
            std::sqrt((ix - (size - iconRadius)) * (ix - (size - iconRadius)) +
                      (iy - iconRadius) * (iy - iconRadius));
        if (d > iconRadius)
          draw = false;
      }
      if (ix < iconRadius && iy >= size - iconRadius) {
        float d =
            std::sqrt((ix - iconRadius) * (ix - iconRadius) +
                      (iy - (size - iconRadius)) * (iy - (size - iconRadius)));
        if (d > iconRadius)
          draw = false;
      }
      if (ix >= size - iconRadius && iy >= size - iconRadius) {
        float d =
            std::sqrt((ix - (size - iconRadius)) * (ix - (size - iconRadius)) +
                      (iy - (size - iconRadius)) * (iy - (size - iconRadius)));
        if (d > iconRadius)
          draw = false;
      }

      if (draw && px >= 0 && px < sw && py >= 0 && py < sh) {
        buf[py * sw + px] = iconColor;
      }
    }
  }

  (void)item; // Name not used yet
}

void OSFDock::drawIndicator(uint32_t *buf, int sw, int sh, int x, int y) {
  // Small dot below icon
  uint32_t color = 0xFF000000 | SpaceOrange;

  for (int iy = -IndicatorSize / 2; iy <= IndicatorSize / 2; iy++) {
    for (int ix = -IndicatorSize / 2; ix <= IndicatorSize / 2; ix++) {
      if (ix * ix + iy * iy <= (IndicatorSize / 2) * (IndicatorSize / 2)) {
        int px = x + ix;
        int py = y + iy;
        if (px >= 0 && px < sw && py >= 0 && py < sh) {
          buf[py * sw + px] = color;
        }
      }
    }
  }
}

void OSFDock::calculateGeometry(int screenW, int screenH) {
  // Calculate dock width based on items
  dockWidth_ = Padding * 2 +
               static_cast<int>(items_.size()) * (IconSize + IconSpacing) -
               IconSpacing;

  // Center horizontally, position at bottom
  dockX_ = (screenW - dockWidth_) / 2;
  dockY_ = screenH - Height - BottomMargin;
}

void OSFDock::updateHover(int cursorX, int cursorY) {
  hoveredIndex_ = itemAtPosition(cursorX, cursorY);
}

bool OSFDock::handleClick(int x, int y) {
  int index = itemAtPosition(x, y);
  if (index >= 0 && index < static_cast<int>(items_.size())) {
    if (onItemClicked) {
      onItemClicked(items_[index].appId);
    }
    return true;
  }
  return false;
}

int OSFDock::itemAtPosition(int x, int y) {
  if (y < dockY_ || y >= dockY_ + Height)
    return -1;
  if (x < dockX_ + Padding || x >= dockX_ + dockWidth_ - Padding)
    return -1;

  int relX = x - dockX_ - Padding;
  int index = relX / (IconSize + IconSpacing);

  if (index >= 0 && index < static_cast<int>(items_.size())) {
    return index;
  }
  return -1;
}

void OSFDock::addItem(const DockItem &item) { items_.push_back(item); }

void OSFDock::removeItem(const std::string &appId) {
  items_.erase(
      std::remove_if(items_.begin(), items_.end(),
                     [&](const DockItem &i) { return i.appId == appId; }),
      items_.end());
}

void OSFDock::setRunning(const std::string &appId, bool running) {
  for (auto &item : items_) {
    if (item.appId == appId) {
      item.isRunning = running;
      break;
    }
  }
}

void OSFDock::setFocused(const std::string &appId, bool focused) {
  for (auto &item : items_) {
    item.isFocused = (item.appId == appId) && focused;
  }
}

} // namespace opensef
