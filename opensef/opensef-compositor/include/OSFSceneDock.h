/**
 * OSFSceneDock.h - Scene-based Dock Overlay
 *
 * Renders the dock as wlr_scene_rect elements in the Top layer.
 */

#pragma once

extern "C" {
#include <wlr/types/wlr_scene.h>
}

#include <cstdint>
#include <functional>
#include <string>
#include <vector>


namespace opensef {

// Forward declarations
class OSFCompositor;
class OSFDesktopLayers;

// Dock item
struct SceneDockItem {
  std::string appId;
  std::string name;
  bool isRunning = false;
  wlr_scene_rect *iconRect = nullptr;
  wlr_scene_rect *indicatorRect = nullptr;
};

/**
 * OSFSceneDock - Scene-graph based dock
 */
class OSFSceneDock {
public:
  OSFSceneDock(OSFCompositor *compositor, OSFDesktopLayers *layers);
  ~OSFSceneDock();

  // Initialize dock visuals
  void create(int screenWidth, int screenHeight);

  // Update position/size
  void resize(int screenWidth, int screenHeight);

  // Update when an app launches/closes
  void setAppRunning(const std::string &appId, bool running);

  // Callbacks
  std::function<void(const std::string &)> onItemClicked;

private:
  OSFCompositor *compositor_;
  OSFDesktopLayers *layers_;

  // Dock background
  wlr_scene_rect *background_ = nullptr;

  // Dock items
  std::vector<SceneDockItem> items_;

  int screenWidth_ = 0;
  int screenHeight_ = 0;

  // Dimensions (from Ares spec)
  static constexpr int Height = 64;
  static constexpr int IconSize = 48;
  static constexpr int Padding = 12;
  static constexpr int IconSpacing = 8;
  static constexpr int CornerRadius = 16;
  static constexpr int BottomMargin = 8;

  void createDefaultItems();
};

} // namespace opensef
