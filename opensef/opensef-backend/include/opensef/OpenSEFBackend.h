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


// Forward declare Wayland types
struct wl_display;
struct wl_registry;
struct wl_compositor;
struct wl_shm;
struct wl_shm_pool;
struct wl_buffer;
struct wl_surface;
struct xdg_wm_base;
struct xdg_surface;
struct xdg_toplevel;

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

  // Wayland globals (accessed by surfaces)
  wl_display *display() const { return wlDisplay_; }
  wl_compositor *compositor() const { return wlCompositor_; }
  wl_shm *shm() const { return wlShm_; }
  xdg_wm_base *xdgWmBase() const { return xdgWmBase_; }

private:
  OSFBackend() = default;
  bool connected_ = false;
  bool running_ = false;

  wl_display *wlDisplay_ = nullptr;
  wl_registry *wlRegistry_ = nullptr;
  wl_compositor *wlCompositor_ = nullptr;
  wl_shm *wlShm_ = nullptr;
  xdg_wm_base *xdgWmBase_ = nullptr;

  static void registryHandler(void *data, wl_registry *registry, uint32_t id,
                              const char *interface, uint32_t version);
  static void registryRemover(void *data, wl_registry *registry, uint32_t id);
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
  void draw(const OSFColor &color); // Fill with solid color

  int width() const { return width_; }
  int height() const { return height_; }
  bool isConfigured() const { return configured_; }
  bool shouldClose() const { return shouldClose_; }

private:
  int width_ = 0;
  int height_ = 0;
  std::string title_;
  bool configured_ = false;
  bool shouldClose_ = false;

  wl_surface *wlSurface_ = nullptr;
  xdg_surface *xdgSurface_ = nullptr;
  xdg_toplevel *xdgToplevel_ = nullptr;

  // Shared memory buffer
  wl_shm_pool *shmPool_ = nullptr;
  wl_buffer *buffer_ = nullptr;
  void *shmData_ = nullptr;
  int shmFd_ = -1;
  size_t shmSize_ = 0;

  bool createBuffer();
  void destroyBuffer();

  // XDG callbacks
  static void xdgSurfaceConfigure(void *data, xdg_surface *surface,
                                  uint32_t serial);
  static void xdgToplevelConfigure(void *data, xdg_toplevel *toplevel,
                                   int32_t width, int32_t height,
                                   wl_array *states);
  static void xdgToplevelClose(void *data, xdg_toplevel *toplevel);
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
