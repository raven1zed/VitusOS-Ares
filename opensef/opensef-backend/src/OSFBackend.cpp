/**
 * OSFBackend.cpp - Wayland backend with XDG shell
 *
 * Creates actual visible windows on Wayland compositors
 */

#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <opensef/OpenSEFBackend.h>
#include <sys/mman.h>
#include <unistd.h>


#include "xdg-shell-client-protocol.h"
#include <wayland-client.h>


namespace opensef {

// ============================================================================
// OSFBackend
// ============================================================================

static const wl_registry_listener registryListener = {
    OSFBackend::registryHandler, OSFBackend::registryRemover};

OSFBackend &OSFBackend::shared() {
  static OSFBackend instance;
  return instance;
}

void OSFBackend::registryHandler(void *data, wl_registry *registry, uint32_t id,
                                 const char *interface, uint32_t version) {
  OSFBackend *self = static_cast<OSFBackend *>(data);

  if (strcmp(interface, wl_compositor_interface.name) == 0) {
    self->wlCompositor_ = static_cast<wl_compositor *>(
        wl_registry_bind(registry, id, &wl_compositor_interface, 4));
    std::cout << "[openSEF] Found compositor" << std::endl;
  } else if (strcmp(interface, wl_shm_interface.name) == 0) {
    self->wlShm_ = static_cast<wl_shm *>(
        wl_registry_bind(registry, id, &wl_shm_interface, 1));
    std::cout << "[openSEF] Found shared memory" << std::endl;
  } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
    self->xdgWmBase_ = static_cast<xdg_wm_base *>(
        wl_registry_bind(registry, id, &xdg_wm_base_interface, 1));
    std::cout << "[openSEF] Found XDG shell" << std::endl;
  }
}

void OSFBackend::registryRemover(void *data, wl_registry *registry,
                                 uint32_t id) {
  // Handle global removal
}

bool OSFBackend::connect() {
  std::cout << "[openSEF] Connecting to Wayland display..." << std::endl;

  wlDisplay_ = wl_display_connect(nullptr);
  if (!wlDisplay_) {
    std::cerr << "[openSEF] Failed to connect to Wayland display" << std::endl;
    std::cerr << "[openSEF] Make sure you're running in a Wayland session"
              << std::endl;
    return false;
  }

  wlRegistry_ = wl_display_get_registry(wlDisplay_);
  wl_registry_add_listener(wlRegistry_, &registryListener, this);

  // Round-trip to get globals
  wl_display_roundtrip(wlDisplay_);

  if (!wlCompositor_ || !wlShm_ || !xdgWmBase_) {
    std::cerr << "[openSEF] Missing required Wayland interfaces:" << std::endl;
    if (!wlCompositor_)
      std::cerr << "  - wl_compositor" << std::endl;
    if (!wlShm_)
      std::cerr << "  - wl_shm" << std::endl;
    if (!xdgWmBase_)
      std::cerr << "  - xdg_wm_base" << std::endl;
    disconnect();
    return false;
  }

  connected_ = true;
  std::cout << "[openSEF] Wayland connection established" << std::endl;
  return true;
}

void OSFBackend::disconnect() {
  if (xdgWmBase_) {
    xdg_wm_base_destroy(xdgWmBase_);
    xdgWmBase_ = nullptr;
  }
  if (wlShm_) {
    wl_shm_destroy(wlShm_);
    wlShm_ = nullptr;
  }
  if (wlCompositor_) {
    wl_compositor_destroy(wlCompositor_);
    wlCompositor_ = nullptr;
  }
  if (wlRegistry_) {
    wl_registry_destroy(wlRegistry_);
    wlRegistry_ = nullptr;
  }
  if (wlDisplay_) {
    wl_display_disconnect(wlDisplay_);
    wlDisplay_ = nullptr;
  }
  connected_ = false;
  std::cout << "[openSEF] Wayland disconnected" << std::endl;
}

void OSFBackend::run() {
  if (!wlDisplay_)
    return;

  running_ = true;
  std::cout << "[openSEF] Entering event loop..." << std::endl;

  while (running_ && connected_) {
    if (wl_display_dispatch(wlDisplay_) < 0) {
      std::cerr << "[openSEF] Display dispatch error" << std::endl;
      break;
    }
  }

  std::cout << "[openSEF] Event loop ended" << std::endl;
}

// ============================================================================
// OSFVulkanRenderer (stub)
// ============================================================================

OSFVulkanRenderer &OSFVulkanRenderer::shared() {
  static OSFVulkanRenderer instance;
  return instance;
}

bool OSFVulkanRenderer::initialize() {
  std::cout << "[openSEF] Vulkan: using shared memory for now" << std::endl;
  available_ = false; // Not actually using Vulkan yet
  return true;
}

void OSFVulkanRenderer::shutdown() { available_ = false; }

// ============================================================================
// OSFInputHandler
// ============================================================================

OSFInputHandler &OSFInputHandler::shared() {
  static OSFInputHandler instance;
  return instance;
}

} // namespace opensef
