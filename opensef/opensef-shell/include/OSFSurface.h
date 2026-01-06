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
#include <map>
#include <memory>
#include <string>
#include <vector>
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
  struct CairoDestroyer {
    void operator()(cairo_t *context) const {
      if (context) {
        cairo_destroy(context);
      }
    }
  };
  using CairoContextPtr = std::unique_ptr<cairo_t, CairoDestroyer>;

  OSFSurface(const std::string &namespace_name);
  ~OSFSurface();

  // Initialization
  bool connect(const char *display = nullptr);
  void disconnect();

  // Configuration
  void setLayer(Layer layer);

  // Wayland callbacks (must be public for C-interop)
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
  void setAnchor(Anchor anchor);
  void setSize(int width, int height);
  void setExclusiveZone(int zone);
  void setInputRegion(const std::vector<cairo_rectangle_int_t> &rects);
  void setOpaqueRegion(const std::vector<cairo_rectangle_int_t> &rects);
  void setMargin(int top, int right, int bottom, int left);

  // Wayland Listeners (Public for C-interop)
  static void seatCapabilities(void *data, struct wl_seat *seat,
                               uint32_t capabilities);
  static void pointerEnter(void *data, struct wl_pointer *pointer,
                           uint32_t serial, struct wl_surface *surface,
                           wl_fixed_t sx, wl_fixed_t sy);
  static void pointerLeave(void *data, struct wl_pointer *pointer,
                           uint32_t serial, struct wl_surface *surface);
  static void pointerMotion(void *data, struct wl_pointer *pointer,
                            uint32_t time, wl_fixed_t sx, wl_fixed_t sy);
  static void pointerButton(void *data, struct wl_pointer *pointer,
                            uint32_t serial, uint32_t time, uint32_t button,
                            uint32_t state);
  static void pointerAxis(void *data, struct wl_pointer *pointer, uint32_t time,
                          uint32_t axis, wl_fixed_t value);

  // Rendering
  CairoContextPtr beginPaint();
  void endPaint();
  void damage(int x, int y, int width, int height);
  void commit();
  void requestRedraw();

  // Event loop & Timers
  void run();
  void stop();
  bool isRunning() const { return running_; }

  /**
   * Adds a repeating timer.
   * @param intervalMs Interval in milliseconds.
   * @param callback Function to call when timer expires.
   * @return Timer ID (fd) or -1 on failure.
   */
  int addTimer(int intervalMs, std::function<void()> callback);
  void removeTimer(int timerId);

  // Callbacks
  using ConfigureCallback = std::function<void(int width, int height)>;
  using DrawCallback = std::function<void(cairo_t *cr, int width, int height)>;
  using CloseCallback = std::function<void()>;
  using MouseCallback = std::function<void(int x, int y, uint32_t button)>;
  using MouseEnterCallback = std::function<void(int x, int y)>;
  using MouseLeaveCallback = std::function<void()>;
  using TickCallback = std::function<void()>;

  void onConfigure(ConfigureCallback cb) { configureCallback_ = cb; }
  void onDraw(DrawCallback cb) { drawCallback_ = cb; }
  void onClose(CloseCallback cb) { closeCallback_ = cb; }
  void onMouseDown(MouseCallback cb) { mouseDownCallback_ = cb; }
  void onMouseUp(MouseCallback cb) { mouseUpCallback_ = cb; }
  void onMouseMove(std::function<void(int x, int y)> cb) {
    mouseMoveCallback_ = cb;
  }
  void onMouseEnter(MouseEnterCallback cb) { mouseEnterCallback_ = cb; }
  void onMouseLeave(MouseLeaveCallback cb) { mouseLeaveCallback_ = cb; }
  void onTick(TickCallback cb) { tickCallback_ = cb; }
  void setFrameTimer(int intervalMs);

  // Getters
  int width() const { return width_; }
  int height() const { return height_; }
  struct wl_display *display() const { return display_; }
  int displayFd() const;
  bool processEvents();
  void dispatchTimers();

private:
  // Wayland objects
  struct ::wl_display *display_ = nullptr;
  struct ::wl_registry *registry_ = nullptr;
  struct ::wl_compositor *compositor_ = nullptr;
  struct ::wl_shm *shm_ = nullptr;
  struct ::wl_seat *seat_ = nullptr;
  struct ::wl_pointer *pointer_ = nullptr;
  struct ::wl_surface *surface_ = nullptr;
  struct ::wl_output *output_ = nullptr;
  struct ::zwlr_layer_shell_v1 *layerShell_ = nullptr;
  struct ::zwlr_layer_surface_v1 *layerSurface_ = nullptr;

  // Input state
  int pointerX_ = 0;
  int pointerY_ = 0;

  // Cairo rendering
  cairo_surface_t *cairoSurface_ = nullptr;
  struct ::wl_buffer *buffer_ = nullptr;
  void *shmData_ = nullptr;
  int shmSize_ = 0;
  int shmFd_ = -1;
  int timerFd_ = -1;
  int timerIntervalMs_ = 0;

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

  // Timer state
  struct TimerInfo {
    int fd;
    std::function<void()> callback;
  };
  std::map<int, TimerInfo> timers_;

  // Callbacks
  ConfigureCallback configureCallback_;
  DrawCallback drawCallback_;
  CloseCallback closeCallback_;
  MouseCallback mouseDownCallback_;
  MouseCallback mouseUpCallback_;
  std::function<void(int, int)> mouseMoveCallback_;
  MouseEnterCallback mouseEnterCallback_;
  MouseLeaveCallback mouseLeaveCallback_;
  TickCallback tickCallback_;

  // Internal methods
  bool createShmBuffer(int width, int height);
  void destroyShmBuffer();
};

} // namespace opensef

#endif // OSF_SURFACE_H
