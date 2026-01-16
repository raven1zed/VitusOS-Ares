/**
 * view.c - View (Window) Management
 *
 * Handles XDG toplevel windows: creation, focus, move, resize.
 */

#include "server.h"

#include "tiling.h"
// #include "titlebar.h" // Server-side decorations disabled
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/util/log.h>

#include <opensef/OSFFrameworkC.h> /* Unified framework integration */

/* ============================================================================
 * View at position (for click handling)
 * ============================================================================
 */

struct osf_view *osf_view_at(struct osf_server *server, double lx, double ly,
                             struct wlr_surface **surface, double *sx,
                             double *sy) {
  /* Find the topmost node at this position */
  struct wlr_scene_node *node =
      wlr_scene_node_at(&server->scene->tree.node, lx, ly, sx, sy);

  if (!node || node->type != WLR_SCENE_NODE_BUFFER) {
    return NULL;
  }

  struct wlr_scene_buffer *scene_buffer = wlr_scene_buffer_from_node(node);
  struct wlr_scene_surface *scene_surface =
      wlr_scene_surface_try_from_buffer(scene_buffer);

  if (!scene_surface) {
    return NULL;
  }

  *surface = scene_surface->surface;

  /* Walk up tree to find the view's scene_tree */
  struct wlr_scene_tree *tree = node->parent;
  while (tree && !tree->node.data) {
    tree = tree->node.parent;
  }

  return tree ? tree->node.data : NULL;
}

/* ============================================================================
 * Focus handling
 * ============================================================================
 */

void osf_focus_view(struct osf_view *view, struct wlr_surface *surface) {
  if (!view) {
    return;
  }

  struct osf_server *server = view->server;
  struct wlr_seat *seat = server->seat;
  struct wlr_surface *prev_surface = seat->keyboard_state.focused_surface;

  if (prev_surface == surface) {
    return; /* Already focused */
  }

  if (prev_surface) {
    /* Deactivate previous */
    struct wlr_xdg_toplevel *prev_toplevel =
        wlr_xdg_toplevel_try_from_wlr_surface(prev_surface);
    if (prev_toplevel) {
      wlr_xdg_toplevel_set_activated(prev_toplevel, false);
      struct osf_view *prev_view = prev_toplevel->base->data;
      if (prev_view) {
        osf_view_update_borders(prev_view, false);
      }
    }
  }

  /* Move view to front */
  wlr_scene_node_raise_to_top(&view->scene_tree->node);
  wl_list_remove(&view->link);
  wl_list_insert(&server->views, &view->link);

  /* Activate view */
  wlr_xdg_toplevel_set_activated(view->xdg_toplevel, true);
  osf_view_update_borders(view, true);

  /* Notify openSEF framework of window focus */
  char window_id[64];
  snprintf(window_id, sizeof(window_id), "window-%p", (void *)view);
  osf_window_focus(window_id);

  wlr_log(WLR_DEBUG, "Framework notified: window focused - %s", window_id);

  /* Send keyboard focus */
  struct wlr_keyboard *keyboard = wlr_seat_get_keyboard(seat);
  if (keyboard) {
    wlr_seat_keyboard_notify_enter(seat, view->xdg_toplevel->base->surface,
                                   keyboard->keycodes, keyboard->num_keycodes,
                                   &keyboard->modifiers);
  }
}

/* ============================================================================
 * View event handlers
 * ============================================================================
 */
static void view_map(struct wl_listener *listener, void *data) {
  struct osf_view *view = wl_container_of(listener, view, map);

  (void)data;

  view->mapped = true;
  wl_list_insert(&view->server->views, &view->link);

  const char *app_id =
      view->xdg_toplevel->app_id ? view->xdg_toplevel->app_id : "unknown";

  int x = 50, y = 50;
  if (app_id && strcmp(app_id, "vitusos.shell") == 0) {
    x = 0;
    y = 0;
    wlr_log(WLR_INFO, "Shell window detected - positioning at (0,0)");
  }

  wlr_scene_node_set_position(&view->scene_tree->node, x, y);

  if (view->xdg_toplevel->base->surface) {
    osf_focus_view(view, view->xdg_toplevel->base->surface);
  }

  const char *title =
      view->xdg_toplevel->title ? view->xdg_toplevel->title : "(untitled)";

  wlr_log(WLR_INFO, "View mapped: %s (app_id: %s) at (%d, %d)", title, app_id,
          x, y);

  /* Register window with openSEF Framework - unregister old if exists */
  if (view->framework_window) {
    osf_window_destroy(view->framework_window);
    view->framework_window = NULL;
  }

  char window_id[64];
  snprintf(window_id, sizeof(window_id), "window-%p", (void *)view);

  OSFWindowC *window = osf_window_create(window_id, title, app_id);
  osf_window_register(window);

  /* Store window pointer in view for later cleanup */
  view->framework_window = window;

  /* Report initial geometry */
  struct wlr_box geo = view->xdg_toplevel->base->current.geometry;
  osf_window_set_geometry(window_id, x, y, geo.width, geo.height);

  wlr_log(WLR_INFO, "Window registered with framework: %s", window_id);
}

