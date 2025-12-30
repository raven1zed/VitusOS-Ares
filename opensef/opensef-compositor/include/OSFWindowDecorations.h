/**
 * OSFWindowDecorations.h - Traffic Light Window Decorations
 *
 * Based on VitusOS Ares mockup:
 * - Traffic light buttons: Red (close), Yellow (minimize), Blue (fullscreen)
 * - Frosted glass title bar
 * - Clean, minimal design
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
class OSFLayer;

/**
 * Window button actions
 */
enum class WindowButton { Close, Minimize, Fullscreen };

/**
 * OSFWindowDecorations - Traffic light style window decorations
 */
class OSFWindowDecorations {
public:
  OSFWindowDecorations(OSFCompositor *compositor, wlr_scene_tree *parent);
  ~OSFWindowDecorations();

  // === Configuration ===

  void setTitle(const std::string &title) { title_ = title; }
  const std::string &title() const { return title_; }

  void setWidth(int width);
  int width() const { return width_; }

  // === State ===

  void setActive(bool active);
  bool isActive() const { return active_; }

  // === Interaction ===

  /**
   * Check if a point is within the title bar.
   */
  bool hitTestTitleBar(int x, int y) const;

  /**
   * Check if a point hits a button. Returns button type or None.
   */
  WindowButton hitTestButton(int x, int y) const;

  // === Callbacks ===

  std::function<void()> onClose;
  std::function<void()> onMinimize;
  std::function<void()> onFullscreen;

  // === Scene Graph ===

  wlr_scene_tree *sceneTree() const { return tree_; }

  // === Dimensions ===

  static constexpr int TitleBarHeight = 32;
  static constexpr int ButtonSize = 12;
  static constexpr int ButtonSpacing = 8;
  static constexpr int ButtonMarginLeft = 12;
  static constexpr int ButtonMarginTop = 10;

private:
  OSFCompositor *compositor_;
  wlr_scene_tree *tree_ = nullptr;

  // Scene nodes
  wlr_scene_rect *titleBarBg_ = nullptr;
  wlr_scene_rect *closeButton_ = nullptr;
  wlr_scene_rect *minimizeButton_ = nullptr;
  wlr_scene_rect *fullscreenButton_ = nullptr;

  // State
  std::string title_;
  int width_ = 0;
  bool active_ = true;

  // Button positions (for hit testing)
  struct ButtonBounds {
    int x, y, size;
  };
  ButtonBounds closeBounds_;
  ButtonBounds minimizeBounds_;
  ButtonBounds fullscreenBounds_;

  void createNodes();
  void updateColors();
};

} // namespace opensef
