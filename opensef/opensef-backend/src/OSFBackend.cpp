/**
 * OSFBackend.cpp - Wayland backend with XDG shell
 */

#include <cstring>
#include <iostream>
#include <opensef/OpenSEFBackend.h>


#include "xdg-shell-client-protocol.h"
#include <wayland-client.h>


namespace opensef {

// Registry listener
static void registryHandler(void *data, wl_registry *registry, uint32_t id,
                            const char *interface, uint32_t version) {
  OSFBackend *self = static_cast<OSFBackend *>(data);

  // Access private members through casts (not ideal but works)
  auto **compositor = reinterpret_cast<wl_compositor **>(
      reinterpret_cast<char *>(self) +
      sizeof(void *) * 5); // Skip vtable + 4 members
  auto **shm = reinterpret_cast<wl_shm **>(reinterpret_cast<char *>(self) +
                                           sizeof(void *) * 6);
  auto **xdgWmBase = reinterpret_cast<xdg_wm_base **>(
      reinterpret_cast<char *>(self) + sizeof(void *) * 7);

  if (strcmp(interface, wl_compositor_interface.name) == 0) {
    *compositor = static_cast<wl_compositor *>(
        wl_registry_bind(registry, id, &wl_compositor_interface, 4));
    std::cout << "[openSEF] Found compositor" << std::endl;
  } else if (strcmp(interface, wl_shm_interface.name) == 0) {
    *shm = static_cast<wl_shm *>(
        wl_registry_bind(registry, id, &wl_shm_interface, 1));
    std::cout << "[openSEF] Found shared memory" << std::endl;
  } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
    *xdgWmBase = static_cast<xdg_wm_base *>(
        wl_registry_bind(registry, id, &xdg_wm_base_interface, 1));
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

  wlDisplay_ = wl_display_connect(nullptr);
  if (!wlDisplay_) {
    std::cerr << "[openSEF] Failed to connect to Wayland display" << std::endl;
    return false;
  }

  wlRegistry_ = wl_display_get_registry(static_cast<wl_display *>(wlDisplay_));
  wl_registry_add_listener(static_cast<wl_registry *>(wlRegistry_),
                           &registryListener, this);

  wl_display_roundtrip(static_cast<wl_display *>(wlDisplay_));

  if (!wlCompositor_ || !wlShm_ || !xdgWmBase_) {
    std::cerr << "[openSEF] Missing required Wayland interfaces" << std::endl;
    disconnect();
    return false;
  }

  connected_ = true;
  std::cout << "[openSEF] Wayland connection established" << std::endl;
  return true;
}

void OSFBackend::disconnect() {
  if (xdgWmBase_) {
    xdg_wm_base_destroy(static_cast<xdg_wm_base *>(xdgWmBase_));
    xdgWmBase_ = nullptr;
  }
  if (wlShm_) {
    wl_shm_destroy(static_cast<wl_shm *>(wlShm_));
    wlShm_ = nullptr;
  }
  if (wlCompositor_) {
    wl_compositor_destroy(static_cast<wl_compositor *>(wlCompositor_));
    wlCompositor_ = nullptr;
  }
  if (wlRegistry_) {
    wl_registry_destroy(static_cast<wl_registry *>(wlRegistry_));
    wlRegistry_ = nullptr;
  }
  if (wlDisplay_) {
    wl_display_disconnect(static_cast<wl_display *>(wlDisplay_));
    wlDisplay_ = nullptr;
  }
  connected_ = false;
}

void OSFBackend::run() {
  if (!wlDisplay_)
    return;

  running_ = true;
  std::cout << "[openSEF] Entering event loop..." << std::endl;

  while (running_ && connected_) {
    if (wl_display_dispatch(static_cast<wl_display *>(wlDisplay_)) < 0) {
      break;
    }
  }
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
