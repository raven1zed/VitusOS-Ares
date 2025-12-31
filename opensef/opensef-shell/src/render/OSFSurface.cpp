#include "OSFSurface.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>

// Generated protocol headers
#include "wlr-layer-shell-unstable-v1-client-protocol.h"
#include "xdg-output-unstable-v1-client-protocol.h"

namespace opensef {

// Forward declarations of listener structs
// Forward declarations of listener structs
extern const struct wl_registry_listener registry_listener;
extern const struct zwlr_layer_surface_v1_listener layer_surface_listener;

// Helper to create a shared memory file
static int create_shm_file(off_t size) {
  std::string name = "/tmp/osf-shm-XXXXXX";
  char *path = new char[name.length() + 1];
  strcpy(path, name.c_str());

  int fd = mkstemp(path);
  if (fd < 0) {
    delete[] path;
    return -1;
  }

  unlink(path);
  delete[] path;

  if (ftruncate(fd, size) < 0) {
    close(fd);
    return -1;
  }

  return fd;
}

OSFSurface::OSFSurface(const std::string &namespace_name)
    : namespace_(namespace_name) {}

OSFSurface::~OSFSurface() { disconnect(); }

bool OSFSurface::connect(const char *display_name) {
  display_ = wl_display_connect(display_name);
  if (!display_) {
    std::cerr << "Failed to connect to Wayland display" << std::endl;
    return false;
  }

  registry_ = wl_display_get_registry(display_);
  wl_registry_add_listener(registry_, &registry_listener, this);

  // Initial roundtrip to get globals
  wl_display_roundtrip(display_);

  if (!compositor_ || !shm_ || !layerShell_) {
    std::cerr
        << "Missing required Wayland globals (compositor, shm, or layer-shell)"
        << std::endl;
    return false;
  }

  surface_ = wl_compositor_create_surface(compositor_);
  if (!surface_) {
    std::cerr << "Failed to create wl_surface" << std::endl;
    return false;
  }

  // Create layer surface
  layerSurface_ = zwlr_layer_shell_v1_get_layer_surface(
      layerShell_, surface_, nullptr,
      static_cast<zwlr_layer_shell_v1_layer>(layer_), namespace_.c_str());

  if (!layerSurface_) {
    std::cerr << "Failed to create layer surface" << std::endl;
    return false;
  }

  // Configure layer surface
  zwlr_layer_surface_v1_set_size(layerSurface_, width_, height_);
  zwlr_layer_surface_v1_set_anchor(layerSurface_,
                                   static_cast<uint32_t>(anchor_));
  zwlr_layer_surface_v1_set_exclusive_zone(layerSurface_, exclusiveZone_);
  zwlr_layer_surface_v1_set_margin(layerSurface_, marginTop_, marginRight_,
                                   marginBottom_, marginLeft_);

  // Add listener
  zwlr_layer_surface_v1_add_listener(layerSurface_, &layer_surface_listener,
                                     this);

  // Commit to apply changes
  wl_surface_commit(surface_);
  wl_display_roundtrip(display_);

  running_ = true;
  return true;
}

void OSFSurface::disconnect() {
  running_ = false;
  destroyShmBuffer();

  if (layerSurface_) {
    zwlr_layer_surface_v1_destroy(layerSurface_);
    layerSurface_ = nullptr;
  }
  if (surface_) {
    wl_surface_destroy(surface_);
    surface_ = nullptr;
  }
  if (layerShell_) {
    zwlr_layer_shell_v1_destroy(layerShell_);
    layerShell_ = nullptr;
  }
  if (shm_) {
    wl_shm_destroy(shm_);
    shm_ = nullptr;
  }
  if (compositor_) {
    wl_compositor_destroy(compositor_);
    compositor_ = nullptr;
  }
  if (registry_) {
    wl_registry_destroy(registry_);
    registry_ = nullptr;
  }
  if (display_) {
    wl_display_disconnect(display_);
    display_ = nullptr;
  }
}

void OSFSurface::setLayer(Layer layer) {
  layer_ = layer;
  if (layerSurface_) {
    zwlr_layer_surface_v1_set_layer(layerSurface_,
                                    static_cast<uint32_t>(layer_));
    wl_surface_commit(surface_);
  }
}

void OSFSurface::setAnchor(Anchor anchor) {
  anchor_ = anchor;
  if (layerSurface_) {
    zwlr_layer_surface_v1_set_anchor(layerSurface_,
                                     static_cast<uint32_t>(anchor_));
    wl_surface_commit(surface_);
  }
}

void OSFSurface::setSize(int width, int height) {
  width_ = width;
  height_ = height;
  if (layerSurface_) {
    zwlr_layer_surface_v1_set_size(layerSurface_, width_, height_);
    wl_surface_commit(surface_);
  }
}

void OSFSurface::setExclusiveZone(int zone) {
  exclusiveZone_ = zone;
  if (layerSurface_) {
    zwlr_layer_surface_v1_set_exclusive_zone(layerSurface_, exclusiveZone_);
    wl_surface_commit(surface_);
  }
}

void OSFSurface::setMargin(int top, int right, int bottom, int left) {
  marginTop_ = top;
  marginRight_ = right;
  marginBottom_ = bottom;
  marginLeft_ = left;
  if (layerSurface_) {
    zwlr_layer_surface_v1_set_margin(layerSurface_, top, right, bottom, left);
    wl_surface_commit(surface_);
  }
}

cairo_t *OSFSurface::beginPaint() {
  if (!configuredWidth_ || !configuredHeight_)
    return nullptr;

  if (configuredWidth_ != width_ || configuredHeight_ != height_ ||
      !cairoSurface_) {
    // Resize buffer if needed
    destroyShmBuffer();
    if (!createShmBuffer(configuredWidth_, configuredHeight_)) {
      return nullptr;
    }
    width_ = configuredWidth_;
    height_ = configuredHeight_;
  }

  return cairo_create(cairoSurface_);
}

void OSFSurface::endPaint() {
  // Nothing logic specific here, cairo_t is destroyed by caller usually?
  // Actually, beginPaint should probably return a managed pointer or the caller
  // destroys it. In this API design, caller gets a raw pointer. They should
  // destroy it with cairo_destroy. But let's assume standard usage pattern.
}

void OSFSurface::damage(int x, int y, int width, int height) {
  wl_surface_damage(surface_, x, y, width, height);
}

void OSFSurface::commit() {
  wl_surface_attach(surface_, buffer_, 0, 0);
  wl_surface_commit(surface_);
}

void OSFSurface::run() {
  while (running_ && wl_display_dispatch(display_) != -1) {
    // Event loop
  }
}

void OSFSurface::stop() { running_ = false; }

bool OSFSurface::createShmBuffer(int width, int height) {
  int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);
  shmSize_ = stride * height;

