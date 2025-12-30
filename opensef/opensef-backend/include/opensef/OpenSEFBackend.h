/**
 * openSEF Backend: Display Server (C++ Version)
 *
 * Wayland surface management with XDG shell support
 *
 * REFACTORED: Input events are now forwarded to registered handlers
 * instead of being handled with hardcoded logic in the backend.
 */

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <opensef/OpenSEFBase.h>
#include <string>

namespace opensef {

// ============================================================================
// Input Event Types
// ============================================================================

/**
 * OSFMouseEvent - Raw mouse event forwarded from backend
 */
struct OSFMouseEvent {
  enum class Type { Move, ButtonDown, ButtonUp, Enter, Leave, Scroll };

  Type type = Type::Move;
  double x = 0;
  double y = 0;
  uint32_t button = 0; // BTN_LEFT, BTN_RIGHT, etc.
  double scrollX = 0;
  double scrollY = 0;
  uint32_t serial = 0; // Wayland serial for cursor setting
};

/**
 * OSFKeyEvent - Raw keyboard event
 */
struct OSFKeyEvent {
  enum class Type { KeyDown, KeyUp };

  Type type = Type::KeyDown;
  uint32_t keycode = 0;
  uint32_t modifiers = 0;
};

// ============================================================================
// OSFBackend - Display backend coordinator
// ============================================================================

class OSFBackend {
public:
  static OSFBackend &shared();

  bool connect();
  void disconnect();
  void run();

  // Run with periodic callback for animations (intervalMs between calls)
  void runWithCallback(std::function<void()> callback, int intervalMs);

  void stop() { running_ = false; }

  // === Input Event Handlers (NEW - replaces hardcoded hit testing) ===
  // Register these to receive raw input events
  using MouseEventHandler = std::function<void(const OSFMouseEvent &)>;
  using KeyEventHandler = std::function<void(const OSFKeyEvent &)>;

  void setMouseEventHandler(MouseEventHandler handler) {
    mouseHandler_ = handler;
  }
  void setKeyEventHandler(KeyEventHandler handler) { keyHandler_ = handler; }

  // === DEPRECATED: Legacy callbacks (use event handlers instead) ===
  [[deprecated("Use setMouseEventHandler instead")]]
  void setMinimizeCallback(std::function<void()> cb);
  [[deprecated("Use setMouseEventHandler instead")]]
  void setMaximizeCallback(std::function<void()> cb);

  bool isWayland() const { return true; }
  bool isConnected() const { return connected_; }

  // Wayland globals (opaque pointers for header)
  void *display() const { return wlDisplay_; }
  void *compositor() const { return wlCompositor_; }
  void *shm() const { return wlShm_; }
  void *xdgWmBase() const { return xdgWmBase_; }

  // Cursor control (for setting cursor from app code)
  void setCursor(const char *cursorName);

  // Internal: Forward events from Wayland callbacks (called by static handlers)
  void forwardMouseEvent(const OSFMouseEvent &event);
  void forwardKeyEvent(const OSFKeyEvent &event);

private:
  OSFBackend() = default;
  bool connected_ = false;
  bool running_ = false;

  void *wlDisplay_ = nullptr;
  void *wlRegistry_ = nullptr;
  void *wlCompositor_ = nullptr;
  void *wlShm_ = nullptr;
  void *xdgWmBase_ = nullptr;

  // Event handlers (forwarded from Wayland callbacks)
  MouseEventHandler mouseHandler_;
  KeyEventHandler keyHandler_;
};

// ============================================================================
// OSFWaylandSurface - Wayland surface with XDG toplevel
// ============================================================================

class OSFWaylandSurface : public OSFObject {
public:
  OSFWaylandSurface();
  ~OSFWaylandSurface();

  static std::shared_ptr<OSFWaylandSurface> create(int width, int height,
                                                   const std::string &title);

  bool initialize(int width, int height, const std::string &title);
  void destroy();

  void setTitle(const std::string &title);
  void commit();
  void draw(const OSFColor &color);

  int width() const { return width_; }
  int height() const { return height_; }
  bool isConfigured() const { return configured_; }
  bool shouldClose() const { return shouldClose_; }

  // Access pixel buffer for direct drawing
  uint32_t *buffer() { return static_cast<uint32_t *>(shmData_); }

  // Called by callbacks
  void onConfigure(uint32_t serial);
  void onClose();

private:
  int width_ = 0;
  int height_ = 0;
  std::string title_;
  bool configured_ = false;
  bool shouldClose_ = false;

  void *wlSurface_ = nullptr;
  void *xdgSurface_ = nullptr;
  void *xdgToplevel_ = nullptr;

  void *shmPool_ = nullptr;
  void *buffer_ = nullptr;
  void *shmData_ = nullptr;
  int shmFd_ = -1;
  size_t shmSize_ = 0;

  bool createBuffer();
  void destroyBuffer();
};

// ============================================================================
// OSFVulkanRenderer - Vulkan rendering (stub for now)
// ============================================================================

class OSFVulkanRenderer {
public:
  static OSFVulkanRenderer &shared();

  bool initialize();
  void shutdown();
  bool isAvailable() const { return available_; }

private:
  OSFVulkanRenderer() = default;
  bool available_ = false;
};

// ============================================================================
// OSFInputHandler - Input events
// ============================================================================

class OSFInputHandler {
public:
  static OSFInputHandler &shared();

  std::function<void(const std::string &key, unsigned modifiers)> onKeyDown;
  std::function<void(const std::string &key, unsigned modifiers)> onKeyUp;
  std::function<void(const OSFPoint &location)> onMouseMove;
  std::function<void(const OSFPoint &location, int button)> onMouseDown;
  std::function<void(const OSFPoint &location, int button)> onMouseUp;

private:
  OSFInputHandler() = default;
};

} // namespace opensef

// Text rendering
#include <opensef/OSFTextRenderer.h>
