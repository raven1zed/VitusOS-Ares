/**
 * OSFAresTheme.h - VitusOS Ares Design System
 *
 * Central color and design constants for the Ares theme.
 * Based on the official VitusOS Ares UI mockups.
 *
 * This file consolidates all theme constants. Use AresTheme:: namespace
 * for new code. The AresColors namespace is maintained for compatibility.
 */

#pragma once

#include <cstdint>

namespace opensef {

/**
 * Ares Theme - Unified Color Palette
 *
 * Colors are based on:
 * - VitusOS Ares UI mockups
 * - Mars mission / space aesthetic
 * - macOS-inspired traffic lights
 */
namespace AresTheme {

// === Primary Colors (from UI mockups) ===
constexpr uint32_t SpaceOrange =
    0xE85D04; // Primary accent (buttons, text, progress)
constexpr uint32_t PureBlack = 0x000000; // Boot/shutdown background
constexpr uint32_t PureWhite = 0xFFFFFF; // Primary text on dark backgrounds

// === Mars Mission Colors ===
constexpr uint32_t MarsDust = 0xFB8500;    // Secondary orange
constexpr uint32_t MissionBlue = 0x3D5A80; // Tertiary accent

// === UI Element Colors ===
constexpr uint32_t LunarGray = 0xF0F0F0; // Title bar, light backgrounds
constexpr uint32_t MenuBarBg = 0x2A2A2A; // Semi-transparent menu bar
constexpr uint32_t DockBg = 0x3A3A3A;    // Frosted glass dock background
constexpr uint32_t WindowBg = 0xE8E8E8;  // Frosted glass window background
constexpr uint32_t SoftCream = 0xFBFBFB; // Background

// === Traffic Light Buttons ===
constexpr uint32_t TrafficRed = 0xFF5F57;    // Close button
constexpr uint32_t TrafficYellow = 0xFEBC2E; // Minimize button
constexpr uint32_t TrafficGreen = 0x28C840;  // Fullscreen button (green)
// Aliases for compatibility:
constexpr uint32_t TrafficBlue = TrafficGreen; // Used in some older code
constexpr uint32_t WarmGold = TrafficYellow;   // Alias

// === Text Colors ===
constexpr uint32_t TextPrimary = 0x2D2D2D; // Primary text on light backgrounds
constexpr uint32_t TextSecondary = 0x6B6B6B; // Secondary text
constexpr uint32_t TextDark = 0x1A1A1A;      // Text on light backgrounds
constexpr uint32_t TextGray = 0x888888; // Placeholder text (e.g., "password")

// === UI Structure ===
constexpr uint32_t Borders = 0xE0DDD9; // Window borders

// === Helper: Convert hex to float RGBA ===
inline void hexToRGBA(uint32_t hex, float *rgba, float alpha = 1.0f) {
  rgba[0] = static_cast<float>((hex >> 16) & 0xFF) / 255.0f; // R
  rgba[1] = static_cast<float>((hex >> 8) & 0xFF) / 255.0f;  // G
  rgba[2] = static_cast<float>(hex & 0xFF) / 255.0f;         // B
  rgba[3] = alpha;                                           // A
}

} // namespace AresTheme

// === Backwards Compatibility ===
// The AresColors namespace is an alias for AresTheme
// New code should use AresTheme directly
namespace AresColors = AresTheme;

// === UI Dimensions ===
namespace AresDimensions {

// Menu Bar
constexpr int MenuBarHeight = 28;
constexpr int MultitaskButtonSize = 20;
constexpr int MultitaskButtonMargin = 8;

// Dock
constexpr int DockHeight = 64;
constexpr int DockIconSize = 48;
constexpr int DockPadding = 12;
constexpr int DockIconSpacing = 8;
constexpr int DockCornerRadius = 16;
constexpr int DockBottomMargin = 8;

// Window Decorations
constexpr int TitleBarHeight = 32;
constexpr int TitleBarCompact = 28;
constexpr int TrafficButtonSize = 12;
constexpr int TrafficButtonSpacing = 8;
constexpr int TrafficButtonMargin = 12;
constexpr int ButtonDiameter = 12; // Alias for TrafficButtonSize
constexpr int ButtonRadius = 6;
constexpr int ButtonLeftOffset = 8;

// Window Frame
constexpr int BorderWidth = 1;
constexpr int CornerRadius = 8;
constexpr int ShadowRadius = 20;
constexpr int AccentBarWidth = 3; // Left edge accent

} // namespace AresDimensions

} // namespace opensef
