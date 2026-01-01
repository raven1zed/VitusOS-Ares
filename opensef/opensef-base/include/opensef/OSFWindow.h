/**
 * OSFWindow.h - Application Window Class
 *
 * Phase 2: Windowing Integration
 *
 * OSFWindow wraps an XDG surface and connects to the view hierarchy.
 * This is separate from OSFSurface (which handles layer-shell for panel/dock).
 */

#pragma once

#include <functional>
#include <memory>
#include <string>

// Forward declarations
struct wl_display;
struct wl_surface;

namespace opensef {

// Forward declarations
class OSFView;

/**
 * OSFWindow - A top-level application window
 *
 * Represents a single window that can be shown on the compositor.
 * Uses XDG shell protocol for standard window decorations.
 */
class OSFWindow {
public:
  using CloseCallback = std::function<void()>;
  using ResizeCallback = std::function<void(int width, int height)>;

  /**
   * Create a new window with given dimensions.
   * Window is not shown until show() is called.
   */
  OSFWindow(int width, int height, const std::string &title = "");
  ~OSFWindow();

  // Prevent copying
  OSFWindow(const OSFWindow &) = delete;
  OSFWindow &operator=(const OSFWindow &) = delete;

  // Lifecycle
  bool connect(const char *displayName = nullptr);
  void disconnect();
  void show();
  void hide();
  void close();
  bool isVisible() const { return visible_; }

  // Properties
  void setTitle(const std::string &title);
  const std::string &title() const { return title_; }
  int width() const { return width_; }
  int height() const { return height_; }
  void setSize(int width, int height);

  // Content
  void setContentView(std::shared_ptr<OSFView> view);
  std::shared_ptr<OSFView> contentView() const { return contentView_; }

  // Event loop (blocking)
  void runEventLoop();
  void stopEventLoop();

  // Callbacks
  void onClose(CloseCallback callback) { closeCallback_ = callback; }
  void onResize(ResizeCallback callback) { resizeCallback_ = callback; }

  // Factory
  static std::shared_ptr<OSFWindow> create(int width, int height,
                                           const std::string &title = "");

private:
  // Window state
  std::string title_;
  int width_ = 0;
  int height_ = 0;
  bool visible_ = false;
  bool running_ = false;

  // Content
  std::shared_ptr<OSFView> contentView_;

  // Callbacks
  CloseCallback closeCallback_;
  ResizeCallback resizeCallback_;

  // Wayland state (implementation in .cpp)
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace opensef
