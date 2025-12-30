/**
 * OSFDecorations.cpp - Server-Side Window Decorations
 *
 * Implements the Ares theme window chrome with:
 * - Lunar Gray title bar (#F0F0F0)
 * - Traffic lights: Orange/Gold/Blue
 * - 3px Space Orange accent on active windows
 */

#include "OSFDecorations.h"
#include "OSFCompositor.h"
#include "OSFView.h"

#include <algorithm>
#include <cmath>

namespace opensef {

using namespace AresColors;
using namespace AresDimensions;

OSFDecorations::OSFDecorations(OSFCompositor *compositor)
    : compositor_(compositor) {}

void OSFDecorations::render(OSFView *view, uint32_t *buffer, int width,
                            int height, bool focused) {
  (void)view;

  // 1. Draw shadow (if focused)
  if (focused) {
    drawShadow(buffer, width, height);
  }

  // 2. Draw window background (Soft Cream)
  fillRect(buffer, width, height, 0, TitleBarHeight, width,
           height - TitleBarHeight,
           makeARGB(255, SoftCream >> 16, (SoftCream >> 8) & 0xFF,
                    SoftCream & 0xFF));

  // 3. Draw title bar
  drawTitleBar(buffer, width, height, focused);

  // 4. Draw accent bar (only if focused)
  if (focused) {
    drawAccentBar(buffer, width, height);
  }

  // 5. Draw traffic lights
  drawTrafficLights(buffer, width, height);

  // 6. Draw border
  drawBorder(buffer, width, height);
}

void OSFDecorations::drawTitleBar(uint32_t *buffer, int w, int h,
                                  bool focused) {
  uint32_t color;
  if (focused) {
    // Lunar Gray with 90% opacity
    color = makeARGB(230, LunarGray >> 16, (LunarGray >> 8) & 0xFF,
                     LunarGray & 0xFF);
  } else {
    // Slightly more transparent when inactive
    color = makeARGB(200, LunarGray >> 16, (LunarGray >> 8) & 0xFF,
                     LunarGray & 0xFF);
  }

  // Draw title bar with rounded top corners (8px radius)
  int r = CornerRadius;

  // Main body (excluding corners)
  fillRect(buffer, w, h, r, 0, w - 2 * r, TitleBarHeight, color); // Center
  fillRect(buffer, w, h, 0, r, r, TitleBarHeight - r, color);     // Left strip
  fillRect(buffer, w, h, w - r, r, r, TitleBarHeight - r, color); // Right strip

  // Top-left corner
  for (int py = 0; py < r; py++) {
    for (int px = 0; px < r; px++) {
      float dist = std::sqrt((r - px) * (r - px) + (r - py) * (r - py));
      if (dist <= r) {
        if (px >= 0 && px < w && py >= 0 && py < h) {
          buffer[py * w + px] = color;
        }
      }
    }
  }

  // Top-right corner
  for (int py = 0; py < r; py++) {
    for (int px = 0; px < r; px++) {
      float dist = std::sqrt(px * px + (r - py) * (r - py));
      if (dist <= r) {
        int x = w - r + px;
        if (x >= 0 && x < w && py >= 0 && py < h) {
          buffer[py * w + x] = color;
        }
      }
    }
  }
}

void OSFDecorations::drawTrafficLights(uint32_t *buffer, int w, int h) {
  (void)h;

  // Vertical center of title bar
  int centerY = TitleBarHeight / 2;

  // Button positions (from left edge)
  int closeX = ButtonLeftOffset + AccentBarWidth + ButtonRadius;
  int minimizeX = closeX + ButtonDiameter + ButtonSpacing;
  int maximizeX = minimizeX + ButtonDiameter + ButtonSpacing;

  // Close (Space Orange)
  fillCircle(buffer, w, h, closeX, centerY, ButtonRadius,
             makeARGB(255, SpaceOrange >> 16, (SpaceOrange >> 8) & 0xFF,
                      SpaceOrange & 0xFF));

  // Minimize (Warm Gold)
  fillCircle(
      buffer, w, h, minimizeX, centerY, ButtonRadius,
      makeARGB(255, WarmGold >> 16, (WarmGold >> 8) & 0xFF, WarmGold & 0xFF));

  // Maximize (Mission Blue)
  fillCircle(buffer, w, h, maximizeX, centerY, ButtonRadius,
             makeARGB(255, MissionBlue >> 16, (MissionBlue >> 8) & 0xFF,
                      MissionBlue & 0xFF));
}

void OSFDecorations::drawAccentBar(uint32_t *buffer, int w, int h) {
  // 3px Space Orange accent on left edge
  fillRect(buffer, w, h, 0, 0, AccentBarWidth, TitleBarHeight,
           makeARGB(255, SpaceOrange >> 16, (SpaceOrange >> 8) & 0xFF,
                    SpaceOrange & 0xFF));
}

void OSFDecorations::drawShadow(uint32_t *buffer, int w, int h) {
  // Soft shadow around window (simplified - just darken edges)
  // Full shadow implementation would require pre-rendered shadow texture
  (void)buffer;
  (void)w;
  (void)h;
  // TODO: Implement soft shadow with blur
}

void OSFDecorations::drawBorder(uint32_t *buffer, int w, int h) {
  // Subtle border (1px)
  uint32_t borderColor =
      makeARGB(255, Borders >> 16, (Borders >> 8) & 0xFF, Borders & 0xFF);

  // Top (skip title bar area)
  fillRect(buffer, w, h, 0, TitleBarHeight, w, BorderWidth, borderColor);

  // Left (below title bar)
  fillRect(buffer, w, h, 0, TitleBarHeight, BorderWidth, h - TitleBarHeight,
           borderColor);

  // Right
  fillRect(buffer, w, h, w - BorderWidth, TitleBarHeight, BorderWidth,
           h - TitleBarHeight, borderColor);

  // Bottom
  fillRect(buffer, w, h, 0, h - BorderWidth, w, BorderWidth, borderColor);
}

OSFDecorations::Button OSFDecorations::buttonAt(int x, int y) {
  // Check if in title bar
  if (y < 0 || y >= TitleBarHeight) {
    return Button::None;
  }

  int centerY = TitleBarHeight / 2;
  int closeX = ButtonLeftOffset + AccentBarWidth + ButtonRadius;
  int minimizeX = closeX + ButtonDiameter + ButtonSpacing;
  int maximizeX = minimizeX + ButtonDiameter + ButtonSpacing;

  auto distance = [](int x1, int y1, int x2, int y2) -> float {
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
  };

  if (distance(x, y, closeX, centerY) <= ButtonRadius) {
    return Button::Close;
  }
  if (distance(x, y, minimizeX, centerY) <= ButtonRadius) {
    return Button::Minimize;
  }
  if (distance(x, y, maximizeX, centerY) <= ButtonRadius) {
    return Button::Maximize;
  }

  return Button::None;
}

// Drawing helpers

void OSFDecorations::fillRect(uint32_t *buf, int bw, int bh, int x, int y,
                              int w, int h, uint32_t color) {
  for (int py = y; py < y + h && py < bh; py++) {
    if (py < 0)
      continue;
    for (int px = x; px < x + w && px < bw; px++) {
      if (px < 0)
        continue;
      buf[py * bw + px] = color;
    }
  }
}

void OSFDecorations::fillCircle(uint32_t *buf, int bw, int bh, int cx, int cy,
                                int r, uint32_t color) {
  for (int y = -r; y <= r; y++) {
    for (int x = -r; x <= r; x++) {
      if (x * x + y * y <= r * r) {
        int px = cx + x;
        int py = cy + y;
        if (px >= 0 && px < bw && py >= 0 && py < bh) {
          buf[py * bw + px] = color;
        }
      }
    }
  }
}

void OSFDecorations::blendPixel(uint32_t *buf, int bw, int bh, int x, int y,
                                uint32_t color, float alpha) {
  if (x < 0 || x >= bw || y < 0 || y >= bh)
    return;

  uint32_t existing = buf[y * bw + x];

  uint8_t eA = (existing >> 24) & 0xFF;
  uint8_t eR = (existing >> 16) & 0xFF;
  uint8_t eG = (existing >> 8) & 0xFF;
  uint8_t eB = existing & 0xFF;

  uint8_t nR = (color >> 16) & 0xFF;
  uint8_t nG = (color >> 8) & 0xFF;
  uint8_t nB = color & 0xFF;

  uint8_t rR = static_cast<uint8_t>(eR * (1 - alpha) + nR * alpha);
  uint8_t rG = static_cast<uint8_t>(eG * (1 - alpha) + nG * alpha);
  uint8_t rB = static_cast<uint8_t>(eB * (1 - alpha) + nB * alpha);
  uint8_t rA = std::max(eA, static_cast<uint8_t>(alpha * 255));

  buf[y * bw + x] = (rA << 24) | (rR << 16) | (rG << 8) | rB;
}

uint32_t OSFDecorations::makeARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
  return (a << 24) | (r << 16) | (g << 8) | b;
}

uint32_t OSFDecorations::applyAlpha(uint32_t color, float alpha) {
  uint8_t a = static_cast<uint8_t>(((color >> 24) & 0xFF) * alpha);
  return (a << 24) | (color & 0x00FFFFFF);
}

} // namespace opensef