static void view_unmap(struct wl_listener *listener, void *data) {
  struct osf_view *view = wl_container_of(listener, view, unmap);

  (void)data;

  const char *app_id =
      view->xdg_toplevel->app_id ? view->xdg_toplevel->app_id : "unknown";
  wlr_log(WLR_INFO, "View unmapped: app_id='%s'", app_id);

  view->mapped = false;
  wl_list_remove(&view->link);

  /* Reset cursor mode if this was the grabbed view */
  if (view == view->server->grabbed_view) {
    osf_reset_cursor_mode(view->server);
  }

  wlr_log(WLR_INFO, "View unmapped");
}

static void view_commit(struct wl_listener *listener, void *data) {
  struct osf_view *view = wl_container_of(listener, view, commit);
  (void)data;

  if (view->xdg_toplevel->base->initial_commit) {
    wlr_xdg_toplevel_set_size(view->xdg_toplevel, 0, 0);
  }

  /* Report geometry changes if mapped to support intelligent shell features
   * like autohide */
  if (view->mapped && view->framework_window) {
    struct wlr_box geo = view->xdg_toplevel->base->current.geometry;
    int x = view->scene_tree->node.x;
    int y = view->scene_tree->node.y;

    char window_id[64];
    snprintf(window_id, sizeof(window_id), "window-%p", (void *)view);
    osf_window_set_geometry(window_id, x, y, geo.width, geo.height);
  }
}

static void view_destroy(struct wl_listener *listener, void *data) {
  struct osf_view *view = wl_container_of(listener, view, destroy);

  (void)data;

  /* Unregister from openSEF Framework */
  if (view->framework_window) {
    osf_window_destroy(view->framework_window);
    view->framework_window = NULL;
    wlr_log(WLR_INFO, "Window unregistered from framework");
  }

  /* Destroy custom titlebar */
  /* NO-OP: Server-side titlebars disabled */
  /*
  if (view->titlebar) {
    osf_titlebar_destroy(view->titlebar);
    view->titlebar = NULL;
  }
  */

  wl_list_remove(&view->map.link);
  wl_list_remove(&view->unmap.link);
  wl_list_remove(&view->commit.link);
  wl_list_remove(&view->destroy.link);
  wl_list_remove(&view->request_move.link);
  wl_list_remove(&view->request_resize.link);
  wl_list_remove(&view->request_maximize.link);
  wl_list_remove(&view->request_fullscreen.link);

  free(view);
}

/* ============================================================================
 * Move and resize
 * ============================================================================
 */

static void begin_interactive(struct osf_view *view, enum osf_cursor_mode mode,
                              uint32_t edges) {
  struct osf_server *server = view->server;
  struct wlr_surface *focused = server->seat->pointer_state.focused_surface;

  if (view->xdg_toplevel->base->surface !=
      wlr_surface_get_root_surface(focused)) {
    return; /* Not focused, ignore */
  }

  server->grabbed_view = view;
  server->cursor_mode = mode;

  if (mode == OSF_CURSOR_MOVE) {
    server->grab_x = server->cursor->x - view->scene_tree->node.x;
    server->grab_y = server->cursor->y - view->scene_tree->node.y;
  } else {
    struct wlr_box geo;
    // wlr_xdg_surface_get_geometry(view->xdg_toplevel->base, &geo);
    // In wlroots 0.18+, access geometry directly from surface state
    geo = view->xdg_toplevel->base->current.geometry;

    double border_x = (view->scene_tree->node.x + geo.x) +
                      ((edges & WLR_EDGE_RIGHT) ? geo.width : 0);
    double border_y = (view->scene_tree->node.y + geo.y) +
                      ((edges & WLR_EDGE_BOTTOM) ? geo.height : 0);

    server->grab_x = server->cursor->x - border_x;
    server->grab_y = server->cursor->y - border_y;
    server->grab_geobox = geo;
    server->grab_geobox.x += view->scene_tree->node.x;
    server->grab_geobox.y += view->scene_tree->node.y;
    server->resize_edges = edges;
  }
}

static void view_request_move(struct wl_listener *listener, void *data) {
  struct osf_view *view = wl_container_of(listener, view, request_move);
  (void)data;
  begin_interactive(view, OSF_CURSOR_MOVE, 0);
}

static void view_request_resize(struct wl_listener *listener, void *data) {
  struct osf_view *view = wl_container_of(listener, view, request_resize);
  struct wlr_xdg_toplevel_resize_event *event = data;
  begin_interactive(view, OSF_CURSOR_RESIZE, event->edges);
}

