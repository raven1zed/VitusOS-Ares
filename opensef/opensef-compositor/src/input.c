/**
 * input.c - Input Handling (Keyboard, Pointer)
 *
 * Handles new input devices, keyboard events, and cursor motion.
 */

#include "server.h"

#include "OSFFrameworkC.h"
#include "multitask.h"
#include "tiling.h"
#include <stdlib.h>
#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_keyboard.h>
#include <wlr/types/wlr_pointer.h>
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_xcursor_manager.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/util/log.h>
#include <xkbcommon/xkbcommon.h>

/* ============================================================================
 * Keyboard
 * ============================================================================
 */

static void keyboard_modifiers(struct wl_listener *listener, void *data) {
  struct osf_keyboard *keyboard =
      wl_container_of(listener, keyboard, modifiers);

  (void)data;

  wlr_seat_set_keyboard(keyboard->server->seat, keyboard->wlr_keyboard);
  wlr_seat_keyboard_notify_modifiers(keyboard->server->seat,
                                     &keyboard->wlr_keyboard->modifiers);
}

static bool handle_compositor_keybinding(struct osf_server *server,
                                         xkb_keysym_t sym) {
  /* Compositor-level keybindings */
  switch (sym) {
  case XKB_KEY_Escape:
    /* Alt+Escape: Terminate compositor (for development) */
    if (wlr_keyboard_get_modifiers(wlr_seat_get_keyboard(server->seat)) &
        WLR_MODIFIER_ALT) {
      wl_display_terminate(server->wl_display);
      return true;
    }
    break;
  case XKB_KEY_F1:
    /* Alt+F1: Cycle focus */
    if (wlr_keyboard_get_modifiers(wlr_seat_get_keyboard(server->seat)) &
        WLR_MODIFIER_ALT) {
      if (!wl_list_empty(&server->views)) {
        struct osf_view *next = wl_container_of(server->views.prev, next, link);
        osf_focus_view(next, next->xdg_toplevel->base->surface);
      }
      return true;
    }
    break;

  case XKB_KEY_Tab:
    /* Super+Tab: Toggle Multitask View */
    if (wlr_keyboard_get_modifiers(wlr_seat_get_keyboard(server->seat)) &
        WLR_MODIFIER_LOGO) {
      osf_multitask_toggle(server);
      return true;
    }
    break;

  case XKB_KEY_t:
    /* Super+T: Toggle Tiling */
    if (wlr_keyboard_get_modifiers(wlr_seat_get_keyboard(server->seat)) &
        WLR_MODIFIER_LOGO) {
      // Toggle tiling on the primary output
      struct osf_output *output =
          wl_container_of(server->outputs.next, output, link);
      osf_tiling_arrange(server, output);
      return true;
    }
    break;
  }
  return false;
}

static void keyboard_key(struct wl_listener *listener, void *data) {
  struct osf_keyboard *keyboard = wl_container_of(listener, keyboard, key);
  struct wlr_keyboard_key_event *event = data;
  struct osf_server *server = keyboard->server;
  struct wlr_seat *seat = server->seat;

  /* Convert keycode to keysym */
  uint32_t keycode = event->keycode + 8;
  const xkb_keysym_t *syms;
  int num_syms =
      xkb_state_key_get_syms(keyboard->wlr_keyboard->xkb_state, keycode, &syms);

  bool handled = false;

  if (event->state == WL_KEYBOARD_KEY_STATE_PRESSED) {
    for (int i = 0; i < num_syms; i++) {
      handled = handle_compositor_keybinding(server, syms[i]);
      if (handled)
        break;
    }
  }

  if (!handled) {
    /* Pass to focused client */
    wlr_seat_set_keyboard(seat, keyboard->wlr_keyboard);
    wlr_seat_keyboard_notify_key(seat, event->time_msec, event->keycode,
                                 event->state);
  }
}

static void keyboard_destroy(struct wl_listener *listener, void *data) {
  struct osf_keyboard *keyboard = wl_container_of(listener, keyboard, destroy);

  (void)data;

  wl_list_remove(&keyboard->modifiers.link);
  wl_list_remove(&keyboard->key.link);
  wl_list_remove(&keyboard->destroy.link);
  wl_list_remove(&keyboard->link);
  free(keyboard);
}

