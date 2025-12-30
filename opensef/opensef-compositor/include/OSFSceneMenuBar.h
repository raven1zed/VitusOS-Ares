/**
 * OSFSceneMenuBar.h - Scene-based Menu Bar
 *
 * Renders the top menu bar in the Overlay layer.
 * Based on VitusOS Ares Desktop mockup:
 * - Orange multitask button (far left)
 * - Menu items: "Filer Menu Settings Help"
 * - Date/time on right
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

  // Update time display
  void updateTime();

  // Hit testing
  bool hitTestMultitask(int x, int y) const;
  int hitTestMenuItem(int x, int y) const; // Returns -1 if no hit

  // Callbacks
  std::function<void()> onMultitaskClicked;
  std::function<void(int)>
      onMenuItemClicked; // 0=Filer, 1=Menu, 2=Settings, 3=Help

private:
  OSFCompositor *compositor_;
  OSFDesktopLayers *layers_;

  // Scene nodes
  wlr_scene_rect *background_ = nullptr;
  wlr_scene_rect *multitaskButton_ = nullptr;

  int screenWidth_ = 0;
  int menuStartX_ = 0;

  // Button bounds for hit testing
  struct Bounds {
    int x, y, w, h;
  };
  Bounds multitaskBounds_;

  // Dimensions (from Ares mockup)
  static constexpr int Height = 28;
  static constexpr int MultitaskButtonSize = 20; // Updated from mockup
  static constexpr int MultitaskButtonMargin = 8;

  // Helpers
  std::string getFormattedDateTime() const;
};

} // namespace opensef
