/**
 * tiling.c - Dynamic Tiling Implementation
 */

#include "tiling.h"
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/util/log.h>

static struct osf_tiling_config config = {
    .inner_gap = 10,
    .outer_gap = 15,
    .border_width = 2,
    .active_border_color = 0xFFE85D04, // Ares Orange
    .inactive_border_color = 0xFF404040,
};

void osf_tiling_init(struct osf_server *server) {
  wlr_log(WLR_INFO, "Tiling engine initialized (Gaps: %d/%d)", config.inner_gap,
          config.outer_gap);
}

static void apply_view_geometry(struct osf_view *view, int x, int y, int w,
                                int h) {
  int bw = config.border_width;
  wlr_scene_node_set_position(&view->scene_tree->node, x, y);

  // Resize to fit inside borders if we implement them as rects
  wlr_xdg_toplevel_set_size(view->xdg_toplevel, w - bw * 2, h - bw * 2);

  // Offset the content to center it within the "tile"
  wlr_scene_node_set_position(&view->content_tree->node, bw, bw);
}

void osf_tiling_arrange(struct osf_server *server, struct osf_output *output) {
  int view_count = 0;
  struct osf_view *view;
  wl_list_for_each(view, &server->views, link) {
    if (view->mapped && !view->xdg_toplevel->current.fullscreen) {
      view_count++;
    }
  }

  if (view_count == 0)
    return;

  struct wlr_box box;
  wlr_output_layout_get_box(server->output_layout, output->wlr_output, &box);

  int og = config.outer_gap;
  int ig = config.inner_gap;

  box.x += og;
  box.y += og;
  box.width -= og * 2;
  box.height -= og * 2;

  if (view_count == 1) {
    wl_list_for_each(view, &server->views, link) {
      if (view->mapped) {
        apply_view_geometry(view, box.x, box.y, box.width, box.height);
        break;
      }
    }
    return;
  }

  // Master/Stack layout: Master on left (60%), Stack on right
  int master_w = (box.width * 0.6) - (ig / 2);
  int stack_w = box.width - master_w - ig;
  int stack_h = (box.height - (ig * (view_count - 2))) / (view_count - 1);

  int i = 0;
  wl_list_for_each(view, &server->views, link) {
    if (!view->mapped)
      continue;

    if (i == 0) {
      // Master
      apply_view_geometry(view, box.x, box.y, master_w, box.height);
    } else {
      // Stack
      int y = box.y + (i - 1) * (stack_h + ig);
      apply_view_geometry(view, box.x + master_w + ig, y, stack_w, stack_h);
    }
    i++;
  }
}

void osf_view_update_borders(struct osf_view *view, bool active) {
  int bw = 2;
  uint32_t active_color = 0xFFE85D04;
  uint32_t inactive_color = 0xFF404040;
  uint32_t color = active ? active_color : inactive_color;
  float rgba[4];
  rgba[0] = ((color >> 16) & 0xFF) / 255.0f;
  rgba[1] = ((color >> 8) & 0xFF) / 255.0f;
  rgba[2] = (color & 0xFF) / 255.0f;
  rgba[3] = ((color >> 24) & 0xFF) / 255.0f;

  struct wlr_box box = view->xdg_toplevel->base->current.geometry;
  int w = box.width + bw * 2;
  int h = box.height + bw * 2;

  if (!view->border_top) {
    view->border_top = wlr_scene_rect_create(view->scene_tree, w, bw, rgba);
    view->border_bottom = wlr_scene_rect_create(view->scene_tree, w, bw, rgba);
    view->border_left = wlr_scene_rect_create(view->scene_tree, bw, h, rgba);
    view->border_right = wlr_scene_rect_create(view->scene_tree, bw, h, rgba);
  } else {
    wlr_scene_rect_set_color(view->border_top, rgba);
    wlr_scene_rect_set_color(view->border_bottom, rgba);
    wlr_scene_rect_set_color(view->border_left, rgba);
    wlr_scene_rect_set_color(view->border_right, rgba);
  }

  wlr_scene_node_set_position(&view->border_top->node, 0, 0);
  wlr_scene_node_set_position(&view->border_bottom->node, 0, h - bw);
  wlr_scene_node_set_position(&view->border_left->node, 0, 0);
  wlr_scene_node_set_position(&view->border_right->node, w - bw, 0);
}
