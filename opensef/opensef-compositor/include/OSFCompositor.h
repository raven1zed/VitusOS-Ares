/**
 * OSFCompositor.h - openSEF Wayland Compositor
 *
 * Based on wlroots, provides the core compositor functionality
 * for the VitusOS Ares desktop environment.
 */

#pragma once

extern "C" {
#include <wlr/backend.h>
#include <wlr/render/allocator.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_subcompositor.h>
#include <wlr/types/wlr_xcursor_manager.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/util/log.h>
}

#include <list>
#include <memory>
#include <string>
#include <wayland-server-core.h>

namespace opensef {

// Forward declarations
class OSFView;
class OSFOutput;
class OSFKeyboard;
class OSFDock;
class OSFMenuBar;
class OSFWorkspaceView;
class OSFWallpaper;

/**
 * OSFCompositor - Main compositor class
 *
 * Manages the Wayland display, wlroots backend, and all
 * connected outputs, views, and input devices.
 */
class OSFCompositor {
public:
  static OSFCompositor &shared();

  bool initialize();
  void run();
  void shutdown();

  // Accessors
  wl_display *display() const { return display_; }
  wlr_backend *backend() const { return backend_; }
  wlr_renderer *renderer() const { return renderer_; }
  wlr_allocator *allocator() const { return allocator_; }
  wlr_scene *scene() const { return scene_; }
  wlr_seat *seat() const { return seat_; }
  wlr_cursor *cursor() const { return cursor_; }
  wlr_output_layout *outputLayout() const { return outputLayout_; }

  // View management
  void focusView(OSFView *view);
  OSFView *viewAt(double x, double y, wlr_surface **surface, double *sx,
                  double *sy);

  // Input
  void setCursorMode(int mode);

private:
  OSFCompositor() = default;
  ~OSFCompositor();

  // Wayland display
  wl_display *display_ = nullptr;

  // wlroots core objects
  wlr_backend *backend_ = nullptr;
  wlr_renderer *renderer_ = nullptr;
  wlr_allocator *allocator_ = nullptr;
  wlr_scene *scene_ = nullptr;
  wlr_scene_output_layout *sceneLayout_ = nullptr;

  // Protocols
  wlr_compositor *compositor_ = nullptr;
  wlr_subcompositor *subcompositor_ = nullptr;
  wlr_data_device_manager *dataDeviceMgr_ = nullptr;
  wlr_xdg_shell *xdgShell_ = nullptr;

  // Output management
  wlr_output_layout *outputLayout_ = nullptr;
  std::list<std::unique_ptr<OSFOutput>> outputs_;

  // Input
  wlr_seat *seat_ = nullptr;
  wlr_cursor *cursor_ = nullptr;
  wlr_xcursor_manager *cursorMgr_ = nullptr;
  std::list<std::unique_ptr<OSFKeyboard>> keyboards_;

  // Views (windows)
  std::list<std::unique_ptr<OSFView>> views_;

  // Desktop components
  std::unique_ptr<OSFDock> dock_;
  std::unique_ptr<OSFMenuBar> menuBar_;
  std::unique_ptr<OSFWorkspaceView> workspaceView_;
  std::unique_ptr<OSFWallpaper> wallpaper_;

  // Cursor state
  int cursorMode_ = 0;
  OSFView *grabbedView_ = nullptr;
  double grabX_ = 0, grabY_ = 0;

  // Event listeners (wl_listener)
  wl_listener newOutput_;
  wl_listener newXdgSurface_;
  wl_listener cursorMotion_;
  wl_listener cursorMotionAbsolute_;
  wl_listener cursorButton_;
  wl_listener cursorAxis_;
  wl_listener cursorFrame_;
  wl_listener newInput_;
  wl_listener requestSetCursor_;
  wl_listener requestSetSelection_;

  // Static event handlers
  static void handleNewOutput(wl_listener *listener, void *data);
  static void handleNewXdgSurface(wl_listener *listener, void *data);
  static void handleCursorMotion(wl_listener *listener, void *data);
  static void handleCursorMotionAbsolute(wl_listener *listener, void *data);
  static void handleCursorButton(wl_listener *listener, void *data);
  static void handleCursorAxis(wl_listener *listener, void *data);
  static void handleCursorFrame(wl_listener *listener, void *data);
  static void handleNewInput(wl_listener *listener, void *data);
  static void handleRequestSetCursor(wl_listener *listener, void *data);
  static void handleRequestSetSelection(wl_listener *listener, void *data);

  void processCursorMotion(uint32_t time);
};

} // namespace opensef
