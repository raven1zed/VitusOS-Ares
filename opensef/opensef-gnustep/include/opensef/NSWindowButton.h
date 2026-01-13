/**
 * NSWindowButton.h - Traffic Light Window Button (GNUstep C++ API)
 *
 * Circular close/minimize/maximize buttons following Ares Design Language
 */

#pragma once

#include "NSButton.h"
#include <cstdint>

namespace OpenSEF {

/**
 * Window button types (macOS-style traffic lights)
 */
enum class NSWindowButtonType {
  Close,    // Red/Orange - closes window
  Minimize, // Yellow/Gold - minimizes to dock
  Maximize  // Green/Blue - fullscreen/maximize
};

/**
 * NSWindowButton - Traffic light button for window decorations
 *
 * Renders as a 12px diameter circle with Ares colors:
 * - Close: Space Orange (#E85D04)
 * - Minimize: Mars Gold (#D4A93E)
 * - Maximize: Mission Blue (#3D5A80)
 */
class NSWindowButton : public NSButton {
public:
  explicit NSWindowButton(NSWindowButtonType type);
  virtual ~NSWindowButton();

  // Type
  NSWindowButtonType windowButtonType() const { return m_windowType; }

  // Colors (Ares Design Language)
  static constexpr uint32_t CloseColor = 0xE85D04;      // Space Orange
  static constexpr uint32_t CloseHoverColor = 0xFF6347; // Lighter
  static constexpr uint32_t MinimizeColor = 0xD4A93E;   // Mars Gold
  static constexpr uint32_t MinimizeHoverColor = 0xFFE066;
  static constexpr uint32_t MaximizeColor = 0x3D5A80; // Mission Blue
  static constexpr uint32_t MaximizeHoverColor = 0x3395FF;

  // Get current color based on state
  uint32_t currentColor() const;
  uint32_t baseColor() const;
  uint32_t hoverColor() const;

  // Hover state
  void setHovered(bool hovered);
  bool isHovered() const { return m_hovered; }

  // Size (12px diameter circle)
  static constexpr int ButtonSize = 12;
  static constexpr int ButtonSpacing = 8;
  static constexpr int ButtonMargin = 12;

private:
  NSWindowButtonType m_windowType;
  bool m_hovered = false;
};

} // namespace OpenSEF
