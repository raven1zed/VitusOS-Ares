/**
 * openSEF Backend: Display Server (C++ Version)
 *
 * Wayland surface management with XDG shell support
 */

#pragma once

#include <functional>
#include <memory>
#include <opensef/OpenSEFBase.h>
#include <string>

namespace opensef {

// ============================================================================
// OSFBackend - Display backend coordinator
// ============================================================================

class OSFBackend {
public:
  static OSFBackend &shared();

  bool connect();
  void disconnect();
  void run();
  void stop() { running_ = false; }

  bool isWayland() const { return true; }
  bool isConnected() const { return connected_; }

  // Wayland globals (opaque pointers for header)
  void *display() const { return wlDisplay_; }
  void *compositor() const { return wlCompositor_; }
  void *shm() const { return wlShm_; }
  void *xdgWmBase() const { return xdgWmBase_; }

private:
  OSFBackend() = default;
  bool connected_ = false;
  bool running_ = false;

  void *wlDisplay_ = nullptr;
  void *wlRegistry_ = nullptr;
  void *wlCompositor_ = nullptr;
  void *wlShm_ = nullptr;
  void *xdgWmBase_ = nullptr;
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
