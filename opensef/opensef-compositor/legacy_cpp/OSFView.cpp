/**
 * OSFView.cpp - Window/View implementation
 */

#include "OSFView.h"
#include "OSFCompositor.h"

#include <iostream>

namespace opensef {

#define WL_CONTAINER_OF(ptr, type, member)                                     \
  reinterpret_cast<type *>(reinterpret_cast<char *>(ptr) -                     \
                           offsetof(type, member))

OSFView::OSFView(OSFCompositor *compositor, wlr_xdg_toplevel *toplevel)
    : compositor_(compositor), toplevel_(toplevel) {

  // Create scene tree for this view
  sceneTree_ =
      wlr_scene_xdg_surface_create(&compositor->scene()->tree, toplevel->base);
  sceneTree_->node.data = this;

  // Set up event listeners
  map_.notify = handleMap;
  wl_signal_add(&toplevel->base->surface->events.map, &map_);

  unmap_.notify = handleUnmap;
  wl_signal_add(&toplevel->base->surface->events.unmap, &unmap_);

  destroy_.notify = handleDestroy;
  wl_signal_add(&toplevel->events.destroy, &destroy_);

  requestMove_.notify = handleRequestMove;
  wl_signal_add(&toplevel->events.request_move, &requestMove_);

  requestResize_.notify = handleRequestResize;
  wl_signal_add(&toplevel->events.request_resize, &requestResize_);

  requestMaximize_.notify = handleRequestMaximize;
  wl_signal_add(&toplevel->events.request_maximize, &requestMaximize_);

  requestMinimize_.notify = handleRequestMinimize;
  wl_signal_add(&toplevel->events.request_minimize, &requestMinimize_);

  requestFullscreen_.notify = handleRequestFullscreen;
  wl_signal_add(&toplevel->events.request_fullscreen, &requestFullscreen_);
}

OSFView::~OSFView() {
  wl_list_remove(&map_.link);
  wl_list_remove(&unmap_.link);
  wl_list_remove(&destroy_.link);
  wl_list_remove(&requestMove_.link);
  wl_list_remove(&requestResize_.link);
  wl_list_remove(&requestMaximize_.link);
  wl_list_remove(&requestMinimize_.link);
  wl_list_remove(&requestFullscreen_.link);
}

void OSFView::setPosition(int x, int y) {
  x_ = x;
  y_ = y;
  wlr_scene_node_set_position(&sceneTree_->node, x, y);
}

void OSFView::focus() {
  wlr_surface *surface = toplevel_->base->surface;
  wlr_seat *seat = compositor_->seat();
  wlr_keyboard *keyboard = wlr_seat_get_keyboard(seat);

  // Raise to top
  wlr_scene_node_raise_to_top(&sceneTree_->node);

  // Focus keyboard
  if (keyboard) {
    wlr_seat_keyboard_notify_enter(seat, surface, keyboard->keycodes,
                                   keyboard->num_keycodes,
                                   &keyboard->modifiers);
  }
}

void OSFView::setMaximized(bool maximized) {
  maximized_ = maximized;
  wlr_xdg_toplevel_set_maximized(toplevel_, maximized);
  std::cout << "[openSEF] Window " << (maximized ? "maximized" : "unmaximized")
            << std::endl;
}

void OSFView::setMinimized(bool minimized) {
  minimized_ = minimized;
  // Hide the scene node when minimized
  wlr_scene_node_set_enabled(&sceneTree_->node, !minimized);
  std::cout << "[openSEF] Window " << (minimized ? "minimized" : "restored")
            << std::endl;
}

void OSFView::setFullscreen(bool fullscreen) {
  fullscreen_ = fullscreen;
  wlr_xdg_toplevel_set_fullscreen(toplevel_, fullscreen);
  std::cout << "[openSEF] Window " << (fullscreen ? "fullscreen" : "windowed")
            << std::endl;
}

// Static handlers

void OSFView::handleMap(wl_listener *listener, void *data) {
  OSFView *self = WL_CONTAINER_OF(listener, OSFView, map_);
  (void)data;

  std::cout << "[openSEF] Window mapped" << std::endl;
  self->focus();
}

void OSFView::handleUnmap(wl_listener *listener, void *data) {
  (void)listener;
  (void)data;
  std::cout << "[openSEF] Window unmapped" << std::endl;
}

void OSFView::handleDestroy(wl_listener *listener, void *data) {
  (void)listener;
  (void)data;
  std::cout << "[openSEF] Window destroyed" << std::endl;
}

void OSFView::handleRequestMove(wl_listener *listener, void *data) {
  OSFView *self = WL_CONTAINER_OF(listener, OSFView, requestMove_);
  (void)data;
  std::cout << "[openSEF] Window move requested" << std::endl;
  // TODO: Implement window dragging
  (void)self;
}

void OSFView::handleRequestResize(wl_listener *listener, void *data) {
  OSFView *self = WL_CONTAINER_OF(listener, OSFView, requestResize_);
  (void)data;
  std::cout << "[openSEF] Window resize requested" << std::endl;
  // TODO: Implement window resizing
  (void)self;
}

void OSFView::handleRequestMaximize(wl_listener *listener, void *data) {
  OSFView *self = WL_CONTAINER_OF(listener, OSFView, requestMaximize_);
  (void)data;
  self->setMaximized(!self->maximized_);
}

void OSFView::handleRequestMinimize(wl_listener *listener, void *data) {
  OSFView *self = WL_CONTAINER_OF(listener, OSFView, requestMinimize_);
  (void)data;
  self->setMinimized(!self->minimized_);
}

void OSFView::handleRequestFullscreen(wl_listener *listener, void *data) {
  OSFView *self = WL_CONTAINER_OF(listener, OSFView, requestFullscreen_);
  (void)data;
  self->setFullscreen(!self->fullscreen_);
}

} // namespace opensef
