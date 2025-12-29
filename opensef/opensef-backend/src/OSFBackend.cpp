/**
 * OSFBackend.cpp - Wayland backend with XDG shell
 */

#include <chrono>
#include <cstring>
#include <iostream>
#include <linux/input.h>
#include <opensef/OpenSEFBackend.h>
#include <poll.h>

#include "xdg-shell-client-protocol.h"
#include <wayland-client.h>
#include <wayland-cursor.h>

namespace opensef {

// Global Wayland objects (set by registry)
static wl_compositor *g_compositor = nullptr;
static wl_shm *g_shm = nullptr;
static xdg_wm_base *g_xdgWmBase = nullptr;
static wl_seat *g_seat = nullptr;
static wl_pointer *g_pointer = nullptr;

// Cursor support
static wl_surface *g_cursorSurface = nullptr;
static wl_cursor_theme *g_cursorTheme = nullptr;
static wl_cursor *g_arrowCursor = nullptr;
static uint32_t g_pointerSerial = 0;

// Backend singleton storage
static wl_display *g_display = nullptr;
static wl_registry *g_registry = nullptr;
static bool g_connected = false;
static bool g_running = false;

// Mouse state
static double g_mouseX = 0;
static double g_mouseY = 0;
static bool g_mousePressed = false;

// XDG WM base ping handler
static void xdgWmBasePing(void *data, xdg_wm_base *xdg_wm_base,
                          uint32_t serial) {
  (void)data;
  xdg_wm_base_pong(xdg_wm_base, serial);
}

static const xdg_wm_base_listener xdgWmBaseListener = {xdgWmBasePing};

// ============================================================================
// Pointer (mouse) handlers
// ============================================================================

static void pointerEnter(void *data, wl_pointer *pointer, uint32_t serial,
                         wl_surface *surface, wl_fixed_t sx, wl_fixed_t sy) {
  (void)data;
  (void)surface;
  g_mouseX = wl_fixed_to_double(sx);
  g_mouseY = wl_fixed_to_double(sy);
  g_pointerSerial = serial;

  // Set arrow cursor when entering window
  if (g_arrowCursor && g_cursorSurface) {
    wl_cursor_image *image = g_arrowCursor->images[0];
    wl_buffer *buffer = wl_cursor_image_get_buffer(image);
    wl_surface_attach(g_cursorSurface, buffer, 0, 0);
    wl_surface_damage(g_cursorSurface, 0, 0, image->width, image->height);
    wl_surface_commit(g_cursorSurface);
    wl_pointer_set_cursor(pointer, serial, g_cursorSurface, image->hotspot_x,
                          image->hotspot_y);
  }
}

static void pointerLeave(void *data, wl_pointer *pointer, uint32_t serial,
                         wl_surface *surface) {
  (void)data;
  (void)pointer;
  (void)serial;
  (void)surface;
}

static void pointerMotion(void *data, wl_pointer *pointer, uint32_t time,
                          wl_fixed_t sx, wl_fixed_t sy) {
  (void)data;
  (void)pointer;
  (void)time;
  g_mouseX = wl_fixed_to_double(sx);
  g_mouseY = wl_fixed_to_double(sy);
}

static void pointerButton(void *data, wl_pointer *pointer, uint32_t serial,
                          uint32_t time, uint32_t button, uint32_t state) {
  (void)data;
  (void)pointer;
  (void)serial;
  (void)time;
  g_mousePressed = (state == WL_POINTER_BUTTON_STATE_PRESSED);

  if (g_mousePressed && button == BTN_LEFT) {
    // Traffic light button hit areas (Ares spec: 12px dia, 6px radius)
    // Buttons at y=16, radius=6, btnStart=18
    // Close at x=18, Minimize at x=38, Maximize at x=58
    bool inButtonY = (g_mouseY >= 10 && g_mouseY <= 22);

    // Close button (Space Orange) - center at x=18
    if (inButtonY && g_mouseX >= 12 && g_mouseX <= 24) {
      std::cout << "[openSEF] Close button clicked - stopping" << std::endl;
      g_running = false;
      return;
    }

    // Minimize button (Warm Gold) - center at x=38
    if (inButtonY && g_mouseX >= 32 && g_mouseX <= 44) {
      std::cout << "[openSEF] Minimize button clicked" << std::endl;
      // TODO: xdg_toplevel_set_minimized
      return;
    }

    // Maximize button (Mission Blue) - center at x=58
    if (inButtonY && g_mouseX >= 52 && g_mouseX <= 64) {
      std::cout << "[openSEF] Maximize button clicked" << std::endl;
      // TODO: Window resize
      return;
    }

    // Log other clicks
    std::cout << "[openSEF] Click at (" << g_mouseX << ", " << g_mouseY << ")"
              << std::endl;
  }
}

static void pointerAxis(void *data, wl_pointer *pointer, uint32_t time,
                        uint32_t axis, wl_fixed_t value) {
  (void)data;
  (void)pointer;
  (void)time;
  (void)axis;
  (void)value;
}

static const wl_pointer_listener pointerListener = {
    pointerEnter, pointerLeave, pointerMotion, pointerButton, pointerAxis};

// ============================================================================
// Seat capabilities (to get pointer/keyboard)
// ============================================================================

static void seatCapabilities(void *data, wl_seat *seat, uint32_t caps) {
  (void)data;

  if ((caps & WL_SEAT_CAPABILITY_POINTER) && !g_pointer) {
    g_pointer = wl_seat_get_pointer(seat);
    wl_pointer_add_listener(g_pointer, &pointerListener, nullptr);
    std::cout << "[openSEF] Pointer (mouse) ready" << std::endl;

    // Initialize cursor theme and surface
    if (g_compositor && g_shm && !g_cursorTheme) {
      g_cursorTheme = wl_cursor_theme_load(nullptr, 24, g_shm);
      if (g_cursorTheme) {
        g_arrowCursor = wl_cursor_theme_get_cursor(g_cursorTheme, "left_ptr");
        if (!g_arrowCursor) {
          g_arrowCursor = wl_cursor_theme_get_cursor(g_cursorTheme, "default");
        }
        g_cursorSurface = wl_compositor_create_surface(g_compositor);
        std::cout << "[openSEF] Cursor theme loaded" << std::endl;
      }
    }
  }
}

static void seatName(void *data, wl_seat *seat, const char *name) {
  (void)data;
  (void)seat;
  (void)name;
}

static const wl_seat_listener seatListener = {seatCapabilities, seatName};

// ============================================================================
// Registry listener
// ============================================================================

static void registryHandler(void *data, wl_registry *registry, uint32_t id,
                            const char *interface, uint32_t version) {
  (void)data;
  (void)version;

  if (strcmp(interface, wl_compositor_interface.name) == 0) {
    g_compositor = static_cast<wl_compositor *>(
        wl_registry_bind(registry, id, &wl_compositor_interface, 4));
    std::cout << "[openSEF] Found compositor" << std::endl;
  } else if (strcmp(interface, wl_shm_interface.name) == 0) {
    g_shm = static_cast<wl_shm *>(
        wl_registry_bind(registry, id, &wl_shm_interface, 1));
    std::cout << "[openSEF] Found shared memory" << std::endl;
  } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
    g_xdgWmBase = static_cast<xdg_wm_base *>(
        wl_registry_bind(registry, id, &xdg_wm_base_interface, 1));
    xdg_wm_base_add_listener(g_xdgWmBase, &xdgWmBaseListener, nullptr);
    std::cout << "[openSEF] Found XDG shell" << std::endl;
  } else if (strcmp(interface, wl_seat_interface.name) == 0) {
    g_seat = static_cast<wl_seat *>(
        wl_registry_bind(registry, id, &wl_seat_interface, 1));
    wl_seat_add_listener(g_seat, &seatListener, nullptr);
    std::cout << "[openSEF] Found seat (input)" << std::endl;
  }
}

