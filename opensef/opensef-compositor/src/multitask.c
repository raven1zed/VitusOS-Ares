/**
 * multitask.c - Overview Mode Implementation
 */

#include "server.h"
#include <stdlib.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/util/log.h>

void osf_multitask_init(struct osf_server *server) {
  server->multitask = calloc(1, sizeof(struct osf_multitask));
  server->multitask->server = server;
  server->multitask->scene_tree = wlr_scene_tree_create(&server->scene->tree);
  wlr_scene_node_set_enabled(&server->multitask->scene_tree->node, false);
}

void osf_multitask_toggle(struct osf_server *server) {
  struct osf_multitask *mt = server->multitask;
  mt->active = !mt->active;
  wlr_scene_node_set_enabled(&mt->scene_tree->node, mt->active);

  if (mt->active) {
    wlr_log(WLR_INFO, "Multitask View activated");
    // Save current states and move to grid
    struct osf_view *view;
    wl_list_for_each(view, &server->views, link) {
      if (view->mapped) {
        view->saved_x = view->scene_tree->node.x;
        view->saved_y = view->scene_tree->node.y;
        view->saved_w = view->xdg_toplevel->current.width;
        view->saved_h = view->xdg_toplevel->current.height;
      }
    }
    osf_multitask_update(server);
  } else {
    wlr_log(WLR_INFO, "Multitask View deactivated");
    // Restore windows
    struct osf_view *view;
    wl_list_for_each(view, &server->views, link) {
      if (view->mapped) {
        wlr_scene_node_set_position(&view->scene_tree->node, view->saved_x,
                                    view->saved_y);
      }
    }
  }
}

void osf_multitask_update(struct osf_server *server) {
  struct osf_multitask *mt = server->multitask;
  if (!mt->active)
    return;

  // Count mapped views
  int count = 0;
  struct osf_view *view;
  wl_list_for_each(view, &server->views, link) {
    if (view->mapped)
      count++;
  }
  if (count == 0)
    return;

  // Simple grid layout on the first output
  struct osf_output *output =
      wl_container_of(server->outputs.next, output, link);
  struct wlr_box box;
  wlr_output_layout_get_box(server->output_layout, output->wlr_output, &box);

  int cols = ceil(sqrt(count));
  int rows = ceil((float)count / cols);

  int i = 0;
  int padding = 40;
  int slot_w = (box.width - padding * (cols + 1)) / cols;
  int slot_h = (box.height - padding * (rows + 1)) / rows;

  wl_list_for_each(view, &server->views, link) {
    if (!view->mapped)
      continue;

    int r = i / cols;
    int c = i % cols;
    int x = box.x + padding + c * (slot_w + padding);
    int y = box.y + padding + r * (slot_h + padding);

    wlr_scene_node_set_position(&view->scene_tree->node, x, y);
    // TODO: Apply scene node scaling once verified
    i++;
  }
}
