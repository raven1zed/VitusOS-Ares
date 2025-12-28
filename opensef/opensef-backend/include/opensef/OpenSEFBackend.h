/**
 * openSEF Backend: Display Server (C++ Version)
 *
 * Wayland surface management and Vulkan rendering
 */

#pragma once

#include <functional>
#include <memory>
#include <opensef/OpenSEFBase.h>


namespace opensef {

// ============================================================================
// OSFBackend - Display backend coordinator
// ============================================================================

class OSFBackend {
public:
  static OSFBackend &shared();

  bool connect();
  void disconnect();
  void run(); // Event loop

  bool isWayland() const { return true; } // VitusOS is Wayland-only
  bool isConnected() const { return connected_; }

private:
  OSFBackend() = default;
  bool connected_ = false;
  void *wlDisplay_ = nullptr;
  void *wlRegistry_ = nullptr;
};

// ============================================================================
// OSFWaylandSurface - Wayland surface wrapper
// ============================================================================

class OSFWaylandSurface : public OSFObject {
public:
  OSFWaylandSurface();
  ~OSFWaylandSurface();

  static std::shared_ptr<OSFWaylandSurface> create(const OSFSize &size,
                                                   const std::string &title);

  void commit();
  void destroy();

  void setSize(const OSFSize &size) { size_ = size; }
  void setTitle(const std::string &title) { title_ = title; }

private:
  OSFSize size_;
  std::string title_;
  void *wlSurface_ = nullptr;
  void *xdgSurface_ = nullptr;
  void *xdgToplevel_ = nullptr;
};

// ============================================================================
// OSFVulkanRenderer - Vulkan rendering backend
// ============================================================================

class OSFVulkanRenderer {
public:
  static OSFVulkanRenderer &shared();

  bool initialize();
  void shutdown();

  bool isAvailable() const { return available_; }
  bool supportsBlur() const { return true; } // We'll implement this

  void beginFrame();
  void endFrame();

  // Drawing primitives
  void drawRect(const OSFRect &rect, const OSFColor &color);
  void drawRoundedRect(const OSFRect &rect, float radius,
                       const OSFColor &color);
  void drawBlur(const OSFRect &rect, float radius);

private:
  OSFVulkanRenderer() = default;
  bool available_ = false;
  void *vkInstance_ = nullptr;
  void *vkDevice_ = nullptr;
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
