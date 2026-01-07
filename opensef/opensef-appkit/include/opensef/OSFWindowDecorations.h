/**
 * OSFWindowDecorations.h - macOS-style window decorations
 *
 * Traffic light buttons (close/minimize/maximize)
 * Client-side decorations for Wayland
 */

#pragma once

#include <functional>
#include <memory>
#include <opensef/OpenSEFAppKit.h>
#include <opensef/OpenSEFBase.h>
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

class OSFWindowButton : public OSFView {
public:
  OSFWindowButton(OSFWindowButtonType type);
  virtual ~OSFWindowButton();

  static std::shared_ptr<OSFWindowButton> create(OSFWindowButtonType type);

  OSFWindowButtonType type() const { return type_; }

  // Colors (AresTheme inspired)
  OSFColor normalColor() const;
  OSFColor hoverColor() const;
  OSFColor pressedColor() const;

  // State
  void setHovered(bool hovered);
  void setPressed(bool pressed);

  // Action
  void setAction(std::function<void()> action) { action_ = action; }
  void click();

  // OSFView overrides
  void render(cairo_t *cr) override;
  bool mouseDown(OSFEvent &event) override;
  bool mouseUp(OSFEvent &event) override;

  // Button radius (macOS standard is 6px radius = 12px diameter)
  static constexpr float kButtonRadius = 6.0f;
  static constexpr float kButtonSpacing = 8.0f;

private:
  OSFWindowButtonType type_;
  bool hovered_ = false;
  bool pressed_ = false;
  double symbolAlpha_ = 0.0; // Animates on hover
  std::function<void()> action_;
};

// ============================================================================
// OSFTitleBar - Window title bar with traffic lights
// ============================================================================

class OSFTitleBar : public OSFView {
public:
  OSFTitleBar();
  virtual ~OSFTitleBar();

  static std::shared_ptr<OSFTitleBar> create(const std::string &title);

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

  // OSFView overrides
  void render(cairo_t *cr) override;
  void layoutSubviews() override;

  // Standard height
  static constexpr float kTitleBarHeight = 28.0f;
  static constexpr float kButtonPadding = 8.0f;

private:
  std::string title_;
  std::shared_ptr<OSFWindowButton> closeButton_;
  std::shared_ptr<OSFWindowButton> minimizeButton_;
  std::shared_ptr<OSFWindowButton> maximizeButton_;
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
