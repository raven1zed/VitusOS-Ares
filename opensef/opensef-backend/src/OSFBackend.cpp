/**
 * OSFBackend.cpp - Wayland backend with XDG shell
 */

#include <cstring>
#include <iostream>
#include <opensef/OpenSEFBackend.h>

#include "xdg-shell-client-protocol.h"
#include <wayland-client.h>

namespace opensef {

// Global Wayland objects (set by registry)
static wl_compositor *g_compositor = nullptr;
static wl_shm *g_shm = nullptr;
static xdg_wm_base *g_xdgWmBase = nullptr;

// Backend singleton storage
static wl_display *g_display = nullptr;
static wl_registry *g_registry = nullptr;
static bool g_connected = false;
static bool g_running = false;

// XDG WM base ping handler
static void xdgWmBasePing(void *data, xdg_wm_base *xdg_wm_base,
                          uint32_t serial) {
  (void)data;
  xdg_wm_base_pong(xdg_wm_base, serial);
}

static const xdg_wm_base_listener xdgWmBaseListener = {xdgWmBasePing};

// Registry listener
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
