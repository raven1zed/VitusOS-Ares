/**
 * OSFView.h - Window/View representation
 *
 * Represents a toplevel window managed by the compositor.
 */

#pragma once

extern "C" {
#include <wlr/types/wlr_scene.h>
#include <wlr/types/wlr_xdg_shell.h>
}

#include <wayland-server-core.h>

namespace opensef {

class OSFCompositor;

/**
 * OSFView - Represents a toplevel window
 */
class OSFView {
public:
  OSFView(OSFCompositor *compositor, wlr_xdg_toplevel *toplevel);
  ~OSFView();

  // Accessors
  wlr_xdg_toplevel *toplevel() const { return toplevel_; }
  wlr_scene_tree *sceneTree() const { return sceneTree_; }

  // Geometry
  int x() const { return x_; }
  int y() const { return y_; }
  void setPosition(int x, int y);

  // Focus
  void focus();

  // Window states
  void setMaximized(bool maximized);
  void setMinimized(bool minimized);
  void setFullscreen(bool fullscreen);

  // Server-side decorations
  void drawDecorations();

private:
  OSFCompositor *compositor_;
  wlr_xdg_toplevel *toplevel_;
  wlr_scene_tree *sceneTree_;

  int x_ = 0, y_ = 0;
  bool maximized_ = false;
  bool minimized_ = false;
  bool fullscreen_ = false;

  // Event listeners
  wl_listener map_;
  wl_listener unmap_;
  wl_listener destroy_;
  wl_listener requestMove_;
  wl_listener requestResize_;
  wl_listener requestMaximize_;
  wl_listener requestMinimize_;
  wl_listener requestFullscreen_;

  // Static handlers
  static void handleMap(wl_listener *listener, void *data);
  static void handleUnmap(wl_listener *listener, void *data);
  static void handleDestroy(wl_listener *listener, void *data);
  static void handleRequestMove(wl_listener *listener, void *data);
  static void handleRequestResize(wl_listener *listener, void *data);
  static void handleRequestMaximize(wl_listener *listener, void *data);
  static void handleRequestMinimize(wl_listener *listener, void *data);
  static void handleRequestFullscreen(wl_listener *listener, void *data);
};

} // namespace opensef
