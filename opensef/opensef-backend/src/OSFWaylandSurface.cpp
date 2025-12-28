/**
 * OSFWaylandSurface.cpp - XDG toplevel window with shared memory buffer
 */

#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <opensef/OpenSEFBackend.h>
#include <sys/mman.h>
#include <unistd.h>


#include "xdg-shell-client-protocol.h"
#include <wayland-client.h>


namespace opensef {

// XDG surface listener
static const xdg_surface_listener xdgSurfaceListener = {
    OSFWaylandSurface::xdgSurfaceConfigure};

// XDG toplevel listener
static const xdg_toplevel_listener xdgToplevelListener = {
    OSFWaylandSurface::xdgToplevelConfigure,
    OSFWaylandSurface::xdgToplevelClose};

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

// Create anonymous file for shared memory
static int createShmFile(size_t size) {
  // Try memfd_create first (Linux 3.17+)
  int fd = memfd_create("opensef-buffer", MFD_CLOEXEC);
  if (fd < 0) {
    // Fallback to /dev/shm
    char name[] = "/dev/shm/opensef-XXXXXX";
    fd = mkstemp(name);
    if (fd >= 0) {
      unlink(name);
    }
  }

  if (fd < 0) {
    std::cerr << "[openSEF] Failed to create shared memory file" << std::endl;
    return -1;
  }

  if (ftruncate(fd, size) < 0) {
    std::cerr << "[openSEF] Failed to resize shared memory: " << strerror(errno)
              << std::endl;
    close(fd);
    return -1;
  }

  return fd;
}

bool OSFWaylandSurface::createBuffer() {
  int stride = width_ * 4; // ARGB8888
  shmSize_ = stride * height_;

  shmFd_ = createShmFile(shmSize_);
  if (shmFd_ < 0) {
    return false;
  }

  shmData_ =
      mmap(nullptr, shmSize_, PROT_READ | PROT_WRITE, MAP_SHARED, shmFd_, 0);
  if (shmData_ == MAP_FAILED) {
    std::cerr << "[openSEF] Failed to mmap shared memory" << std::endl;
    close(shmFd_);
    shmFd_ = -1;
    return false;
  }

  auto &backend = OSFBackend::shared();
  shmPool_ = wl_shm_create_pool(backend.shm(), shmFd_, shmSize_);
  buffer_ = wl_shm_pool_create_buffer(shmPool_, 0, width_, height_, stride,
                                      WL_SHM_FORMAT_ARGB8888);

  std::cout << "[openSEF] Created " << width_ << "x" << height_ << " buffer"
            << std::endl;
  return true;
}

void OSFWaylandSurface::destroyBuffer() {
  if (buffer_) {
    wl_buffer_destroy(buffer_);
    buffer_ = nullptr;
  }
  if (shmPool_) {
    wl_shm_pool_destroy(shmPool_);
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

  // Create Wayland surface
  wlSurface_ = wl_compositor_create_surface(backend.compositor());
  if (!wlSurface_) {
    std::cerr << "[openSEF] Failed to create Wayland surface" << std::endl;
    return false;
  }

  // Create XDG surface
  xdgSurface_ = xdg_wm_base_get_xdg_surface(backend.xdgWmBase(), wlSurface_);
  if (!xdgSurface_) {
    std::cerr << "[openSEF] Failed to create XDG surface" << std::endl;
    return false;
  }
  xdg_surface_add_listener(xdgSurface_, &xdgSurfaceListener, this);

  // Create XDG toplevel (actual window)
  xdgToplevel_ = xdg_surface_get_toplevel(xdgSurface_);
  if (!xdgToplevel_) {
    std::cerr << "[openSEF] Failed to create XDG toplevel" << std::endl;
    return false;
  }
  xdg_toplevel_add_listener(xdgToplevel_, &xdgToplevelListener, this);
  xdg_toplevel_set_title(xdgToplevel_, title.c_str());
  xdg_toplevel_set_app_id(xdgToplevel_, "vitusos.opensef");

  // Create buffer
  if (!createBuffer()) {
    return false;
  }

  // Commit to trigger configure
  wl_surface_commit(wlSurface_);

  // Wait for configure
  wl_display_roundtrip(backend.display());

  std::cout << "[openSEF] Window '" << title << "' created" << std::endl;
  return true;
}

void OSFWaylandSurface::destroy() {
  destroyBuffer();

  if (xdgToplevel_) {
    xdg_toplevel_destroy(xdgToplevel_);
    xdgToplevel_ = nullptr;
  }
  if (xdgSurface_) {
    xdg_surface_destroy(xdgSurface_);
    xdgSurface_ = nullptr;
  }
  if (wlSurface_) {
    wl_surface_destroy(wlSurface_);
    wlSurface_ = nullptr;
  }
}

void OSFWaylandSurface::setTitle(const std::string &title) {
  title_ = title;
  if (xdgToplevel_) {
    xdg_toplevel_set_title(xdgToplevel_, title.c_str());
  }
}

void OSFWaylandSurface::commit() {
  if (wlSurface_ && buffer_) {
    wl_surface_attach(wlSurface_, buffer_, 0, 0);
    wl_surface_damage_buffer(wlSurface_, 0, 0, width_, height_);
    wl_surface_commit(wlSurface_);
  }
}

void OSFWaylandSurface::draw(const OSFColor &color) {
  if (!shmData_)
    return;

  // Convert to ARGB8888
  uint32_t pixel = (uint32_t)(color.a * 255) << 24 |
                   (uint32_t)(color.r * 255) << 16 |
                   (uint32_t)(color.g * 255) << 8 | (uint32_t)(color.b * 255);

  // Fill buffer with color
  uint32_t *pixels = static_cast<uint32_t *>(shmData_);
  int count = width_ * height_;
  for (int i = 0; i < count; i++) {
    pixels[i] = pixel;
  }

  commit();
}

// XDG callbacks
void OSFWaylandSurface::xdgSurfaceConfigure(void *data, xdg_surface *surface,
                                            uint32_t serial) {
  OSFWaylandSurface *self = static_cast<OSFWaylandSurface *>(data);
  xdg_surface_ack_configure(surface, serial);
  self->configured_ = true;
  std::cout << "[openSEF] Surface configured" << std::endl;
}

void OSFWaylandSurface::xdgToplevelConfigure(void *data, xdg_toplevel *toplevel,
                                             int32_t width, int32_t height,
                                             wl_array *states) {
  OSFWaylandSurface *self = static_cast<OSFWaylandSurface *>(data);

  if (width > 0 && height > 0) {
    // Compositor wants us to resize
    // For now, ignore and keep our size
  }
}

void OSFWaylandSurface::xdgToplevelClose(void *data, xdg_toplevel *toplevel) {
  OSFWaylandSurface *self = static_cast<OSFWaylandSurface *>(data);
  self->shouldClose_ = true;
  std::cout << "[openSEF] Window close requested" << std::endl;
  OSFBackend::shared().stop();
}

} // namespace opensef
