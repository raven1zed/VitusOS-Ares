/**
 * server.c - openSEF Compositor Server Implementation
 *
 * Core server initialization and event loop.
 */

#define _POSIX_C_SOURCE 200112L

#include "server.h"
#include "multitask.h"
#include "tiling.h"

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wayland-server-core.h>

#include <wlr/backend.h>
#include <wlr/render/allocator.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_layer_shell_v1.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_server_decoration.h>
#include <wlr/types/wlr_subcompositor.h>
#include <wlr/types/wlr_xcursor_manager.h>
#include <wlr/types/wlr_xdg_decoration_v1.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/util/log.h>

/* Forward declarations for handlers defined in other files */
extern void osf_new_output(struct wl_listener *listener, void *data);
extern void osf_new_xdg_toplevel(struct wl_listener *listener, void *data);
extern void osf_new_xdg_popup(struct wl_listener *listener, void *data);
extern void osf_new_layer_surface(struct wl_listener *listener, void *data);
extern void osf_new_input(struct wl_listener *listener, void *data);
extern void osf_cursor_motion(struct wl_listener *listener, void *data);
extern void osf_cursor_motion_absolute(struct wl_listener *listener,
                                       void *data);
extern void osf_cursor_button(struct wl_listener *listener, void *data);
extern void osf_cursor_axis(struct wl_listener *listener, void *data);
extern void osf_cursor_frame(struct wl_listener *listener, void *data);
extern void osf_seat_request_cursor(struct wl_listener *listener, void *data);
extern void osf_seat_request_set_selection(struct wl_listener *listener,
                                           void *data);
extern void osf_new_xdg_decoration(struct wl_listener *listener, void *data);

/* Forced frame timer callback for WSLg/Nested environments */
static int frame_timer_callback(void *data) {
  struct osf_server *server = data;
  struct osf_output *output;

  wl_list_for_each(output, &server->outputs, link) {
    wlr_output_schedule_frame(output->wlr_output);
  }

  /* Re-arm timer for ~60fps (16ms) */
  wl_event_source_timer_update(server->frame_timer, 16);
  return 0;
}

