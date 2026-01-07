/**
 * server.h - openSEF Compositor Server State
 *
 * Core server structure and function declarations.
 * Pure C implementation for wlroots compatibility.
 */

#ifndef OSF_SERVER_H
#define OSF_SERVER_H

#include <wayland-server-core.h>
#include <wlr/backend.h>
#include <wlr/render/allocator.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_layer_shell_v1.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_server_decoration.h>
#include <wlr/types/wlr_xcursor_manager.h>
#include <wlr/types/wlr_xdg_decoration_v1.h>
#include <wlr/types/wlr_xdg_shell.h>

#include <stdbool.h>

/* ============================================================================
 * Forward Declarations
 * ============================================================================
 */

struct osf_server;
struct osf_output;
struct osf_view;
struct osf_layer_surface;

/* ============================================================================
 * Server State
 * ============================================================================
 */

enum osf_cursor_mode {
  OSF_CURSOR_PASSTHROUGH,
  OSF_CURSOR_MOVE,
  OSF_CURSOR_RESIZE,
};

struct osf_server {
  struct wl_display *wl_display;
  struct wl_event_loop *wl_event_loop;

  /* wlroots components */
  struct wlr_backend *backend;
  struct wlr_renderer *renderer;
  struct wlr_allocator *allocator;
  struct wlr_compositor *compositor;

  /* Scene graph */
  struct wlr_scene *scene;
  struct wlr_scene_output_layout *scene_layout;

  /* Desktop layers (bottom to top) */
  struct wlr_scene_tree *layer_background;
  struct wlr_scene_tree *layer_bottom;
  struct wlr_scene_tree *layer_views;   /* Normal windows */
  struct wlr_scene_tree *layer_top;     /* Dock, panel */
  struct wlr_scene_tree *layer_overlay; /* Notifications, OSD */

  /* Output management */
  struct wlr_output_layout *output_layout;
  struct wl_list outputs; /* osf_output::link */
  struct wl_listener new_output;

  /* XDG shell (windows) */
  struct wlr_xdg_shell *xdg_shell;
  struct wl_list views; /* osf_view::link */
  struct wl_listener new_xdg_toplevel;
  struct wl_listener new_xdg_popup;

  /* Layer shell (dock, panel) */
  struct wlr_layer_shell_v1 *layer_shell;
  struct wl_list layer_surfaces; /* osf_layer_surface::link */
  struct wl_listener new_layer_surface;

  /* Input */
  struct wlr_seat *seat;
  struct wl_listener new_input;
  struct wl_listener request_cursor;
  struct wl_listener request_set_selection;
  struct wl_list keyboards;

  /* Cursor */
  struct wlr_cursor *cursor;
  struct wlr_xcursor_manager *cursor_mgr;
  struct wl_listener cursor_motion;
  struct wl_listener cursor_motion_absolute;
  struct wl_listener cursor_button;
  struct wl_listener cursor_axis;
  struct wl_listener cursor_frame;

  /* Cursor mode (move/resize) */
  enum osf_cursor_mode cursor_mode;
  struct osf_view *grabbed_view;
  double grab_x, grab_y;
  struct wlr_box grab_geobox;
  uint32_t resize_edges;

  /* Decorations */
  struct wlr_xdg_decoration_manager_v1 *xdg_decoration_mgr;
  struct wlr_server_decoration_manager *server_decoration_mgr;
  struct wl_listener new_xdg_decoration;

  /* Socket name for clients */
  const char *socket;

  /* Forced frame timer for hosts that don't send events (WSLg) */
  struct wl_event_source *frame_timer;
};

/* ============================================================================
 * Output (Monitor)
 * ============================================================================
 */

struct osf_output {
  struct wl_list link;
  struct osf_server *server;
  struct wlr_output *wlr_output;
  struct wlr_scene_output *scene_output;

  struct wl_listener frame;
  struct wl_listener request_state;
  struct wl_listener destroy;
};

/* ============================================================================
 * View (Window)
 * ============================================================================
 */

struct osf_view {
  struct wl_list link;
  struct osf_server *server;
  struct wlr_xdg_toplevel *xdg_toplevel;
  struct wlr_scene_tree *scene_tree;

  /* Listeners */
  struct wl_listener map;
  struct wl_listener unmap;
  struct wl_listener commit;
  struct wl_listener destroy;
  struct wl_listener request_move;
  struct wl_listener request_resize;
  struct wl_listener request_maximize;
  struct wl_listener request_fullscreen;
  struct wl_listener request_minimize;

  /* State */
  bool mapped;

  /* Framework integration */
  void *framework_window; /* OSFWindowC* from framework */
};

/* ============================================================================
 * Layer Surface (Dock, Panel)
 * ============================================================================
 */

struct osf_layer_surface {
  struct wl_list link;
  struct osf_server *server;
  struct wlr_layer_surface_v1 *layer_surface;
  struct wlr_scene_layer_surface_v1 *scene_layer_surface;

  struct wl_listener map;
  struct wl_listener unmap;
  struct wl_listener commit;
  struct wl_listener destroy;

  bool mapped;
};

/* ============================================================================
 * Keyboard
 * ============================================================================
 */

struct osf_keyboard {
  struct wl_list link;
  struct osf_server *server;
  struct wlr_keyboard *wlr_keyboard;

  struct wl_listener modifiers;
  struct wl_listener key;
  struct wl_listener destroy;
};

/* ============================================================================
 * Function Declarations
 * ============================================================================
 */

/* Server lifecycle */
bool osf_server_init(struct osf_server *server, const char *socket_name);
void osf_server_run(struct osf_server *server);
void osf_server_finish(struct osf_server *server);

/* View management */
struct osf_view *osf_view_at(struct osf_server *server, double lx, double ly,
                             struct wlr_surface **surface, double *sx,
                             double *sy);
void osf_focus_view(struct osf_view *view, struct wlr_surface *surface);

/* Cursor */
void osf_reset_cursor_mode(struct osf_server *server);

#endif /* OSF_SERVER_H */
