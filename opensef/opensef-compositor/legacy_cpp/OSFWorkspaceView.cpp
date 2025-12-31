/**
 * OSFWorkspaceView.cpp - Multitask/Workspace Switcher
 */

#include "OSFWorkspaceView.h"
#include "OSFCompositor.h"
#include "OSFDecorations.h"
#include "OSFView.h"


#include <algorithm>

namespace opensef {

using namespace AresColors;

OSFWorkspaceView::OSFWorkspaceView(OSFCompositor *compositor)
    : compositor_(compositor) {}

void OSFWorkspaceView::show() { visible_ = true; }

void OSFWorkspaceView::hide() { visible_ = false; }

void OSFWorkspaceView::tick() {
  // Animate visibility
  float target = visible_ ? 1.0f : 0.0f;
  animProgress_ += (target - animProgress_) * 0.15f; // Smooth animation

  if (animProgress_ < 0.01f)
    animProgress_ = 0.0f;
  if (animProgress_ > 0.99f)
    animProgress_ = 1.0f;
}

void OSFWorkspaceView::render(uint32_t *buffer, int screenW, int screenH) {
  if (animProgress_ < 0.01f)
    return; // Not visible

  drawOverlay(buffer, screenW, screenH);
  drawWorkspaceStrip(buffer, screenW, screenH);
  drawWindowThumbnails(buffer, screenW, screenH);
}

void OSFWorkspaceView::drawOverlay(uint32_t *buf, int sw, int sh) {
  // Semi-transparent dark overlay
  uint8_t overlayAlpha = static_cast<uint8_t>(180 * animProgress_);
  uint32_t overlayColor = (overlayAlpha << 24) | 0x1A1A2E; // Dark blue-black

  for (int y = 0; y < sh; y++) {
    for (int x = 0; x < sw; x++) {
      // Blend with existing
      uint32_t existing = buf[y * sw + x];
      uint8_t eR = (existing >> 16) & 0xFF;
      uint8_t eG = (existing >> 8) & 0xFF;
      uint8_t eB = existing & 0xFF;

      uint8_t oR = (overlayColor >> 16) & 0xFF;
      uint8_t oG = (overlayColor >> 8) & 0xFF;
      uint8_t oB = overlayColor & 0xFF;

      float a = overlayAlpha / 255.0f;
      uint8_t rR = static_cast<uint8_t>(eR * (1 - a) + oR * a);
      uint8_t rG = static_cast<uint8_t>(eG * (1 - a) + oG * a);
      uint8_t rB = static_cast<uint8_t>(eB * (1 - a) + oB * a);

      buf[y * sw + x] = 0xFF000000 | (rR << 16) | (rG << 8) | rB;
    }
  }
}

void OSFWorkspaceView::drawWorkspaceStrip(uint32_t *buf, int sw, int sh) {
  // Top strip showing all workspaces
  int stripHeight = 60;
  int stripY = 20;

  // Slight lighter background for strip area
  uint32_t stripColor = 0x40FFFFFF;
  for (int y = stripY; y < stripY + stripHeight && y < sh; y++) {
    for (int x = 0; x < sw; x++) {
      uint32_t existing = buf[y * sw + x];
      uint8_t eR = (existing >> 16) & 0xFF;
      uint8_t eG = (existing >> 8) & 0xFF;
      uint8_t eB = existing & 0xFF;

      // Lighten slightly
      uint8_t rR = std::min(255, eR + 20);
      uint8_t rG = std::min(255, eG + 20);
      uint8_t rB = std::min(255, eB + 20);

      buf[y * sw + x] = 0xFF000000 | (rR << 16) | (rG << 8) | rB;
    }
  }

  // Draw workspace thumbnails (3 workspaces for now)
  int workspaceCount = 3;
  int thumbWidth = 120;
  int thumbHeight = 40;
  int spacing = 20;
  int totalWidth = workspaceCount * thumbWidth + (workspaceCount - 1) * spacing;
  int startX = (sw - totalWidth) / 2;

  for (int i = 0; i < workspaceCount; i++) {
    int x = startX + i * (thumbWidth + spacing);
    int y = stripY + (stripHeight - thumbHeight) / 2;

    // Workspace thumbnail background
    uint32_t thumbColor = (i == 0) ? (0xFF000000 | SpaceOrange) : 0xFF404040;

    for (int ty = 0; ty < thumbHeight; ty++) {
      for (int tx = 0; tx < thumbWidth; tx++) {
        int px = x + tx;
        int py = y + ty;
        if (px >= 0 && px < sw && py >= 0 && py < sh) {
          buf[py * sw + px] = thumbColor;
        }
      }
    }
  }
}

void OSFWorkspaceView::drawWindowThumbnails(uint32_t *buf, int sw, int sh) {
  // TODO: Draw thumbnails of all windows in current workspace
  // For now, just show placeholder
  (void)buf;
  (void)sw;
  (void)sh;
}

bool OSFWorkspaceView::handleClick(int x, int y) {
  if (!visible_)
    return false;

  // Check if clicked on overlay (not on a window) -> hide
  // TODO: Implement proper hit testing
  (void)x;
  (void)y;

  hide();
  return true;
}

void OSFWorkspaceView::updateHover(int x, int y) {
  (void)x;
  (void)y;
  // TODO: Track which window/workspace is hovered
}

} // namespace opensef