static void new_keyboard(struct osf_server *server,
                         struct wlr_input_device *device) {
  struct wlr_keyboard *wlr_keyboard = wlr_keyboard_from_input_device(device);

  struct osf_keyboard *keyboard = calloc(1, sizeof(*keyboard));
  if (!keyboard) {
    wlr_log(WLR_ERROR, "Failed to allocate keyboard");
    return;
  }

  keyboard->server = server;
  keyboard->wlr_keyboard = wlr_keyboard;

  /* Set up keymap */
  struct xkb_context *context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
  struct xkb_keymap *keymap =
      xkb_keymap_new_from_names(context, NULL, XKB_KEYMAP_COMPILE_NO_FLAGS);

  wlr_keyboard_set_keymap(wlr_keyboard, keymap);
  xkb_keymap_unref(keymap);
  xkb_context_unref(context);

  wlr_keyboard_set_repeat_info(wlr_keyboard, 25, 600);

  /* Listeners */
  keyboard->modifiers.notify = keyboard_modifiers;
  wl_signal_add(&wlr_keyboard->events.modifiers, &keyboard->modifiers);

  keyboard->key.notify = keyboard_key;
  wl_signal_add(&wlr_keyboard->events.key, &keyboard->key);

  keyboard->destroy.notify = keyboard_destroy;
  wl_signal_add(&device->events.destroy, &keyboard->destroy);

  wlr_seat_set_keyboard(server->seat, wlr_keyboard);
  wl_list_insert(&server->keyboards, &keyboard->link);

  wlr_log(WLR_INFO, "Keyboard added");
}

static void new_pointer(struct osf_server *server,
                        struct wlr_input_device *device) {
  wlr_cursor_attach_input_device(server->cursor, device);
  wlr_log(WLR_INFO, "Pointer added");
}

void osf_new_input(struct wl_listener *listener, void *data) {
  struct osf_server *server = wl_container_of(listener, server, new_input);
  struct wlr_input_device *device = data;

  switch (device->type) {
  case WLR_INPUT_DEVICE_KEYBOARD:
    new_keyboard(server, device);
    break;
  case WLR_INPUT_DEVICE_POINTER:
    new_pointer(server, device);
    break;
  default:
    wlr_log(WLR_INFO, "Unhandled input device type: %d", device->type);
    break;
  }

  /* Update seat capabilities */
  uint32_t caps = WL_SEAT_CAPABILITY_POINTER;
  if (!wl_list_empty(&server->keyboards)) {
    caps |= WL_SEAT_CAPABILITY_KEYBOARD;
  }
  wlr_seat_set_capabilities(server->seat, caps);
}

/* ============================================================================
 * Cursor
 * ============================================================================
 */

static void process_cursor_move(struct osf_server *server, uint32_t time) {
  (void)time;

  struct osf_view *view = server->grabbed_view;
  double next_y = server->cursor->y - server->grab_y;

  /* Global Menu (Panel) constraint: Y must be >= 28 */
  if (next_y < 28) {
    next_y = 28;
  }

  wlr_scene_node_set_position(&view->scene_tree->node,
                              server->cursor->x - server->grab_x, next_y);

  /* Report to framework */
  char window_id[64];
  snprintf(window_id, sizeof(window_id), "window-%p", (void *)view);
  struct wlr_box geo = view->xdg_toplevel->base->current.geometry;
  osf_window_set_geometry(window_id, server->cursor->x - server->grab_x, next_y,
                          geo.width, geo.height);
}

static void process_cursor_resize(struct osf_server *server, uint32_t time) {
  (void)time;

  struct osf_view *view = server->grabbed_view;

  double border_x = server->cursor->x - server->grab_x;
  double border_y = server->cursor->y - server->grab_y;

  int new_left = server->grab_geobox.x;
  int new_right = server->grab_geobox.x + server->grab_geobox.width;
  int new_top = server->grab_geobox.y;
  int new_bottom = server->grab_geobox.y + server->grab_geobox.height;

  if (server->resize_edges & WLR_EDGE_TOP) {
    new_top = border_y;
    /* Global Menu constraint */
    if (new_top < 28) {
      new_top = 28;
    }
    if (new_top >= new_bottom) {
      new_top = new_bottom - 1;
    }
  } else if (server->resize_edges & WLR_EDGE_BOTTOM) {
    new_bottom = border_y;
    if (new_bottom <= new_top) {
      new_bottom = new_top + 1;
    }
  }

  if (server->resize_edges & WLR_EDGE_LEFT) {
    new_left = border_x;
    if (new_left >= new_right) {
      new_left = new_right - 1;
    }
  } else if (server->resize_edges & WLR_EDGE_RIGHT) {
    new_right = border_x;
    if (new_right <= new_left) {
      new_right = new_left + 1;
    }
  }

  struct wlr_box geo;
  // wlr_xdg_surface_get_geometry(view->xdg_toplevel->base, &geo);
  geo = view->xdg_toplevel->base->current.geometry;
  wlr_scene_node_set_position(&view->scene_tree->node, new_left - geo.x,
                              new_top - geo.y);

  int new_width = new_right - new_left;
  int new_height = new_bottom - new_top;
  wlr_xdg_toplevel_set_size(view->xdg_toplevel, new_width, new_height);

  /* Report to framework */
  char window_id[64];
  snprintf(window_id, sizeof(window_id), "window-%p", (void *)view);
  osf_window_set_geometry(window_id, new_left, new_top, new_width, new_height);
}

