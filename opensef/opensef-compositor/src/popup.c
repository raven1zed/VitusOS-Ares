/**
 * XDG Popup Handler
 */

#include "server.h"
#include <wlr/types/wlr_scene.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/util/log.h>

void osf_new_xdg_popup(struct wl_listener *listener, void *data) {
  struct wlr_xdg_popup *xdg_popup = data;
  struct osf_server *server = wl_container_of(listener, server, new_xdg_popup);

  wlr_log(WLR_INFO, "New XDG popup created");

  /* Get the parent surface's scene node */
  struct wlr_xdg_surface *parent =
      wlr_xdg_surface_try_from_wlr_surface(xdg_popup->parent);
  if (!parent) {
    wlr_log(WLR_ERROR, "Popup parent is not an XDG surface");
    return;
  }

  /* Get the parent's scene tree */
  struct wlr_scene_tree *parent_tree = NULL;

  // Try to see if it's an osf_view (toplevel) or a scene_tree (popup)
  // We use the node data of the scene_tree to identify osf_view
  struct osf_view *view = parent->data;
  if (view && view->content_tree) {
    parent_tree = view->content_tree;
  } else {
    parent_tree = parent->data;
  }

  if (!parent_tree) {
    wlr_log(WLR_ERROR, "Popup parent has no scene tree, using overlay layer");
    parent_tree = server->layer_overlay;
  }

  /* Create scene node for popup */
  struct wlr_scene_tree *popup_tree =
      wlr_scene_xdg_surface_create(parent_tree, xdg_popup->base);
  xdg_popup->base->data = popup_tree;

  wlr_log(WLR_INFO, "XDG popup scene node created");
}
