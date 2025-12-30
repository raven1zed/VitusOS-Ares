/**
 * OSFShutdownScreen.h - Shutdown/Restart Screen
 *
 * Based on VitusOS Ares shutdown mockup (Windows Phone Lumia style):
 * - Pure black background
 * - Single word centered: "goodbye" (shutdown) or "restart" (restart)
 * - White text, lowercase
 * - No buttons, no icons - just text
 */

#pragma once

extern "C" {
#include <wlr/types/wlr_scene.h>
}

#include <cstdint>

namespace opensef {

// Forward declarations
class OSFCompositor;
class OSFDesktopLayers;

/**
 * Power action types
 */
enum class PowerAction {
  None,
  Shutdown, // Shows "goodbye"
  Restart,  // Shows "restart"
  Sleep,
  Logout
};

/**
 * OSFShutdownScreen - Minimal shutdown/restart display
 */
class OSFShutdownScreen {
public:
  OSFShutdownScreen(OSFCompositor *compositor, OSFDesktopLayers *layers);
  ~OSFShutdownScreen();

  // Show shutdown screen with "goodbye" text
  void showGoodbye();

  // Show restart screen with "restart" text
  void showRestart();

  // Hide the screen
  void hide();

  bool isVisible() const { return visible_; }
  PowerAction currentAction() const { return currentAction_; }

private:
  OSFCompositor *compositor_;
  OSFDesktopLayers *layers_;

  bool visible_ = false;
  PowerAction currentAction_ = PowerAction::None;

  // Scene nodes
  wlr_scene_rect *background_ = nullptr;

  // Create/destroy scene nodes
  void createNodes(const char *text);
  void destroyNodes();

  // Execute the power action
  void executeAction();
};

} // namespace opensef
