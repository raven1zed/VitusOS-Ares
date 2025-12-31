/**
 * view.c - View (Window) Management
 *
 * Handles XDG toplevel windows: creation, focus, move, resize.
 */

#include "server.h"

#include <stdlib.h>
#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/util/log.h>

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
    }
  }

  /* Move view to front */
  wlr_scene_node_raise_to_top(&view->scene_tree->node);
  wl_list_remove(&view->link);
  wl_list_insert(&server->views, &view->link);

  /* Activate view */
  wlr_xdg_toplevel_set_activated(view->xdg_toplevel, true);

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
  osf_focus_view(view, view->xdg_toplevel->base->surface);

  wlr_log(WLR_INFO, "View mapped: %s",
          view->xdg_toplevel->title ? view->xdg_toplevel->title : "(untitled)");
}

static void view_unmap(struct wl_listener *listener, void *data) {
  struct osf_view *view = wl_container_of(listener, view, unmap);

  (void)data;

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
    /* Set initial size */
    wlr_xdg_toplevel_set_size(view->xdg_toplevel, 0, 0);
  }
}

static void view_destroy(struct wl_listener *listener, void *data) {
  struct osf_view *view = wl_container_of(listener, view, destroy);

  (void)data;

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
    wlr_xdg_toplevel_set_maximized(view->xdg_toplevel,
                                   !view->xdg_toplevel->current.maximized);
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

  wlr_log(WLR_INFO, "New XDG toplevel: %s",
          xdg_toplevel->title ? xdg_toplevel->title : "(untitled)");

  /* Create view */
  struct osf_view *view = calloc(1, sizeof(*view));
  if (!view) {
    wlr_log(WLR_ERROR, "Failed to allocate view");
    return;
  }

  view->server = server;
  view->xdg_toplevel = xdg_toplevel;

  /* Create scene tree in views layer */
  view->scene_tree =
      wlr_scene_xdg_surface_create(server->layer_views, xdg_toplevel->base);
  view->scene_tree->node.data = view;
  xdg_toplevel->base->data = view->scene_tree;

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