static void view_request_maximize(struct wl_listener *listener, void *data) {
  struct osf_view *view = wl_container_of(listener, view, request_maximize);
  (void)data;

  if (view->xdg_toplevel->base->surface->mapped) {
    bool next_max = !view->xdg_toplevel->current.maximized;
    wlr_xdg_toplevel_set_maximized(view->xdg_toplevel, next_max);

    /* Report to framework */
    char window_id[64];
    snprintf(window_id, sizeof(window_id), "window-%p", (void *)view);
    struct wlr_box geo = view->xdg_toplevel->base->current.geometry;

    /* If maximizing, we assume it takes full screen (or output size)
     * In this compositor, we'll report the output size if maxed.
     */
    if (next_max) {
      osf_window_set_geometry(window_id, 0, 28, 1920,
                              1052); // Assuming 1080p - panel
    } else {
      osf_window_set_geometry(window_id, 50, 50, geo.width, geo.height);
    }
  }
}

static void view_request_fullscreen(struct wl_listener *listener, void *data) {
  struct osf_view *view = wl_container_of(listener, view, request_fullscreen);
  (void)data;

  if (view->xdg_toplevel->base->surface->mapped) {
    wlr_xdg_toplevel_set_fullscreen(view->xdg_toplevel,
                                    !view->xdg_toplevel->current.fullscreen);
  }
}

/* ============================================================================
 * New XDG toplevel
 * ============================================================================
 */

void osf_new_xdg_toplevel(struct wl_listener *listener, void *data) {
  struct osf_server *server =
      wl_container_of(listener, server, new_xdg_toplevel);
  struct wlr_xdg_toplevel *xdg_toplevel = data;
  if (!xdg_toplevel || !xdg_toplevel->base) {
    wlr_log(WLR_ERROR, "CRITICAL: New XDG toplevel has INVALID state!");
    return;
  }

  const char *app_id = xdg_toplevel->app_id ? xdg_toplevel->app_id : "unknown";
  const char *title = xdg_toplevel->title ? xdg_toplevel->title : "untitled";

  wlr_log(WLR_INFO,
          "New XDG toplevel request received: app_id='%s', title='%s'", app_id,
          title);

  wlr_log(WLR_INFO, "New XDG toplevel: %s (app_id: %s)",
          xdg_toplevel->title ? xdg_toplevel->title : "(untitled)",
          xdg_toplevel->app_id ? xdg_toplevel->app_id : "(unknown)");

  /* Add surface commit logger to debug buffer attachment */
  // setup but we can trust view_commit will handle it once created.

  /* Create view */
  struct osf_view *view = calloc(1, sizeof(*view));
  if (!view) {
    wlr_log(WLR_ERROR, "Failed to allocate view");
    return;
  }

  view->server = server;
  view->xdg_toplevel = xdg_toplevel;

  /* Create scene tree in views layer */
  view->scene_tree = wlr_scene_tree_create(server->layer_views);
  view->content_tree =
      wlr_scene_xdg_surface_create(view->scene_tree, xdg_toplevel->base);
  view->scene_tree->node.data = view;
  xdg_toplevel->base->data = view;

  /* Create custom titlebar (skip for shell window) */
  /* NO-OP: Server-side titlebars disabled per AGENTS.md */
  view->titlebar = NULL;

  /*
  const char *app_id_check = xdg_toplevel->app_id ? xdg_toplevel->app_id : "";
  if (strcmp(app_id_check, "vitusos.shell") != 0) {
    view->titlebar = osf_titlebar_create(view->scene_tree, view);
    // Position content below titlebar
    wlr_scene_node_set_position(&view->content_tree->node, 0,
                                OSF_TITLEBAR_HEIGHT);
    wlr_log(WLR_INFO, "Created custom titlebar for: %s", app_id_check);
  } else {
    view->titlebar = NULL;
  }
  */

  /* Set up listeners */
  view->map.notify = view_map;
  wl_signal_add(&xdg_toplevel->base->surface->events.map, &view->map);

  view->unmap.notify = view_unmap;
  wl_signal_add(&xdg_toplevel->base->surface->events.unmap, &view->unmap);

  view->commit.notify = view_commit;
  wl_signal_add(&xdg_toplevel->base->surface->events.commit, &view->commit);

  view->destroy.notify = view_destroy;
  wl_signal_add(&xdg_toplevel->events.destroy, &view->destroy);

  view->request_move.notify = view_request_move;
  wl_signal_add(&xdg_toplevel->events.request_move, &view->request_move);

  view->request_resize.notify = view_request_resize;
  wl_signal_add(&xdg_toplevel->events.request_resize, &view->request_resize);

  view->request_maximize.notify = view_request_maximize;
  wl_signal_add(&xdg_toplevel->events.request_maximize,
                &view->request_maximize);

  view->request_fullscreen.notify = view_request_fullscreen;
  wl_signal_add(&xdg_toplevel->events.request_fullscreen,
                &view->request_fullscreen);
}
