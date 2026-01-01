/**
 * OSFWindowDecorations.h - macOS-style window decorations
 *
 * Traffic light buttons (close/minimize/maximize)
 * Client-side decorations for Wayland
 */

#pragma once

#include <functional>
#include <memory>
#include <opensef/OpenSEFBase.h>
#include <opensef/OpenSEFAppKit.h>
#include <string>
#include <vector>


namespace opensef {

// Forward declarations (avoid circular include)
class OSFView;
class OSFWaylandSurface;

// ============================================================================
// OSFWindowButton - Traffic light button (close/min/max)
// ============================================================================

enum class OSFWindowButtonType { Close, Minimize, Maximize };

class OSFWindowButton {
public:
  OSFWindowButton(OSFWindowButtonType type);
  virtual ~OSFWindowButton();

  static std::shared_ptr<OSFWindowButton> create(OSFWindowButtonType type);

  OSFWindowButtonType type() const { return type_; }

  // Frame
  OSFRect frame() const { return frame_; }
  void setFrame(const OSFRect &frame) { frame_ = frame; }

  // Colors
  OSFColor normalColor() const;
  OSFColor hoverColor() const;
  OSFColor pressedColor() const;

  // State
  bool isHovered() const { return hovered_; }
  bool isPressed() const { return pressed_; }
  void setHovered(bool hovered) { hovered_ = hovered; }
  void setPressed(bool pressed) { pressed_ = pressed; }

  // Action
  void setAction(std::function<void()> action) { action_ = action; }
  void click();

  virtual void render(cairo_t *cr);

  // Button radius (macOS standard is 6px radius = 12px diameter)
  static constexpr float kButtonRadius = 6.0f;
  static constexpr float kButtonSpacing = 8.0f;

private:
  OSFWindowButtonType type_;
  OSFRect frame_;
  bool hovered_ = false;
  bool pressed_ = false;
  std::function<void()> action_;
};

// ============================================================================
// OSFTitleBar - Window title bar with traffic lights
// ============================================================================

class OSFTitleBar {
public:
  OSFTitleBar();
  virtual ~OSFTitleBar();

  static std::shared_ptr<OSFTitleBar> create(const std::string &title);

  // Frame
  OSFRect frame() const { return frame_; }
  void setFrame(const OSFRect &frame) { frame_ = frame; }

  // Title
  const std::string &title() const { return title_; }
  void setTitle(const std::string &title);

  // Buttons
  std::shared_ptr<OSFWindowButton> closeButton() const { return closeButton_; }
  std::shared_ptr<OSFWindowButton> minimizeButton() const {
    return minimizeButton_;
  }
  std::shared_ptr<OSFWindowButton> maximizeButton() const {
    return maximizeButton_;
  }

  // Actions
  void setOnClose(std::function<void()> action);
  void setOnMinimize(std::function<void()> action);
  void setOnMaximize(std::function<void()> action);

  // Drawing
  virtual void render(cairo_t *cr);

  // Standard height
  static constexpr float kTitleBarHeight = 28.0f;
  static constexpr float kButtonPadding = 8.0f;

private:
  std::string title_;
  OSFRect frame_;
  std::shared_ptr<OSFWindowButton> closeButton_;
  std::shared_ptr<OSFWindowButton> minimizeButton_;
  std::shared_ptr<OSFWindowButton> maximizeButton_;

  void layoutButtons();
};

// ============================================================================
// OSFTilingZone - Tiling snap zones
// ============================================================================

enum class OSFTilingZone {
  None,
  Left,        // Left half
  Right,       // Right half
  TopLeft,     // Top-left quarter
  TopRight,    // Top-right quarter
  BottomLeft,  // Bottom-left quarter
  BottomRight, // Bottom-right quarter
  Maximize     // Full screen
};

class OSFTilingManager {
public:
  static OSFTilingManager &shared();

  // Detect zone from screen position
  OSFTilingZone zoneAtPosition(float x, float y, float screenWidth,
                               float screenHeight);

  // Get frame for zone
  OSFRect frameForZone(OSFTilingZone zone, float screenWidth,
                       float screenHeight);

  // Edge detection threshold (pixels from edge)
  static constexpr float kEdgeThreshold = 20.0f;

private:
  OSFTilingManager() = default;
};

} // namespace opensef
