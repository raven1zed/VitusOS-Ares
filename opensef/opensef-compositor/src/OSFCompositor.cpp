/**
 * OSFCompositor.cpp - openSEF Wayland Compositor Implementation
 *
 * Based on tinywl from wlroots, adapted for C++ and openSEF.
 */

#include "OSFCompositor.h"
#include "OSFCursor.h"
#include "OSFKeyboard.h"
#include "OSFOutput.h"
#include "OSFView.h"


#include <cassert>
#include <iostream>


extern "C" {
#include <wlr/types/wlr_data_control_v1.h>
#include <wlr/types/wlr_output_layout.h>

}

namespace opensef {

// Helper macro to get class instance from wl_listener
#define WL_CONTAINER_OF(ptr, type, member)                                     \
  reinterpret_cast<type *>(reinterpret_cast<char *>(ptr) -                     \
                           offsetof(type, member))

OSFCompositor &OSFCompositor::shared() {
  static OSFCompositor instance;
  return instance;
}

OSFCompositor::~OSFCompositor() { shutdown(); }

bool OSFCompositor::initialize() {
  // Create Wayland display
  display_ = wl_display_create();
  if (!display_) {
    std::cerr << "[openSEF] Failed to create Wayland display" << std::endl;
    return false;
  }

  // Create wlroots backend (auto-detects DRM, Wayland, X11, or headless)
  backend_ =
      wlr_backend_autocreate(wl_display_get_event_loop(display_), nullptr);
  if (!backend_) {
    std::cerr << "[openSEF] Failed to create wlroots backend" << std::endl;
    wl_display_destroy(display_);
    return false;
  }

  // Create renderer
  renderer_ = wlr_renderer_autocreate(backend_);
  if (!renderer_) {
    std::cerr << "[openSEF] Failed to create renderer" << std::endl;
    wlr_backend_destroy(backend_);
    wl_display_destroy(display_);
    return false;
  }
  wlr_renderer_init_wl_display(renderer_, display_);

  // Create allocator
  allocator_ = wlr_allocator_autocreate(backend_, renderer_);
  if (!allocator_) {
    std::cerr << "[openSEF] Failed to create allocator" << std::endl;
    return false;
  }

  // Create compositor and subcompositor
  compositor_ = wlr_compositor_create(display_, 5, renderer_);
  subcompositor_ = wlr_subcompositor_create(display_);

  // Create data device manager (for clipboard)
  dataDeviceMgr_ = wlr_data_device_manager_create(display_);

  // Create output layout (for multi-monitor)
  outputLayout_ = wlr_output_layout_create(display_);

  // Create scene graph
  scene_ = wlr_scene_create();
  sceneLayout_ = wlr_scene_attach_output_layout(scene_, outputLayout_);

  // Set up XDG shell for window management
  xdgShell_ = wlr_xdg_shell_create(display_, 3);
  newXdgSurface_.notify = handleNewXdgSurface;
  wl_signal_add(&xdgShell_->events.new_toplevel, &newXdgSurface_);

  // Set up output handling
  newOutput_.notify = handleNewOutput;
  wl_signal_add(&backend_->events.new_output, &newOutput_);

  // Create cursor
  cursor_ = wlr_cursor_create();
  wlr_cursor_attach_output_layout(cursor_, outputLayout_);

  // Create cursor manager (for cursor themes)
  cursorMgr_ = wlr_xcursor_manager_create(nullptr, 24);

  // Set up cursor events
  cursorMotion_.notify = handleCursorMotion;
  wl_signal_add(&cursor_->events.motion, &cursorMotion_);

  cursorMotionAbsolute_.notify = handleCursorMotionAbsolute;
  wl_signal_add(&cursor_->events.motion_absolute, &cursorMotionAbsolute_);

  cursorButton_.notify = handleCursorButton;
  wl_signal_add(&cursor_->events.button, &cursorButton_);

  cursorAxis_.notify = handleCursorAxis;
  wl_signal_add(&cursor_->events.axis, &cursorAxis_);

  cursorFrame_.notify = handleCursorFrame;
  wl_signal_add(&cursor_->events.frame, &cursorFrame_);

  // Create seat (input handling)
  seat_ = wlr_seat_create(display_, "seat0");

  requestSetCursor_.notify = handleRequestSetCursor;
  wl_signal_add(&seat_->events.request_set_cursor, &requestSetCursor_);

  requestSetSelection_.notify = handleRequestSetSelection;
  wl_signal_add(&seat_->events.request_set_selection, &requestSetSelection_);

  // Set up input device handling
  newInput_.notify = handleNewInput;
  wl_signal_add(&backend_->events.new_input, &newInput_);

  // Add socket for Wayland clients
  const char *socket = wl_display_add_socket_auto(display_);
  if (!socket) {
    std::cerr << "[openSEF] Failed to create Wayland socket" << std::endl;
    return false;
  }

  std::cout << "[openSEF] Wayland socket: " << socket << std::endl;

  // Start the backend
  if (!wlr_backend_start(backend_)) {
    std::cerr << "[openSEF] Failed to start backend" << std::endl;
    return false;
  }

  // Set WAYLAND_DISPLAY for child processes
  setenv("WAYLAND_DISPLAY", socket, true);

  return true;
}

void OSFCompositor::run() { wl_display_run(display_); }

void OSFCompositor::shutdown() {
  if (display_) {
    wl_display_destroy_clients(display_);
    wl_display_destroy(display_);
    display_ = nullptr;
  }
}

void OSFCompositor::focusView(OSFView *view) {
  if (!view)
    return;

  wlr_surface *surface = view->toplevel()->base->surface;
  wlr_seat_keyboard_notify_enter(seat_, surface, nullptr, 0, nullptr);
}

OSFView *OSFCompositor::viewAt(double x, double y, wlr_surface **surface,
                               double *sx, double *sy) {
  wlr_scene_node *node = wlr_scene_node_at(&scene_->tree.node, x, y, sx, sy);
  if (!node || node->type != WLR_SCENE_NODE_BUFFER) {
    return nullptr;
  }

  wlr_scene_buffer *sceneBuffer = wlr_scene_buffer_from_node(node);
  wlr_scene_surface *sceneSurface =
      wlr_scene_surface_try_from_buffer(sceneBuffer);
  if (!sceneSurface) {
    return nullptr;
  }

  *surface = sceneSurface->surface;

  // Find the view for this surface
  wlr_scene_tree *tree = node->parent;
  while (tree && !tree->node.data) {
    tree = tree->node.parent;
  }

  return tree ? static_cast<OSFView *>(tree->node.data) : nullptr;
}

// Static event handlers

void OSFCompositor::handleNewOutput(wl_listener *listener, void *data) {
  OSFCompositor *self = WL_CONTAINER_OF(listener, OSFCompositor, newOutput_);
  wlr_output *output = static_cast<wlr_output *>(data);

  std::cout << "[openSEF] New output: " << output->name << std::endl;

  // Create OSFOutput wrapper
  auto osfOutput = std::make_unique<OSFOutput>(self, output);
  self->outputs_.push_back(std::move(osfOutput));
}

void OSFCompositor::handleNewXdgSurface(wl_listener *listener, void *data) {
  OSFCompositor *self =
      WL_CONTAINER_OF(listener, OSFCompositor, newXdgSurface_);
  wlr_xdg_toplevel *toplevel = static_cast<wlr_xdg_toplevel *>(data);

  std::cout << "[openSEF] New XDG toplevel" << std::endl;

  // Create OSFView wrapper
  auto view = std::make_unique<OSFView>(self, toplevel);
  self->views_.push_back(std::move(view));
}

void OSFCompositor::handleCursorMotion(wl_listener *listener, void *data) {
  OSFCompositor *self = WL_CONTAINER_OF(listener, OSFCompositor, cursorMotion_);
  wlr_pointer_motion_event *event =
      static_cast<wlr_pointer_motion_event *>(data);

  wlr_cursor_move(self->cursor_, &event->pointer->base, event->delta_x,
                  event->delta_y);
  self->processCursorMotion(event->time_msec);
}

void OSFCompositor::handleCursorMotionAbsolute(wl_listener *listener,
                                               void *data) {
  OSFCompositor *self =
      WL_CONTAINER_OF(listener, OSFCompositor, cursorMotionAbsolute_);
  wlr_pointer_motion_absolute_event *event =
      static_cast<wlr_pointer_motion_absolute_event *>(data);

  wlr_cursor_warp_absolute(self->cursor_, &event->pointer->base, event->x,
                           event->y);
  self->processCursorMotion(event->time_msec);
}

void OSFCompositor::handleCursorButton(wl_listener *listener, void *data) {
  OSFCompositor *self = WL_CONTAINER_OF(listener, OSFCompositor, cursorButton_);
  wlr_pointer_button_event *event =
      static_cast<wlr_pointer_button_event *>(data);

  wlr_seat_pointer_notify_button(self->seat_, event->time_msec, event->button,
                                 event->state);

  if (event->state == WL_POINTER_BUTTON_STATE_RELEASED) {
    self->cursorMode_ = static_cast<int>(OSFCursorMode::PASSTHROUGH);
  } else {
    double sx, sy;
    wlr_surface *surface = nullptr;
    OSFView *view =
        self->viewAt(self->cursor_->x, self->cursor_->y, &surface, &sx, &sy);
    if (view) {
      self->focusView(view);
    }
  }
}

void OSFCompositor::handleCursorAxis(wl_listener *listener, void *data) {
  OSFCompositor *self = WL_CONTAINER_OF(listener, OSFCompositor, cursorAxis_);
  wlr_pointer_axis_event *event = static_cast<wlr_pointer_axis_event *>(data);

  wlr_seat_pointer_notify_axis(
      self->seat_, event->time_msec, event->orientation, event->delta,
      event->delta_discrete, event->source, event->relative_direction);
}

void OSFCompositor::handleCursorFrame(wl_listener *listener, void *data) {
  OSFCompositor *self = WL_CONTAINER_OF(listener, OSFCompositor, cursorFrame_);
  (void)data;
  wlr_seat_pointer_notify_frame(self->seat_);
}

void OSFCompositor::handleNewInput(wl_listener *listener, void *data) {
  OSFCompositor *self = WL_CONTAINER_OF(listener, OSFCompositor, newInput_);
  wlr_input_device *device = static_cast<wlr_input_device *>(data);

  switch (device->type) {
  case WLR_INPUT_DEVICE_KEYBOARD: {
    std::cout << "[openSEF] New keyboard" << std::endl;
    wlr_keyboard *keyboard = wlr_keyboard_from_input_device(device);
    auto kb = std::make_unique<OSFKeyboard>(self, keyboard);
    self->keyboards_.push_back(std::move(kb));
    break;
  }
  case WLR_INPUT_DEVICE_POINTER:
    std::cout << "[openSEF] New pointer" << std::endl;
    wlr_cursor_attach_input_device(self->cursor_, device);
    break;
  default:
    break;
  }

  uint32_t caps = WL_SEAT_CAPABILITY_POINTER;
  if (!self->keyboards_.empty()) {
    caps |= WL_SEAT_CAPABILITY_KEYBOARD;
  }
  wlr_seat_set_capabilities(self->seat_, caps);
}

void OSFCompositor::handleRequestSetCursor(wl_listener *listener, void *data) {
  OSFCompositor *self =
      WL_CONTAINER_OF(listener, OSFCompositor, requestSetCursor_);
  wlr_seat_pointer_request_set_cursor_event *event =
      static_cast<wlr_seat_pointer_request_set_cursor_event *>(data);

  wlr_cursor_set_surface(self->cursor_, event->surface, event->hotspot_x,
                         event->hotspot_y);
}

void OSFCompositor::handleRequestSetSelection(wl_listener *listener,
                                              void *data) {
  OSFCompositor *self =
      WL_CONTAINER_OF(listener, OSFCompositor, requestSetSelection_);
  wlr_seat_request_set_selection_event *event =
      static_cast<wlr_seat_request_set_selection_event *>(data);
  wlr_seat_set_selection(self->seat_, event->source, event->serial);
}

void OSFCompositor::processCursorMotion(uint32_t time) {
  // Update cursor image
  if (cursorMode_ == static_cast<int>(OSFCursorMode::PASSTHROUGH)) {
    double sx, sy;
    wlr_surface *surface = nullptr;
    OSFView *view = viewAt(cursor_->x, cursor_->y, &surface, &sx, &sy);

    if (!view) {
      wlr_cursor_set_xcursor(cursor_, cursorMgr_, "default");
    }

    if (surface) {
      wlr_seat_pointer_notify_enter(seat_, surface, sx, sy);
      wlr_seat_pointer_notify_motion(seat_, time, sx, sy);
    } else {
      wlr_seat_pointer_clear_focus(seat_);
    }
  }
}

} // namespace opensef