static void registryRemover(void *data, wl_registry *registry, uint32_t id) {
  (void)data;
  (void)registry;
  (void)id;
}

static const wl_registry_listener registryListener = {registryHandler,
                                                      registryRemover};

OSFBackend &OSFBackend::shared() {
  static OSFBackend instance;
  return instance;
}

bool OSFBackend::connect() {
  std::cout << "[openSEF] Connecting to Wayland display..." << std::endl;

  g_display = wl_display_connect(nullptr);
  if (!g_display) {
    std::cerr << "[openSEF] Failed to connect to Wayland display" << std::endl;
    return false;
  }
  wlDisplay_ = g_display;

  g_registry = wl_display_get_registry(g_display);
  wl_registry_add_listener(g_registry, &registryListener, nullptr);
  wlRegistry_ = g_registry;

  // Round-trip to receive globals
  wl_display_roundtrip(g_display);

  // Store in instance
  wlCompositor_ = g_compositor;
  wlShm_ = g_shm;
  xdgWmBase_ = g_xdgWmBase;

  if (!g_compositor || !g_shm || !g_xdgWmBase) {
    std::cerr << "[openSEF] Missing required Wayland interfaces:" << std::endl;
    if (!g_compositor)
      std::cerr << "  - wl_compositor" << std::endl;
    if (!g_shm)
      std::cerr << "  - wl_shm" << std::endl;
    if (!g_xdgWmBase)
      std::cerr << "  - xdg_wm_base" << std::endl;
    disconnect();
    return false;
  }

  g_connected = true;
  connected_ = true;
  std::cout << "[openSEF] Wayland connection established" << std::endl;
  return true;
}

