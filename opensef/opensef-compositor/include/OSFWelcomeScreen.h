/**
 * OSFWelcomeScreen.h - Onboarding Welcome Screen
 *
 * Based on VitusOS Ares welcome mockup:
 * - Top menu bar with "Onboarding Filer Settings Help" and date/time
 * - Frosted glass window with Mars wallpaper visible behind
 * - Traffic light buttons (red, yellow, blue) in top-left of window
 * - "vitusOS | ARES" branding (white + orange)
 * - Large "Hello,Welcome" text
 * - Orange "Let's Start" button
 */

#pragma once

extern "C" {
#include <wlr/types/wlr_scene.h>
}

#include <cstdint>
#include <functional>

namespace opensef {

// Forward declarations
class OSFCompositor;
class OSFDesktopLayers;

/**
 * OSFWelcomeScreen - First-run onboarding welcome
 */
class OSFWelcomeScreen {
public:
  OSFWelcomeScreen(OSFCompositor *compositor, OSFDesktopLayers *layers);
  ~OSFWelcomeScreen();

  // Show/hide the welcome screen
  void show();
  void hide();
  bool isVisible() const { return visible_; }

  // Callback when "Let's Start" is clicked
  std::function<void()> onStartClicked;

  // Handle click at position
  bool handleClick(int x, int y);

private:
  OSFCompositor *compositor_;
  OSFDesktopLayers *layers_;

  bool visible_ = false;

  // Screen dimensions
  int screenWidth_ = 0;
  int screenHeight_ = 0;

  // Scene nodes
  wlr_scene_rect *windowBg_ = nullptr;    // Frosted glass window background
  wlr_scene_rect *startButton_ = nullptr; // "Let's Start" button

  // Traffic light buttons
  wlr_scene_rect *closeButton_ = nullptr;
  wlr_scene_rect *minimizeButton_ = nullptr;
  wlr_scene_rect *fullscreenButton_ = nullptr;

  // Button bounds for click detection
  struct Bounds {
    int x, y, w, h;
  };
  Bounds startButtonBounds_;
  Bounds closeButtonBounds_;

  // Create/destroy scene nodes
  void createNodes();
  void destroyNodes();
};

} // namespace opensef