static void process_cursor_motion(struct osf_server *server, uint32_t time) {
  if (server->cursor_mode == OSF_CURSOR_MOVE) {
    process_cursor_move(server, time);
    return;
  } else if (server->cursor_mode == OSF_CURSOR_RESIZE) {
    process_cursor_resize(server, time);
    return;
  }

  /* Find view under cursor */
  double sx, sy;
  struct wlr_surface *surface = NULL;
  struct osf_view *view = osf_view_at(server, server->cursor->x,
                                      server->cursor->y, &surface, &sx, &sy);

  if (!view) {
    wlr_cursor_set_xcursor(server->cursor, server->cursor_mgr, "default");
  }

  if (surface) {
    wlr_seat_pointer_notify_enter(server->seat, surface, sx, sy);
    wlr_seat_pointer_notify_motion(server->seat, time, sx, sy);
  } else {
    wlr_seat_pointer_clear_focus(server->seat);
  }
}

void osf_cursor_motion(struct wl_listener *listener, void *data) {
  struct osf_server *server = wl_container_of(listener, server, cursor_motion);
  struct wlr_pointer_motion_event *event = data;

  wlr_cursor_move(server->cursor, &event->pointer->base, event->delta_x,
                  event->delta_y);
  process_cursor_motion(server, event->time_msec);
}

void osf_cursor_motion_absolute(struct wl_listener *listener, void *data) {
  struct osf_server *server =
      wl_container_of(listener, server, cursor_motion_absolute);
  struct wlr_pointer_motion_absolute_event *event = data;

  wlr_cursor_warp_absolute(server->cursor, &event->pointer->base, event->x,
                           event->y);
  process_cursor_motion(server, event->time_msec);
}

void osf_cursor_button(struct wl_listener *listener, void *data) {
  struct osf_server *server = wl_container_of(listener, server, cursor_button);
  struct wlr_pointer_button_event *event = data;

  wlr_seat_pointer_notify_button(server->seat, event->time_msec, event->button,
                                 event->state);

  if (event->state == WL_POINTER_BUTTON_STATE_RELEASED) {
    osf_reset_cursor_mode(server);
  } else {
    /* Focus clicked view */
    double sx, sy;
    struct wlr_surface *surface = NULL;
    struct osf_view *view = osf_view_at(server, server->cursor->x,
                                        server->cursor->y, &surface, &sx, &sy);
    if (view) {
      osf_focus_view(view, surface);
    }
  }
}

void osf_cursor_axis(struct wl_listener *listener, void *data) {
  struct osf_server *server = wl_container_of(listener, server, cursor_axis);
  struct wlr_pointer_axis_event *event = data;

  wlr_seat_pointer_notify_axis(
      server->seat, event->time_msec, event->orientation, event->delta,
      event->delta_discrete, event->source, event->relative_direction);
}

void osf_cursor_frame(struct wl_listener *listener, void *data) {
  struct osf_server *server = wl_container_of(listener, server, cursor_frame);
  (void)data;
  wlr_seat_pointer_notify_frame(server->seat);
}

void osf_seat_request_cursor(struct wl_listener *listener, void *data) {
  struct osf_server *server = wl_container_of(listener, server, request_cursor);
  struct wlr_seat_pointer_request_set_cursor_event *event = data;

  struct wlr_seat_client *focused = server->seat->pointer_state.focused_client;
  if (focused == event->seat_client) {
    wlr_cursor_set_surface(server->cursor, event->surface, event->hotspot_x,
                           event->hotspot_y);
  }
}

void osf_seat_request_set_selection(struct wl_listener *listener, void *data) {
  struct osf_server *server =
      wl_container_of(listener, server, request_set_selection);
  struct wlr_seat_request_set_selection_event *event = data;
  wlr_seat_set_selection(server->seat, event->source, event->serial);
}