void OSFBackend::disconnect() {
  if (g_xdgWmBase) {
    xdg_wm_base_destroy(g_xdgWmBase);
    g_xdgWmBase = nullptr;
    xdgWmBase_ = nullptr;
  }
  if (g_shm) {
    wl_shm_destroy(g_shm);
    g_shm = nullptr;
    wlShm_ = nullptr;
  }
  if (g_compositor) {
    wl_compositor_destroy(g_compositor);
    g_compositor = nullptr;
    wlCompositor_ = nullptr;
  }
  if (g_registry) {
    wl_registry_destroy(g_registry);
    g_registry = nullptr;
    wlRegistry_ = nullptr;
  }
  if (g_display) {
    wl_display_disconnect(g_display);
    g_display = nullptr;
    wlDisplay_ = nullptr;
  }
  g_connected = false;
  connected_ = false;
}

void OSFBackend::run() {
  if (!g_display)
    return;

  g_running = true;
  running_ = true;
  std::cout << "[openSEF] Entering event loop..." << std::endl;

  while (g_running && g_connected) {
    if (wl_display_dispatch(g_display) < 0) {
      std::cerr << "[openSEF] Display error" << std::endl;
      break;
    }
  }

  std::cout << "[openSEF] Event loop ended" << std::endl;
}

void OSFBackend::runWithCallback(std::function<void()> callback,
                                 int intervalMs) {
  if (!g_display)
    return;

  g_running = true;
  running_ = true;
  std::cout << "[openSEF] Entering animated event loop..." << std::endl;

  int fd = wl_display_get_fd(g_display);
  struct pollfd pfd = {fd, POLLIN, 0};

  auto lastCallback = std::chrono::steady_clock::now();

  while (g_running && g_connected) {
    // Flush pending requests
    wl_display_flush(g_display);

    // Poll with timeout
    int timeout = intervalMs;
    int ret = poll(&pfd, 1, timeout);

    if (ret < 0) {
      std::cerr << "[openSEF] Poll error" << std::endl;
      break;
    }

    // Handle Wayland events if available
    if (ret > 0 && (pfd.revents & POLLIN)) {
      if (wl_display_dispatch(g_display) < 0) {
        std::cerr << "[openSEF] Display error" << std::endl;
        break;
      }
    }

    // Call animation callback at interval
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       now - lastCallback)
                       .count();
    if (elapsed >= intervalMs) {
      callback();
      lastCallback = now;
    }
  }

  std::cout << "[openSEF] Event loop ended" << std::endl;
}

// Stubs
OSFVulkanRenderer &OSFVulkanRenderer::shared() {
  static OSFVulkanRenderer instance;
  return instance;
}
bool OSFVulkanRenderer::initialize() { return true; }
void OSFVulkanRenderer::shutdown() {}

OSFInputHandler &OSFInputHandler::shared() {
  static OSFInputHandler instance;
  return instance;
}

} // namespace opensef
