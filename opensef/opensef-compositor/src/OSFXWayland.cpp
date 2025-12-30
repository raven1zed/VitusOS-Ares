/**
 * OSFXWayland.cpp - XWayland Integration
 *
 * Enables X11 application support in the openSEF compositor.
 */

#include "OSFXWayland.h"
#include "OSFCompositor.h"

#include <iostream>

extern "C" {
#include <wlr/types/wlr_scene.h>
#include <wlr/xwayland.h>

}

namespace opensef {

// Helper macro
#define WL_CONTAINER_OF(ptr, type, member)                                     \
  reinterpret_cast<type *>(reinterpret_cast<char *>(ptr) -                     \
                           offsetof(type, member))

// ============================================================================
// OSFXWayland
// ============================================================================

OSFXWayland::OSFXWayland(OSFCompositor *compositor) : compositor_(compositor) {}

OSFXWayland::~OSFXWayland() { shutdown(); }

bool OSFXWayland::initialize() {
  // Create XWayland server
  xwayland_ = wlr_xwayland_create(compositor_->display(),
                                  compositor_->compositor_wlr(), false);
  if (!xwayland_) {
    std::cerr << "[openSEF] Failed to create XWayland server" << std::endl;
    return false;
  }

  // Set up event listeners
  ready_.notify = handleReady;
  wl_signal_add(&xwayland_->events.ready, &ready_);

  newSurface_.notify = handleNewSurface;
  wl_signal_add(&xwayland_->events.new_surface, &newSurface_);

  std::cout << "[openSEF] XWayland initializing..." << std::endl;

  return true;
}

void OSFXWayland::shutdown() {
  if (xwayland_) {
    wlr_xwayland_destroy(xwayland_);
    xwayland_ = nullptr;
  }
  views_.clear();
}

const char *OSFXWayland::displayName() const {
  return xwayland_ ? xwayland_->display_name : nullptr;
}

void OSFXWayland::handleReady(wl_listener *listener, void *data) {
  OSFXWayland *self = WL_CONTAINER_OF(listener, OSFXWayland, ready_);
  (void)data;

  std::cout << "[openSEF] XWayland ready on display "
            << self->xwayland_->display_name << std::endl;

  // Set the DISPLAY environment variable for child processes
  setenv("DISPLAY", self->xwayland_->display_name, true);
}

void OSFXWayland::handleNewSurface(wl_listener *listener, void *data) {
  OSFXWayland *self = WL_CONTAINER_OF(listener, OSFXWayland, newSurface_);
  wlr_xwayland_surface *surface = static_cast<wlr_xwayland_surface *>(data);

  std::cout << "[openSEF] New XWayland surface" << std::endl;

  auto view = std::make_unique<OSFXWaylandView>(self, surface);
  self->views_.push_back(std::move(view));
}

// ============================================================================
// OSFXWaylandView
// ============================================================================

OSFXWaylandView::OSFXWaylandView(OSFXWayland *xwayland,
                                 wlr_xwayland_surface *surface)
    : xwayland_(xwayland), surface_(surface) {

  // Set up event listeners
  map_.notify = handleMap;
  wl_signal_add(&surface->surface->events.map, &map_);

  unmap_.notify = handleUnmap;
  wl_signal_add(&surface->surface->events.unmap, &unmap_);

  destroy_.notify = handleDestroy;
  wl_signal_add(&surface->events.destroy, &destroy_);

  requestMove_.notify = handleRequestMove;
  wl_signal_add(&surface->events.request_move, &requestMove_);

  requestResize_.notify = handleRequestResize;
  wl_signal_add(&surface->events.request_resize, &requestResize_);

  requestMaximize_.notify = handleRequestMaximize;
  wl_signal_add(&surface->events.request_maximize, &requestMaximize_);

  requestFullscreen_.notify = handleRequestFullscreen;
  wl_signal_add(&surface->events.request_fullscreen, &requestFullscreen_);

  setTitle_.notify = handleSetTitle;
  wl_signal_add(&surface->events.set_title, &setTitle_);
}

OSFXWaylandView::~OSFXWaylandView() {
  wl_list_remove(&map_.link);
  wl_list_remove(&unmap_.link);
  wl_list_remove(&destroy_.link);
  wl_list_remove(&requestMove_.link);
  wl_list_remove(&requestResize_.link);
  wl_list_remove(&requestMaximize_.link);
  wl_list_remove(&requestFullscreen_.link);
  wl_list_remove(&setTitle_.link);
}

void OSFXWaylandView::setPosition(int x, int y) {
  wlr_xwayland_surface_configure(surface_, x, y, surface_->width,
                                 surface_->height);
}

void OSFXWaylandView::setSize(int width, int height) {
  wlr_xwayland_surface_configure(surface_, surface_->x, surface_->y, width,
                                 height);
}

void OSFXWaylandView::focus() {
  wlr_xwayland_surface_activate(surface_, true);
  wlr_xwayland_surface_restack(surface_, nullptr, XCB_STACK_MODE_ABOVE);
}

void OSFXWaylandView::activate(bool active) {
  wlr_xwayland_surface_activate(surface_, active);
}

void OSFXWaylandView::setMaximized(bool maximized) {
  wlr_xwayland_surface_set_maximized(surface_, maximized);
}

void OSFXWaylandView::setFullscreen(bool fullscreen) {
  wlr_xwayland_surface_set_fullscreen(surface_, fullscreen);
}

void OSFXWaylandView::close() { wlr_xwayland_surface_close(surface_); }

// Event handlers

void OSFXWaylandView::handleMap(wl_listener *listener, void *data) {
  OSFXWaylandView *self = WL_CONTAINER_OF(listener, OSFXWaylandView, map_);
  (void)data;

  std::cout << "[openSEF] XWayland surface mapped: "
            << (self->surface_->title ? self->surface_->title : "untitled")
            << std::endl;

  // TODO: Add to scene graph and decorations
}

void OSFXWaylandView::handleUnmap(wl_listener *listener, void *data) {
  OSFXWaylandView *self = WL_CONTAINER_OF(listener, OSFXWaylandView, unmap_);
  (void)data;
  (void)self;

  // TODO: Remove from scene
}

void OSFXWaylandView::handleDestroy(wl_listener *listener, void *data) {
  OSFXWaylandView *self = WL_CONTAINER_OF(listener, OSFXWaylandView, destroy_);
  (void)data;
  (void)self;

  // View will be cleaned up by parent container
}

void OSFXWaylandView::handleRequestMove(wl_listener *listener, void *data) {
  (void)listener;
  (void)data;
  // TODO: Start interactive move
}

void OSFXWaylandView::handleRequestResize(wl_listener *listener, void *data) {
  (void)listener;
  (void)data;
  // TODO: Start interactive resize
}

void OSFXWaylandView::handleRequestMaximize(wl_listener *listener, void *data) {
  OSFXWaylandView *self =
      WL_CONTAINER_OF(listener, OSFXWaylandView, requestMaximize_);
  (void)data;
  self->setMaximized(true);
}

void OSFXWaylandView::handleRequestFullscreen(wl_listener *listener,
                                              void *data) {
  OSFXWaylandView *self =
      WL_CONTAINER_OF(listener, OSFXWaylandView, requestFullscreen_);
  (void)data;
  self->setFullscreen(true);
}

void OSFXWaylandView::handleRequestActivate(wl_listener *listener, void *data) {
  OSFXWaylandView *self =
      WL_CONTAINER_OF(listener, OSFXWaylandView, requestActivate_);
  (void)data;
  self->focus();
}

void OSFXWaylandView::handleSetTitle(wl_listener *listener, void *data) {
  OSFXWaylandView *self = WL_CONTAINER_OF(listener, OSFXWaylandView, setTitle_);
  (void)data;

  std::cout << "[openSEF] XWayland title: "
            << (self->surface_->title ? self->surface_->title : "(none)")
            << std::endl;
}

} // namespace opensef
