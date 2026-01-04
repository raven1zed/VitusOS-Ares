/**
 * OSFLockScreen.h - Lock Screen UI
 *
 * Based on VitusOS Ares lockscreen mockup:
 * - Mars wallpaper background (full, not dimmed)
 * - Date in orange (e.g., "tuesday 25, 2025")
 * - Time in large orange text (e.g., "10:15 PM")
 * - Password field with "//" icon and "password" placeholder
 */

#pragma once

extern "C" {
#include <wlr/types/wlr_scene.h>
}

#include <cstdint>
#include <functional>
#include <string>

namespace opensef {

// Forward declarations
class OSFCompositor;
class OSFDesktopLayers;

/**
 * OSFLockScreen - Minimal lock screen with time/date
 */
class OSFLockScreen {
public:
  OSFLockScreen(OSFCompositor *compositor, OSFDesktopLayers *layers);
  ~OSFLockScreen();

  // Show/hide the lock screen
  void show();
  void hide();
  bool isVisible() const { return visible_; }

  // Update clock display (call every minute)
  void updateClock();

  // Authentication callback
  std::function<void(const std::string &password)> onPasswordEntered;
  std::function<void(bool success)> onAuthResult;

  // Handle input
  void onKeyPress(uint32_t keycode, bool pressed);

private:
  OSFCompositor *compositor_;
  OSFDesktopLayers *layers_;

  bool visible_ = false;
  std::string passwordBuffer_;

  // Scene nodes (in Lock layer)
  wlr_scene_rect *background_ = nullptr; // Dim overlay if needed

  // Time formatting
  std::string formattedDate() const; // "tuesday 25, 2025"
  std::string formattedTime() const; // "10:15 PM"

  // Create/destroy scene nodes
  void createNodes();
  void destroyNodes();
};

} // namespace opensef