bool osf_server_init(struct osf_server *server, const char *socket_name) {
  wlr_log(WLR_INFO, "Initializing openSEF compositor...");

  /* Create Wayland display */
  server->wl_display = wl_display_create();
  if (!server->wl_display) {
    wlr_log(WLR_ERROR, "Failed to create Wayland display");
    return false;
  }
  server->wl_event_loop = wl_display_get_event_loop(server->wl_display);

  /* Create backend (auto-detects DRM, Wayland, X11, headless) */
  const char *backend_type = getenv("VITUS_BACKEND");

  if (backend_type && strcmp(backend_type, "wayland") == 0) {
    /* Nested mode: Create Wayland backend (connects to WSLg) */
    wlr_log(WLR_INFO, "Using Wayland backend (nested mode)");
    setenv("WLR_BACKENDS", "wayland", 1);
  } else if (backend_type && strcmp(backend_type, "x11") == 0) {
    /* X11 window mode */
    wlr_log(WLR_INFO, "Using X11 backend (nested mode)");
    setenv("WLR_BACKENDS", "x11", 1);
  }

  /* wlroots 0.18 signature (Confirmed by build error) */
  server->backend = wlr_backend_autocreate(server->wl_event_loop, NULL);
  if (!server->backend) {
    wlr_log(WLR_ERROR, "Failed to create wlroots backend");
    goto error_display;
  }

  /* Create renderer */
  if (backend_type) {
    wlr_log(WLR_INFO, "Nested mode detected, disabling hardware cursors");
    setenv("WLR_NO_HARDWARE_CURSORS", "1", 1);
  }
  server->renderer = wlr_renderer_autocreate(server->backend);
  if (!server->renderer) {
    wlr_log(WLR_ERROR, "Failed to create renderer");
    goto error_backend;
  }
  wlr_renderer_init_wl_display(server->renderer, server->wl_display);
  wlr_log(WLR_INFO, "  Renderer created successfully");

  /* Create allocator */
  server->allocator =
      wlr_allocator_autocreate(server->backend, server->renderer);
  if (!server->allocator) {
    wlr_log(WLR_ERROR, "Failed to create allocator");
    goto error_backend;
  }
  wlr_log(WLR_INFO, "  Allocator created successfully");

  /* Create compositor */
  server->compositor =
      wlr_compositor_create(server->wl_display, 5, server->renderer);
  wlr_subcompositor_create(server->wl_display);
  wlr_data_device_manager_create(server->wl_display);

  /* Create output layout */
  server->output_layout = wlr_output_layout_create(server->wl_display);

  /* Create scene graph */
  server->scene = wlr_scene_create();
  server->scene_layout =
      wlr_scene_attach_output_layout(server->scene, server->output_layout);

  /* Create desktop layers */
  server->layer_background = wlr_scene_tree_create(&server->scene->tree);
  server->layer_bottom = wlr_scene_tree_create(&server->scene->tree);
  server->layer_views = wlr_scene_tree_create(&server->scene->tree);
  server->layer_top = wlr_scene_tree_create(&server->scene->tree);
  server->layer_overlay = wlr_scene_tree_create(&server->scene->tree);

  /* Initialize lists */
  wl_list_init(&server->outputs);
  wl_list_init(&server->views);
  wl_list_init(&server->layer_surfaces);
  wl_list_init(&server->keyboards);

  /* Output handling */
  server->new_output.notify = osf_new_output;
  wl_signal_add(&server->backend->events.new_output, &server->new_output);

  /* XDG shell for windows */
  server->xdg_shell = wlr_xdg_shell_create(server->wl_display, 3);
  server->new_xdg_toplevel.notify = osf_new_xdg_toplevel;
  wl_signal_add(&server->xdg_shell->events.new_toplevel,
                &server->new_xdg_toplevel);
  server->new_xdg_popup.notify = osf_new_xdg_popup;
  wl_signal_add(&server->xdg_shell->events.new_popup, &server->new_xdg_popup);

  /* Layer shell for dock/panel */
  server->layer_shell = wlr_layer_shell_v1_create(server->wl_display, 4);
  server->new_layer_surface.notify = osf_new_layer_surface;
  wl_signal_add(&server->layer_shell->events.new_surface,
                &server->new_layer_surface);

  /* Cursor */
  server->cursor = wlr_cursor_create();
  wlr_cursor_attach_output_layout(server->cursor, server->output_layout);

  server->cursor_mgr = wlr_xcursor_manager_create(NULL, 24);

  server->cursor_motion.notify = osf_cursor_motion;
  wl_signal_add(&server->cursor->events.motion, &server->cursor_motion);
  server->cursor_motion_absolute.notify = osf_cursor_motion_absolute;
  wl_signal_add(&server->cursor->events.motion_absolute,
                &server->cursor_motion_absolute);
  server->cursor_button.notify = osf_cursor_button;
  wl_signal_add(&server->cursor->events.button, &server->cursor_button);
  server->cursor_axis.notify = osf_cursor_axis;
  wl_signal_add(&server->cursor->events.axis, &server->cursor_axis);
  server->cursor_frame.notify = osf_cursor_frame;
  wl_signal_add(&server->cursor->events.frame, &server->cursor_frame);

  /* Seat (input devices) */
  server->seat = wlr_seat_create(server->wl_display, "seat0");
  server->new_input.notify = osf_new_input;
  wl_signal_add(&server->backend->events.new_input, &server->new_input);
  server->request_cursor.notify = osf_seat_request_cursor;
  wl_signal_add(&server->seat->events.request_set_cursor,
                &server->request_cursor);
  server->request_set_selection.notify = osf_seat_request_set_selection;
  wl_signal_add(&server->seat->events.request_set_selection,
                &server->request_set_selection);

  /* Decorations - prefer server-side */
  server->xdg_decoration_mgr =
      wlr_xdg_decoration_manager_v1_create(server->wl_display);
  server->new_xdg_decoration.notify = osf_new_xdg_decoration;
  wl_signal_add(&server->xdg_decoration_mgr->events.new_toplevel_decoration,
                &server->new_xdg_decoration);

  server->server_decoration_mgr =
      wlr_server_decoration_manager_create(server->wl_display);
  wlr_server_decoration_manager_set_default_mode(
      server->server_decoration_mgr, WLR_SERVER_DECORATION_MANAGER_MODE_SERVER);

  /* Add socket - use explicit name if provided, otherwise auto */
  if (socket_name && socket_name[0] != '\0') {
    if (wl_display_add_socket(server->wl_display, socket_name) != 0) {
      wlr_log(WLR_ERROR, "Failed to create Wayland socket: %s", socket_name);
      goto error_backend;
    }
    server->socket = socket_name;
    wlr_log(WLR_INFO, "Using explicit socket name: %s", socket_name);
  } else {
    server->socket = wl_display_add_socket_auto(server->wl_display);
    if (!server->socket) {
      wlr_log(WLR_ERROR, "Failed to create Wayland socket (auto)");
      goto error_backend;
    }
    wlr_log(WLR_INFO, "Using auto socket name: %s", server->socket);
  }

  /* Start backend */
  if (!wlr_backend_start(server->backend)) {
    wlr_log(WLR_ERROR, "Failed to start wlroots backend");
    goto error_backend;
  }

  wlr_log(WLR_INFO, "Compositor initialized successfully");

  /* Initialize forced frame timer (WSLg workaround) */
  struct wl_event_loop *loop = wl_display_get_event_loop(server->wl_display);
  server->frame_timer =
      wl_event_loop_add_timer(loop, frame_timer_callback, server);
  wl_event_source_timer_update(server->frame_timer, 16);

  /* Multitask View */
  osf_multitask_init(server);

  /* Tiling Engine */
  osf_tiling_init(server);

  return true;

error_backend:
  wlr_backend_destroy(server->backend);
error_display:
  wl_display_destroy(server->wl_display);
  return false;
}

void osf_server_run(struct osf_server *server) {
  wlr_log(WLR_INFO, "Starting event loop...");
  wl_display_run(server->wl_display);
}

void osf_server_finish(struct osf_server *server) {
  wlr_log(WLR_INFO, "Shutting down compositor...");

  wl_display_destroy_clients(server->wl_display);

  wlr_scene_node_destroy(&server->scene->tree.node);
  wlr_xcursor_manager_destroy(server->cursor_mgr);
  wlr_cursor_destroy(server->cursor);
  wlr_allocator_destroy(server->allocator);
  wlr_renderer_destroy(server->renderer);
  wlr_backend_destroy(server->backend);
  wl_display_destroy(server->wl_display);
}

void osf_reset_cursor_mode(struct osf_server *server) {
  server->cursor_mode = OSF_CURSOR_PASSTHROUGH;
  server->grabbed_view = NULL;
}
