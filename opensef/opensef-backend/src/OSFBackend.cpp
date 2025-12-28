/**
 * OSFBackend.cpp - Backend implementation
 */

#include <iostream>
#include <opensef/OpenSEFBackend.h>
#include <wayland-client.h>


namespace opensef {

OSFBackend &OSFBackend::shared() {
  static OSFBackend instance;
  return instance;
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

  wlRegistry_ = wl_display_get_registry(static_cast<wl_display *>(wlDisplay_));
  if (!wlRegistry_) {
    std::cerr << "[openSEF] Failed to get Wayland registry" << std::endl;
    wl_display_disconnect(static_cast<wl_display *>(wlDisplay_));
    wlDisplay_ = nullptr;
    return false;
  }

  connected_ = true;
  std::cout << "[openSEF] Wayland connection established" << std::endl;
  return true;
}

void OSFBackend::disconnect() {
  if (wlDisplay_) {
    wl_display_disconnect(static_cast<wl_display *>(wlDisplay_));
    wlDisplay_ = nullptr;
    wlRegistry_ = nullptr;
    connected_ = false;
    std::cout << "[openSEF] Wayland disconnected" << std::endl;
  }
}

void OSFBackend::run() {
  if (!wlDisplay_)
    return;

  std::cout << "[openSEF] Entering event loop..." << std::endl;

  // Simple event loop
  while (connected_) {
    if (wl_display_dispatch(static_cast<wl_display *>(wlDisplay_)) < 0) {
      break;
    }
  }
}

} // namespace opensef
