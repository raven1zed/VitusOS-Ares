/**
 * OSFWindow.h - Application Window Class
 *
 * Phase 2+3: Windowing Integration + Responder Chain
 *
 * OSFWindow wraps an XDG surface for standard application windows.
 * Inherits from OSFResponder to participate in the responder chain.
 */

#pragma once

#include <functional>
#include <memory>
#include <opensef/OSFResponder.h>
#include <string>

// Forward declaration for Cairo context
typedef struct _cairo cairo_t;

namespace opensef {

// Forward declaration
class OSFView;

/**
 * OSFWindow - A top-level application window
 *
 * Represents a single window that can be shown on the compositor.
 * Uses XDG shell protocol for standard window decorations.
 * Participates in the responder chain (forwards to OSFApplication).
 */
class OSFWindow : public OSFResponder {
public:
  using CloseCallback = std::function<void()>;
  using ResizeCallback = std::function<void(int width, int height)>;
  using DrawCallback = std::function<void(cairo_t *cr, int width, int height)>;

  /**
   * Create a new window with given dimensions.
   * Window is not shown until show() is called.
   * Automatically registers with OSFApplication.
   */
  OSFWindow(int width, int height, const std::string &title = "");
  ~OSFWindow();

  // Prevent copying
  OSFWindow(const OSFWindow &) = delete;
  OSFWindow &operator=(const OSFWindow &) = delete;

  // === Lifecycle ===

  bool connect(const char *displayName = nullptr);
  void disconnect();
  void show();
  void hide();
  void close();
  bool isVisible() const { return visible_; }

  // === Properties ===

  void setTitle(const std::string &title);
  const std::string &title() const { return title_; }
  int width() const { return width_; }
  int height() const { return height_; }
  void setSize(int width, int height);

  // === Content View (Phase 3) ===

  /**
   * Set the root view for this window.
   * The content view fills the window and is the root of the view hierarchy.
   */
  void setContentView(std::shared_ptr<OSFView> view);
  std::shared_ptr<OSFView> contentView() const { return contentView_; }

  // === Event Loop ===

  /**
   * Run the event loop for this window (blocks).
   * In Phase 3+, prefer OSFApplication::run() for unified event loop.
   */
  void runEventLoop();
  void stopEventLoop();

  /**
   * Process pending events without blocking (for unified event loop).
   * Returns true if there are more events to process.
   */
  bool processEvents();

  /**
   * Update window state and render if needed.
   * Called by OSFApplication every iteration.
   */
  void update();

  /**
   * Returns true if the window needs a redraw.
   */
  bool needsRedraw() const { return needsRedraw_; }

  // === Callbacks ===

  void onClose(CloseCallback callback) { closeCallback_ = callback; }
  void onResize(ResizeCallback callback) { resizeCallback_ = callback; }
  void onDraw(DrawCallback callback) { drawCallback_ = callback; }

  /**
   * Mark the window as needing a redraw.
   * rendering will happen in the next event loop iteration.
   */
  void setNeedsDisplay() { needsRedraw_ = true; }

  // === OSFResponder Overrides ===

  OSFResponder *nextResponder() const override;
  bool mouseDown(OSFEvent &event) override;
  bool keyDown(OSFEvent &event) override;

  // === First Responder ===

  /**
   * Make a view the first responder in this window.
   */
  bool makeFirstResponder(OSFResponder *responder);
  OSFResponder *firstResponder() const { return firstResponder_; }

  // === Factory ===

  static std::shared_ptr<OSFWindow> create(int width, int height,
                                           const std::string &title = "");

  // === Internal ===

  /**
   * Get the Wayland display file descriptor for polling.
   */
  int displayFd() const;

private:
  friend struct Impl;

  // Window state
  std::string title_;
  int width_ = 0;
  int height_ = 0;
  bool visible_ = false;
  bool running_ = false;
  bool needsRedraw_ = true; // Phase 3 Performance Fix: Event-driven redraws

  // Content
  std::shared_ptr<OSFView> contentView_;
  OSFResponder *firstResponder_ = nullptr;

  // Callbacks
  CloseCallback closeCallback_;
  ResizeCallback resizeCallback_;
  DrawCallback drawCallback_;

  // Wayland state (implementation in .cpp)
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace opensef
