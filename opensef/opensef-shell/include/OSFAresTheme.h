/**
 * OSFAresTheme.h - VitusOS Ares Design System
 *
 * Colors, dimensions, and styling constants for the Ares desktop.
 * Based on Mars-inspired warm palette with modern glassmorphism.
 */

#ifndef OSF_ARES_THEME_H
#define OSF_ARES_THEME_H

#define _USE_MATH_DEFINES
#include <cairo.h>
#include <cmath>
#include <cstdint>

namespace opensef {

namespace AresTheme {

// ==========================================================================
// Color Palette (ARGB format)
// ==========================================================================

// Mars-inspired warm colors
// Mars-inspired warm colors
constexpr uint32_t MarsOrange = 0xFFE57C3A; // Primary accent
constexpr uint32_t MarsGold = 0xFFD4A93E;   // Secondary accent
constexpr uint32_t MarsSand = 0xFFB5651D;   // Desert sand
constexpr uint32_t MarsRed = 0xFFC44536;    // Rust red

// Neutral colors (Light Mode)
constexpr uint32_t LunarGray = 0xFFF5F5F5; // Title bar, panels (Light)
constexpr uint32_t DeepSpace = 0xFFFFFFFF; // Window backgrounds (White)
constexpr uint32_t Nebula = 0xFFE0E0E0;    // Hover states (Light Gray)
constexpr uint32_t StarWhite = 0xFF1A1A1A; // Primary text (Dark Grey)
constexpr uint32_t MoonGray = 0xFF555555;  // Secondary text (Medium Grey)

// Traffic light buttons (VitusOS style)
constexpr uint32_t ButtonClose = 0xFFE57C3A;    // Orange (close)
constexpr uint32_t ButtonMinimize = 0xFFD4A93E; // Gold (minimize)
constexpr uint32_t ButtonMaximize = 0xFF4A9FD4; // Blue (maximize)

// Transparency
constexpr uint32_t GlassBackground = 0xD9FFFFFF; // 85% opacity white
constexpr uint32_t PanelBackground = 0xF2F5F5F5; // 95% opacity light grey
constexpr uint32_t DockBackground = 0xB3FFFFFF;  // 70% opacity white

// ==========================================================================
// Dimensions
// ==========================================================================

constexpr int PanelHeight = 28;      // Global menu bar height
constexpr int DockHeight = 64;       // Dock icon size
constexpr int DockItemSpacing = 8;   // Space between dock items
constexpr int DockPadding = 8;       // Dock edge padding
constexpr int DockCornerRadius = 16; // Dock rounded corners

constexpr int WindowCornerRadius = 8;  // Window corner radius
constexpr int TitleBarHeight = 30;     // Window title bar
constexpr int TrafficLightSize = 12;   // Traffic light button diameter
constexpr int TrafficLightSpacing = 8; // Space between buttons
constexpr int TrafficLightMargin = 10; // Margin from edge

constexpr int AccentBarHeight = 3; // Orange accent bar on windows

// ==========================================================================
// Typography
// ==========================================================================

constexpr const char *FontFamily = "Inter";
constexpr const char *FallbackFont = "sans-serif";

constexpr int FontSizeSmall = 11;
constexpr int FontSizeNormal = 13;
constexpr int FontSizeLarge = 16;
constexpr int FontSizeTitle = 20;
constexpr int FontSizeHero = 32;

// ==========================================================================
// Animation Timing
// ==========================================================================

constexpr double AnimationDurationFast = 0.15;   // 150ms
constexpr double AnimationDurationNormal = 0.25; // 250ms
constexpr double AnimationDurationSlow = 0.4;    // 400ms

// Easing curves (cubic bezier control points)
// ease-out: starts fast, ends slow
constexpr double EaseOutX1 = 0.0, EaseOutY1 = 0.0;
constexpr double EaseOutX2 = 0.2, EaseOutY2 = 1.0;

// ease-in-out: slow start and end
constexpr double EaseInOutX1 = 0.4, EaseInOutY1 = 0.0;
constexpr double EaseInOutX2 = 0.2, EaseInOutY2 = 1.0;

// ==========================================================================
// Helper Functions
// ==========================================================================

// Extract color components from ARGB
inline void argbToRGBA(uint32_t color, double &r, double &g, double &b,
                       double &a) {
  a = ((color >> 24) & 0xFF) / 255.0;
  r = ((color >> 16) & 0xFF) / 255.0;
  g = ((color >> 8) & 0xFF) / 255.0;
  b = (color & 0xFF) / 255.0;
}

// Set Cairo source from ARGB color
inline void setCairoColor(cairo_t *cr, uint32_t color) {
  double r, g, b, a;
  argbToRGBA(color, r, g, b, a);
  cairo_set_source_rgba(cr, r, g, b, a);
}

// Draw rounded rectangle path
inline void roundedRect(cairo_t *cr, double x, double y, double w, double h,
                        double r) {
  cairo_new_sub_path(cr);
  cairo_arc(cr, x + w - r, y + r, r, -M_PI / 2, 0);
  cairo_arc(cr, x + w - r, y + h - r, r, 0, M_PI / 2);
  cairo_arc(cr, x + r, y + h - r, r, M_PI / 2, M_PI);
  cairo_arc(cr, x + r, y + r, r, M_PI, 3 * M_PI / 2);
  cairo_close_path(cr);
}

// Draw circle
inline void circle(cairo_t *cr, double cx, double cy, double r) {
  cairo_arc(cr, cx, cy, r, 0, 2 * M_PI);
}

// Linear interpolation for animations
inline double lerp(double a, double b, double t) { return a + (b - a) * t; }

// Cubic ease-out for animations
inline double easeOut(double t) {
  return 1.0 - (1.0 - t) * (1.0 - t) * (1.0 - t);
}

// Cubic ease-in-out for animations
inline double easeInOut(double t) {
  return t < 0.5 ? 4 * t * t * t
                 : 1 - ((-2 * t + 2) * (-2 * t + 2) * (-2 * t + 2)) / 2;
}

} // namespace AresTheme

} // namespace opensef

#endif // OSF_ARES_THEME_H
