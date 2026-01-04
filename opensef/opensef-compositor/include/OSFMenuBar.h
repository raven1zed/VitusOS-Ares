/**
 * OSFMenuBar.h - VitusOS Top Menu Bar
 *
 * Global menu bar with:
 * - Multitask button (orange circle, top-left)
 * - App menu (File, Edit, View, etc.)
 * - System tray (clock, wifi, battery)
 */

#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>


namespace opensef {

// Menu bar dimensions (from Ares design)
namespace AresMenuBar {
constexpr int Height = 28;               // Menu bar height
constexpr int MultitaskButtonSize = 16;  // Orange circle size
constexpr int MultitaskButtonMargin = 8; // From edge
constexpr int ItemPadding = 12;          // Menu item padding
constexpr int FontSize = 13;             // Menu text size
} // namespace AresMenuBar

// Menu item
struct MenuItem {
  std::string title;
  bool isEnabled = true;
  std::function<void()> action;
};

// Forward declarations
class OSFCompositor;

/**
 * OSFMenuBar - Global top menu bar
 */
class OSFMenuBar {
public:
  OSFMenuBar(OSFCompositor *compositor);

  // Render the menu bar
  void render(uint32_t *buffer, int screenW, int screenH);

  // Handle click
  bool handleClick(int x, int y);

  // Update current time (call every second)
  void updateClock();

  // Set the active app name (shows in menu bar)
  void setActiveApp(const std::string &appName);

  // Callbacks
  std::function<void()> onMultitaskClicked; // Opens workspace view

  // Is the multitask button hovered?
  bool isMultitaskHovered(int x, int y);

private:
  OSFCompositor *compositor_;
  std::string activeApp_ = "VitusOS";
  std::string currentTime_;

  // Drawing
  void drawBackground(uint32_t *buf, int sw, int sh);
  void drawMultitaskButton(uint32_t *buf, int sw, int sh, bool hovered);
  void drawAppMenu(uint32_t *buf, int sw, int sh);
  void drawSystemTray(uint32_t *buf, int sw, int sh);
  void drawClock(uint32_t *buf, int sw, int sh);
};

} // namespace opensef
