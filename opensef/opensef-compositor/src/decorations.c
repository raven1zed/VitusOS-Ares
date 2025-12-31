/**
 * decorations.c - Server-Side Window Decorations
 *
 * Handles XDG decoration protocol to ensure server-side decorations.
 */

#include "server.h"

#include <stdlib.h>
#include <wlr/types/wlr_xdg_decoration_v1.h>
#include <wlr/util/log.h>

struct osf_xdg_decoration {
  struct wlr_xdg_toplevel_decoration_v1 *wlr_decoration;
  struct osf_server *server;

  struct wl_listener destroy;
  struct wl_listener request_mode;
};

static void decoration_request_mode(struct wl_listener *listener, void *data) {
  struct osf_xdg_decoration *decoration =
      wl_container_of(listener, decoration, request_mode);

  (void)data;

  /* Always prefer server-side decorations */
  wlr_xdg_toplevel_decoration_v1_set_mode(
      decoration->wlr_decoration,
      WLR_XDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
}

static void decoration_destroy(struct wl_listener *listener, void *data) {
  struct osf_xdg_decoration *decoration =
      wl_container_of(listener, decoration, destroy);

  (void)data;

  wl_list_remove(&decoration->destroy.link);
  wl_list_remove(&decoration->request_mode.link);
  free(decoration);
}

void osf_new_xdg_decoration(struct wl_listener *listener, void *data) {
  struct osf_server *server =
      wl_container_of(listener, server, new_xdg_decoration);
  struct wlr_xdg_toplevel_decoration_v1 *wlr_decoration = data;

  wlr_log(WLR_INFO, "New XDG decoration request");

  struct osf_xdg_decoration *decoration = calloc(1, sizeof(*decoration));
  if (!decoration) {
    wlr_log(WLR_ERROR, "Failed to allocate decoration");
    return;
  }

  decoration->wlr_decoration = wlr_decoration;
  decoration->server = server;

  decoration->destroy.notify = decoration_destroy;
  wl_signal_add(&wlr_decoration->events.destroy, &decoration->destroy);

  decoration->request_mode.notify = decoration_request_mode;
  wl_signal_add(&wlr_decoration->events.request_mode,
                &decoration->request_mode);

  /* Set server-side decorations immediately */
  wlr_xdg_toplevel_decoration_v1_set_mode(
      wlr_decoration, WLR_XDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
}