  shmFd_ = create_shm_file(shmSize_);
  if (shmFd_ < 0)
    return false;

  shmData_ =
      mmap(NULL, shmSize_, PROT_READ | PROT_WRITE, MAP_SHARED, shmFd_, 0);
  if (shmData_ == MAP_FAILED) {
    close(shmFd_);
    return false;
  }

  struct wl_shm_pool *pool = wl_shm_create_pool(shm_, shmFd_, shmSize_);
  buffer_ = wl_shm_pool_create_buffer(pool, 0, width, height, stride,
                                      WL_SHM_FORMAT_ARGB8888);
  wl_shm_pool_destroy(pool);
  close(shmFd_); // FD can be closed after pool creation

  cairoSurface_ = cairo_image_surface_create_for_data(
      static_cast<unsigned char *>(shmData_), CAIRO_FORMAT_ARGB32, width,
      height, stride);

  return true;
}

void OSFSurface::destroyShmBuffer() {
  if (cairoSurface_) {
    cairo_surface_destroy(cairoSurface_);
    cairoSurface_ = nullptr;
  }
  if (buffer_) {
    wl_buffer_destroy(buffer_);
    buffer_ = nullptr;
  }
  if (shmData_) {
    munmap(shmData_, shmSize_);
    shmData_ = nullptr;
  }
}

void OSFSurface::registryGlobal(void *data, struct wl_registry *registry,
                                uint32_t name, const char *interface,
                                uint32_t version) {
  auto self = static_cast<OSFSurface *>(data);
  if (strcmp(interface, wl_compositor_interface.name) == 0) {
    self->compositor_ = static_cast<wl_compositor *>(
        wl_registry_bind(registry, name, &wl_compositor_interface, 4));
  } else if (strcmp(interface, wl_shm_interface.name) == 0) {
    self->shm_ = static_cast<wl_shm *>(
        wl_registry_bind(registry, name, &wl_shm_interface, 1));
  } else if (strcmp(interface, zwlr_layer_shell_v1_interface.name) == 0) {
    self->layerShell_ = static_cast<zwlr_layer_shell_v1 *>(
        wl_registry_bind(registry, name, &zwlr_layer_shell_v1_interface, 1));
  }
}

void OSFSurface::registryGlobalRemove(void *data, struct wl_registry *registry,
                                      uint32_t name) {
  // Handle global removal if necessary
}

void OSFSurface::layerSurfaceConfigure(void *data,
                                       struct zwlr_layer_surface_v1 *surface,
                                       uint32_t serial, uint32_t width,
                                       uint32_t height) {
  auto self = static_cast<OSFSurface *>(data);
  self->configuredWidth_ = width;
  self->configuredHeight_ = height;
  self->configured_ = true;

  // Acknowledge configure
  zwlr_layer_surface_v1_ack_configure(surface, serial);

  // Trigger user callback if set
  if (self->configureCallback_) {
    self->configureCallback_(width, height);
  }

  // Auto-paint on configure (basic frame loop)
  if (self->drawCallback_) {
    cairo_t *cr = self->beginPaint();
    if (cr) {
      self->drawCallback_(cr, width, height);
      cairo_destroy(cr); // Clean up cairo context
      self->endPaint();
      self->damage(0, 0, width, height);
      self->commit();
    }
  }
}

void OSFSurface::layerSurfaceClosed(void *data,
                                    struct zwlr_layer_surface_v1 *surface) {
  auto self = static_cast<OSFSurface *>(data);
  self->running_ = false;
  if (self->closeCallback_) {
    self->closeCallback_();
  }
}

// Logic definitions
const struct wl_registry_listener registry_listener = {
    .global = OSFSurface::registryGlobal,
    .global_remove = OSFSurface::registryGlobalRemove,
};

const struct zwlr_layer_surface_v1_listener layer_surface_listener = {
    .configure = OSFSurface::layerSurfaceConfigure,
    .closed = OSFSurface::layerSurfaceClosed};

} // namespace opensef
