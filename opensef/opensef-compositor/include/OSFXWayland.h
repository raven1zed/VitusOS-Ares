/**
 * OSFXWayland.h - XWayland Integration
 *
 * Enables running X11 applications on the openSEF compositor.
 * Provides seamless integration with Wayland-native windows.
 */

#pragma once

extern "C" {
#include <wlr/xwayland.h>
}

#include <list>
#include <memory>

namespace opensef {

// Forward declarations
class OSFCompositor;
class OSFXWaylandView;

/**
 * OSFXWayland - XWayland server manager
 */
class OSFXWayland {
public:
  OSFXWayland(OSFCompositor *compositor);
  ~OSFXWayland();

  bool initialize();
  void shutdown();

  // Check if XWayland is available
  bool isAvailable() const { return xwayland_ != nullptr; }

  // Get the X11 display string (e.g., ":0")
  const char *displayName() const;

private:
  OSFCompositor *compositor_;
  wlr_xwayland *xwayland_ = nullptr;

  // Views for X11 windows
  std::list<std::unique_ptr<OSFXWaylandView>> views_;

  // Event listeners
  wl_listener ready_;
  wl_listener newSurface_;

  // Static handlers
  static void handleReady(wl_listener *listener, void *data);
  static void handleNewSurface(wl_listener *listener, void *data);
};

/**
 * OSFXWaylandView - Wrapper for X11 windows
 */
class OSFXWaylandView {
public:
  OSFXWaylandView(OSFXWayland *xwayland, wlr_xwayland_surface *surface);
  ~OSFXWaylandView();

  wlr_xwayland_surface *surface() const { return surface_; }

  // Position and size
  void setPosition(int x, int y);
  void setSize(int width, int height);

  // Focus
  void focus();
  void activate(bool active);

  // Window state
  void setMaximized(bool maximized);
  void setFullscreen(bool fullscreen);
  void close();

private:
  OSFXWayland *xwayland_;
  wlr_xwayland_surface *surface_;
  wlr_scene_tree *sceneTree_ = nullptr;

  // Event listeners
  wl_listener map_;
  wl_listener unmap_;
  wl_listener destroy_;
  wl_listener requestMove_;
  wl_listener requestResize_;
  wl_listener requestMaximize_;
  wl_listener requestFullscreen_;
  wl_listener requestActivate_;
  wl_listener setTitle_;

  // Static handlers
  static void handleMap(wl_listener *listener, void *data);
  static void handleUnmap(wl_listener *listener, void *data);
  static void handleDestroy(wl_listener *listener, void *data);
  static void handleRequestMove(wl_listener *listener, void *data);
  static void handleRequestResize(wl_listener *listener, void *data);
  static void handleRequestMaximize(wl_listener *listener, void *data);
  static void handleRequestFullscreen(wl_listener *listener, void *data);
  static void handleRequestActivate(wl_listener *listener, void *data);
  static void handleSetTitle(wl_listener *listener, void *data);
};

} // namespace opensef
