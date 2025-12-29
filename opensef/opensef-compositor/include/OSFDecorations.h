/**
 * OSFDecorations.h - Server-Side Window Decorations
 *
 * Implements the Ares theme window chrome:
 * - 32px title bar with Lunar Gray background
 * - Traffic lights: Orange/Gold/Blue (12px diameter)
 * - 3px Space Orange accent bar on active windows
 * - Soft shadows for depth
 */

#pragma once

#include <cstdint>

namespace opensef {

// Ares theme colors (from Design Reference)
namespace AresColors {
// Primary (Mars Mission)
constexpr uint32_t SpaceOrange = 0xE85D04; // Close button, accent
constexpr uint32_t MarsDust = 0xFB8500;
constexpr uint32_t MissionBlue = 0x3D5A80; // Maximize button
constexpr uint32_t LunarGray = 0xF0F0F0;   // Title bar

// OS1-Inspired Warmth
constexpr uint32_t DeepBurgundy = 0x621212;
constexpr uint32_t WarmGold = 0xC3BC19; // Minimize button
constexpr uint32_t CoolTeal = 0x389693;
constexpr uint32_t SoftCream = 0xFBFBFB; // Background
constexpr uint32_t LavenderMist = 0xEFEBF7;

// Text & UI
constexpr uint32_t TextPrimary = 0x2D2D2D;
constexpr uint32_t TextSecondary = 0x6B6B6B;
constexpr uint32_t TextTertiary = 0x9E9E9E;
constexpr uint32_t Borders = 0xE0DDD9;
} // namespace AresColors

// Decoration dimensions (from Design Reference)
namespace AresDimensions {
constexpr int TitleBarHeight = 32;  // Standard
constexpr int TitleBarCompact = 28; // Compact
constexpr int AccentBarWidth = 3;   // Left edge accent

// Traffic lights
constexpr int ButtonDiameter = 12;
constexpr int ButtonRadius = 6;
constexpr int ButtonSpacing = 8;    // Between buttons
constexpr int ButtonLeftOffset = 8; // From left edge

// Window frame
constexpr int BorderWidth = 1;
constexpr int CornerRadius = 8;
constexpr int ShadowRadius = 20;
} // namespace AresDimensions

// Forward declarations
class OSFView;
class OSFCompositor;

/**
 * OSFDecorations - Renders window decorations
 */
class OSFDecorations {
public:
  OSFDecorations(OSFCompositor *compositor);

  // Render decorations for a view
  void render(OSFView *view, uint32_t *buffer, int width, int height,
              bool focused);

  // Draw components
  void drawTitleBar(uint32_t *buffer, int w, int h, bool focused);
  void drawTrafficLights(uint32_t *buffer, int w, int h);
  void drawAccentBar(uint32_t *buffer, int w, int h);
  void drawShadow(uint32_t *buffer, int w, int h);
  void drawBorder(uint32_t *buffer, int w, int h);

  // Hit testing for traffic lights
  enum class Button { None, Close, Minimize, Maximize };
  Button buttonAt(int x, int y);

private:
  OSFCompositor *compositor_;

  // Drawing helpers
  void fillRect(uint32_t *buf, int bw, int bh, int x, int y, int w, int h,
                uint32_t color);
  void fillCircle(uint32_t *buf, int bw, int bh, int cx, int cy, int r,
                  uint32_t color);
  void blendPixel(uint32_t *buf, int bw, int bh, int x, int y, uint32_t color,
                  float alpha);

  uint32_t makeARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
  uint32_t applyAlpha(uint32_t color, float alpha);
};

} // namespace opensef
