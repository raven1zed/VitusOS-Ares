/**
 * OSFDock.h - VitusOS Dock Component
 *
 * macOS-style dock at bottom of screen:
 * - Semi-transparent background with blur
 * - Icon magnification on hover
 * - Click to launch/focus apps
 * - Indicators for running apps
 */

#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>


namespace opensef {

// Dock item representing an app
struct DockItem {
  std::string appId;      // Unique identifier
  std::string name;       // Display name
  std::string iconPath;   // Path to icon
  bool isRunning = false; // Show indicator dot
  bool isFocused = false; // Currently focused
  float scale = 1.0f;     // For magnification animation
};

// Dock dimensions (from Ares design)
namespace AresDock {
constexpr int Height = 64;           // Dock height
constexpr int IconSize = 48;         // Icon size
constexpr int IconSpacing = 8;       // Between icons
constexpr int Padding = 12;          // Left/right padding
constexpr int CornerRadius = 16;     // Pill shape
constexpr int IndicatorSize = 4;     // Running app dot
constexpr float MagnifyScale = 1.4f; // Hover magnification
constexpr int BottomMargin = 8;      // Distance from screen edge
} // namespace AresDock

// Forward declarations
class OSFCompositor;

/**
 * OSFDock - The application dock
 */
class OSFDock {
public:
  OSFDock(OSFCompositor *compositor);

  // Render the dock to a buffer
  void render(uint32_t *buffer, int screenW, int screenH);

  // Update hover state based on cursor position
  void updateHover(int cursorX, int cursorY);

  // Handle click on dock
  bool handleClick(int x, int y);

  // Item management
  void addItem(const DockItem &item);
  void removeItem(const std::string &appId);
  void setRunning(const std::string &appId, bool running);
  void setFocused(const std::string &appId, bool focused);

  // Callbacks
  std::function<void(const std::string &)> onItemClicked;

  // Geometry
  int dockX() const { return dockX_; }
  int dockY() const { return dockY_; }
  int dockWidth() const { return dockWidth_; }
  int dockHeight() const { return AresDock::Height; }

private:
  OSFCompositor *compositor_;
  std::vector<DockItem> items_;
  int hoveredIndex_ = -1;

  // Calculated position
  int dockX_ = 0;
  int dockY_ = 0;
  int dockWidth_ = 0;

  // Drawing helpers
  void drawBackground(uint32_t *buf, int sw, int sh);
  void drawItem(uint32_t *buf, int sw, int sh, const DockItem &item, int x,
                int y);
  void drawIndicator(uint32_t *buf, int sw, int sh, int x, int y);

  void calculateGeometry(int screenW, int screenH);
  int itemAtPosition(int x, int y);
};

} // namespace opensef
