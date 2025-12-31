/**
 * OSFSurface.h - Cairo Surface for Wayland Layer Shell
 *
 * Core rendering surface that connects to the compositor via layer-shell.
 * Used by panel, dock, and other shell components.
 */

#ifndef OSF_SURFACE_H
#define OSF_SURFACE_H

#include <cairo.h>
#include <functional>
#include <memory>
#include <string>
#include <wayland-client.h>

// Forward declarations of Wayland structs (Global Scope)
struct wl_display;
struct wl_registry;
struct wl_compositor;
struct wl_shm;
struct wl_surface;
struct wl_output;
struct wl_buffer;
struct wl_shm_pool;
struct zwlr_layer_shell_v1;
struct zwlr_layer_surface_v1;

namespace opensef {

/**
 * Layer for surface positioning
 */
enum class Layer { Background = 0, Bottom = 1, Top = 2, Overlay = 3 };

/**
 * Anchor edges for layer surfaces
 */
enum class Anchor { None = 0, Top = 1, Bottom = 2, Left = 4, Right = 8 };

inline Anchor operator|(Anchor a, Anchor b) {
  return static_cast<Anchor>(static_cast<int>(a) | static_cast<int>(b));
}

inline int operator&(Anchor a, Anchor b) {
  return static_cast<int>(a) & static_cast<int>(b);
}

/**
 * OSFSurface - Cairo-rendered Wayland layer surface
 */
class OSFSurface {
public:
  OSFSurface(const std::string &namespace_name);
  ~OSFSurface();

  // Initialization
  bool connect(const char *display = nullptr);
  void disconnect();

  // Configuration
  void setLayer(Layer layer);
  void setAnchor(Anchor anchor);
  void setSize(int width, int height);
  void setExclusiveZone(int zone);
  void setMargin(int top, int right, int bottom, int left);

  // Rendering
  cairo_t *beginPaint();
  void endPaint();
  void damage(int x, int y, int width, int height);
  void commit();

  // Event loop
  void run();
  void stop();
  bool isRunning() const { return running_; }

  // Callbacks
  using ConfigureCallback = std::function<void(int width, int height)>;
  using DrawCallback = std::function<void(cairo_t *cr, int width, int height)>;
  using CloseCallback = std::function<void()>;

  void onConfigure(ConfigureCallback cb) { configureCallback_ = cb; }
  void onDraw(DrawCallback cb) { drawCallback_ = cb; }
  void onClose(CloseCallback cb) { closeCallback_ = cb; }

  // Getters
  int width() const { return width_; }
  int height() const { return height_; }
  struct wl_display *display() const { return display_; }

private:
  // Wayland objects
  struct ::wl_display *display_ = nullptr;
  struct ::wl_registry *registry_ = nullptr;
  struct ::wl_compositor *compositor_ = nullptr;
  struct ::wl_shm *shm_ = nullptr;
  struct ::wl_surface *surface_ = nullptr;
  struct ::wl_output *output_ = nullptr;
  struct ::zwlr_layer_shell_v1 *layerShell_ = nullptr;
  struct ::zwlr_layer_surface_v1 *layerSurface_ = nullptr;

  // Cairo rendering
  cairo_surface_t *cairoSurface_ = nullptr;
  cairo_t *cairoContext_ = nullptr;
  struct ::wl_buffer *buffer_ = nullptr;
  void *shmData_ = nullptr;
  int shmSize_ = 0;
  int shmFd_ = -1;

  // State
  std::string namespace_;
  Layer layer_ = Layer::Top;
  Anchor anchor_ = Anchor::None;
  int width_ = 0;
  int height_ = 0;
  int configuredWidth_ = 0;
  int configuredHeight_ = 0;
  int exclusiveZone_ = 0;
  int marginTop_ = 0, marginRight_ = 0, marginBottom_ = 0, marginLeft_ = 0;
  bool running_ = false;
  bool configured_ = false;

  // Callbacks
  ConfigureCallback configureCallback_;
  DrawCallback drawCallback_;
  CloseCallback closeCallback_;

  // Internal methods
  bool createShmBuffer(int width, int height);
  void destroyShmBuffer();

  // Wayland listeners
  static void registryGlobal(void *data, struct wl_registry *registry,
                             uint32_t name, const char *interface,
                             uint32_t version);
  static void registryGlobalRemove(void *data, struct wl_registry *registry,
                                   uint32_t name);
  static void layerSurfaceConfigure(void *data,
                                    struct zwlr_layer_surface_v1 *surface,
                                    uint32_t serial, uint32_t width,
                                    uint32_t height);
  static void layerSurfaceClosed(void *data,
                                 struct zwlr_layer_surface_v1 *surface);
};

} // namespace opensef

#endif // OSF_SURFACE_H
