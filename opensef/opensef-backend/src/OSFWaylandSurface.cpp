/**
 * OSFWaylandSurface.cpp - XDG toplevel window with shared memory buffer
 */

#include <cstring>
#include <iostream>
#include <opensef/OpenSEFBackend.h>
#include <sys/mman.h>
#include <unistd.h>


#include "xdg-shell-client-protocol.h"
#include <wayland-client.h>


namespace opensef {

// XDG surface callbacks
static void xdgSurfaceHandleConfigure(void *data, xdg_surface *surface,
                                      uint32_t serial) {
  OSFWaylandSurface *self = static_cast<OSFWaylandSurface *>(data);
  xdg_surface_ack_configure(surface, serial);
  self->onConfigure(serial);
}

static const xdg_surface_listener xdgSurfaceListener = {
    xdgSurfaceHandleConfigure};

// XDG toplevel callbacks
static void xdgToplevelHandleConfigure(void *data, xdg_toplevel *toplevel,
                                       int32_t width, int32_t height,
                                       wl_array *states) {
  (void)data;
  (void)toplevel;
  (void)width;
  (void)height;
  (void)states;
  // We ignore resize requests for now
}

static void xdgToplevelHandleClose(void *data, xdg_toplevel *toplevel) {
  (void)toplevel;
  OSFWaylandSurface *self = static_cast<OSFWaylandSurface *>(data);
  self->onClose();
}

static const xdg_toplevel_listener xdgToplevelListener = {
    xdgToplevelHandleConfigure, xdgToplevelHandleClose};

// Create shared memory file
static int createShmFile(size_t size) {
  int fd = memfd_create("opensef-buffer", MFD_CLOEXEC);
  if (fd < 0) {
    char name[] = "/dev/shm/opensef-XXXXXX";
    fd = mkstemp(name);
    if (fd >= 0)
      unlink(name);
  }
  if (fd >= 0)
    ftruncate(fd, size);
  return fd;
}

OSFWaylandSurface::OSFWaylandSurface() = default;

OSFWaylandSurface::~OSFWaylandSurface() { destroy(); }

std::shared_ptr<OSFWaylandSurface>
OSFWaylandSurface::create(int width, int height, const std::string &title) {
  auto surface = std::make_shared<OSFWaylandSurface>();
  if (!surface->initialize(width, height, title)) {
    return nullptr;
  }
  return surface;
}

bool OSFWaylandSurface::createBuffer() {
  int stride = width_ * 4;
  shmSize_ = stride * height_;

  shmFd_ = createShmFile(shmSize_);
  if (shmFd_ < 0)
    return false;

  shmData_ =
      mmap(nullptr, shmSize_, PROT_READ | PROT_WRITE, MAP_SHARED, shmFd_, 0);
  if (shmData_ == MAP_FAILED) {
    close(shmFd_);
    shmFd_ = -1;
    return false;
  }

  auto &backend = OSFBackend::shared();
  wl_shm *shm = static_cast<wl_shm *>(backend.shm());
  shmPool_ = wl_shm_create_pool(shm, shmFd_, shmSize_);
  buffer_ =
      wl_shm_pool_create_buffer(static_cast<wl_shm_pool *>(shmPool_), 0, width_,
                                height_, stride, WL_SHM_FORMAT_ARGB8888);

  return true;
}

void OSFWaylandSurface::destroyBuffer() {
  if (buffer_) {
    wl_buffer_destroy(static_cast<wl_buffer *>(buffer_));
    buffer_ = nullptr;
  }
  if (shmPool_) {
    wl_shm_pool_destroy(static_cast<wl_shm_pool *>(shmPool_));
    shmPool_ = nullptr;
  }
  if (shmData_ && shmData_ != MAP_FAILED) {
    munmap(shmData_, shmSize_);
    shmData_ = nullptr;
  }
  if (shmFd_ >= 0) {
    close(shmFd_);
    shmFd_ = -1;
  }
}

bool OSFWaylandSurface::initialize(int width, int height,
                                   const std::string &title) {
  width_ = width;
  height_ = height;
  title_ = title;

  auto &backend = OSFBackend::shared();
  wl_compositor *compositor =
      static_cast<wl_compositor *>(backend.compositor());
  xdg_wm_base *xdgWmBase = static_cast<xdg_wm_base *>(backend.xdgWmBase());

  wlSurface_ = wl_compositor_create_surface(compositor);
  if (!wlSurface_)
    return false;

  xdgSurface_ = xdg_wm_base_get_xdg_surface(
      xdgWmBase, static_cast<wl_surface *>(wlSurface_));
  if (!xdgSurface_)
    return false;
  xdg_surface_add_listener(static_cast<xdg_surface *>(xdgSurface_),
                           &xdgSurfaceListener, this);

  xdgToplevel_ =
      xdg_surface_get_toplevel(static_cast<xdg_surface *>(xdgSurface_));
  if (!xdgToplevel_)
    return false;
  xdg_toplevel_add_listener(static_cast<xdg_toplevel *>(xdgToplevel_),
                            &xdgToplevelListener, this);
  xdg_toplevel_set_title(static_cast<xdg_toplevel *>(xdgToplevel_),
                         title.c_str());
  xdg_toplevel_set_app_id(static_cast<xdg_toplevel *>(xdgToplevel_),
                          "vitusos.opensef");

  if (!createBuffer())
    return false;

  wl_surface_commit(static_cast<wl_surface *>(wlSurface_));
  wl_display_roundtrip(static_cast<wl_display *>(backend.display()));

  std::cout << "[openSEF] Window '" << title << "' created (" << width << "x"
            << height << ")" << std::endl;
  return true;
}

void OSFWaylandSurface::destroy() {
  destroyBuffer();
  if (xdgToplevel_) {
    xdg_toplevel_destroy(static_cast<xdg_toplevel *>(xdgToplevel_));
    xdgToplevel_ = nullptr;
  }
  if (xdgSurface_) {
    xdg_surface_destroy(static_cast<xdg_surface *>(xdgSurface_));
    xdgSurface_ = nullptr;
  }
  if (wlSurface_) {
    wl_surface_destroy(static_cast<wl_surface *>(wlSurface_));
    wlSurface_ = nullptr;
  }
}

void OSFWaylandSurface::setTitle(const std::string &title) {
  title_ = title;
  if (xdgToplevel_) {
    xdg_toplevel_set_title(static_cast<xdg_toplevel *>(xdgToplevel_),
                           title.c_str());
  }
}

void OSFWaylandSurface::commit() {
  if (wlSurface_ && buffer_) {
    wl_surface_attach(static_cast<wl_surface *>(wlSurface_),
                      static_cast<wl_buffer *>(buffer_), 0, 0);
    wl_surface_damage_buffer(static_cast<wl_surface *>(wlSurface_), 0, 0,
                             width_, height_);
    wl_surface_commit(static_cast<wl_surface *>(wlSurface_));
  }
}

void OSFWaylandSurface::draw(const OSFColor &color) {
  if (!shmData_)
    return;

  uint32_t pixel = (uint32_t)(color.a * 255) << 24 |
                   (uint32_t)(color.r * 255) << 16 |
                   (uint32_t)(color.g * 255) << 8 | (uint32_t)(color.b * 255);

  uint32_t *pixels = static_cast<uint32_t *>(shmData_);
  for (int i = 0; i < width_ * height_; i++) {
    pixels[i] = pixel;
  }

  commit();
}

void OSFWaylandSurface::onConfigure(uint32_t serial) {
  (void)serial;
  configured_ = true;
  std::cout << "[openSEF] Surface configured" << std::endl;
}

void OSFWaylandSurface::onClose() {
  shouldClose_ = true;
  std::cout << "[openSEF] Close requested" << std::endl;
  OSFBackend::shared().stop();
}

} // namespace opensef
