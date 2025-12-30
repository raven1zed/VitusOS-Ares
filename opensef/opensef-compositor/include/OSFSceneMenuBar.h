/**
 * OSFSceneMenuBar.h - Scene-based Menu Bar
 *
 * Renders the top menu bar in the Overlay layer.
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
 * OSFSceneMenuBar - Scene-graph based menu bar
 */
class OSFSceneMenuBar {
public:
  OSFSceneMenuBar(OSFCompositor *compositor, OSFDesktopLayers *layers);
  ~OSFSceneMenuBar();

  // Initialize menu bar visuals
  void create(int screenWidth);

  // Update when screen size changes
  void resize(int screenWidth);

  // Callbacks
  std::function<void()> onMultitaskClicked;

private:
  OSFCompositor *compositor_;
  OSFDesktopLayers *layers_;

  // Scene nodes
  wlr_scene_rect *background_ = nullptr;
  wlr_scene_rect *multitaskButton_ = nullptr;

  int screenWidth_ = 0;

  // Dimensions (from Ares spec)
  static constexpr int Height = 28;
  static constexpr int MultitaskButtonSize = 16;
  static constexpr int MultitaskButtonMargin = 8;
};

} // namespace opensef
