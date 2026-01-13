/**
 * titlebar.c - OpenSEF Custom Window Titlebar Implementation
 *
 * Draws Ares Design Language window decorations using wlr_scene_rect.
 */

#include "titlebar.h"
#include "server.h"

#include <stdlib.h>
#include <wlr/util/log.h>

struct osf_titlebar *osf_titlebar_create(struct wlr_scene_tree *parent,
                                         struct osf_view *view) {
  struct osf_titlebar *titlebar = calloc(1, sizeof(*titlebar));
  if (!titlebar) {
    wlr_log(WLR_ERROR, "Failed to allocate titlebar");
    return NULL;
  }

  titlebar->view = view;
  titlebar->active = true;
  titlebar->width = 800; /* Default, will be updated */

  /* Create container tree positioned above window */
  titlebar->tree = wlr_scene_tree_create(parent);
  wlr_scene_node_set_position(&titlebar->tree->node, 0, -OSF_TITLEBAR_HEIGHT);

  /* Background - Warm Ceramic */
  float ceramic[4] = {OSF_COLOR_CERAMIC_R, OSF_COLOR_CERAMIC_G,
                      OSF_COLOR_CERAMIC_B, 1.0f};
  titlebar->background = wlr_scene_rect_create(titlebar->tree, titlebar->width,
                                               OSF_TITLEBAR_HEIGHT, ceramic);

  /* Close button - Space Orange */
  float close_color[4] = {OSF_COLOR_CLOSE_R, OSF_COLOR_CLOSE_G,
                          OSF_COLOR_CLOSE_B, 1.0f};
  titlebar->btn_close = wlr_scene_rect_create(titlebar->tree, OSF_BUTTON_SIZE,
                                              OSF_BUTTON_SIZE, close_color);
  wlr_scene_node_set_position(&titlebar->btn_close->node,
                              OSF_BUTTON_MARGIN_LEFT, OSF_BUTTON_MARGIN_TOP);

  /* Minimize button - Mars Gold */
  float minimize_color[4] = {OSF_COLOR_MINIMIZE_R, OSF_COLOR_MINIMIZE_G,
                             OSF_COLOR_MINIMIZE_B, 1.0f};
  titlebar->btn_minimize = wlr_scene_rect_create(
      titlebar->tree, OSF_BUTTON_SIZE, OSF_BUTTON_SIZE, minimize_color);
  wlr_scene_node_set_position(&titlebar->btn_minimize->node,
                              OSF_BUTTON_MARGIN_LEFT + OSF_BUTTON_SIZE +
                                  OSF_BUTTON_SPACING,
                              OSF_BUTTON_MARGIN_TOP);

  /* Maximize button - Mission Blue */
  float maximize_color[4] = {OSF_COLOR_MAXIMIZE_R, OSF_COLOR_MAXIMIZE_G,
                             OSF_COLOR_MAXIMIZE_B, 1.0f};
  titlebar->btn_maximize = wlr_scene_rect_create(
      titlebar->tree, OSF_BUTTON_SIZE, OSF_BUTTON_SIZE, maximize_color);
  wlr_scene_node_set_position(&titlebar->btn_maximize->node,
                              OSF_BUTTON_MARGIN_LEFT +
                                  (OSF_BUTTON_SIZE + OSF_BUTTON_SPACING) * 2,
                              OSF_BUTTON_MARGIN_TOP);

  wlr_log(WLR_INFO, "[Titlebar] Created with traffic lights");

  return titlebar;
}

void osf_titlebar_destroy(struct osf_titlebar *titlebar) {
  if (!titlebar)
    return;

  if (titlebar->tree) {
    wlr_scene_node_destroy(&titlebar->tree->node);
  }
  free(titlebar);
}

void osf_titlebar_set_width(struct osf_titlebar *titlebar, int width) {
  if (!titlebar || width <= 0)
    return;

  titlebar->width = width;
  wlr_scene_rect_set_size(titlebar->background, width, OSF_TITLEBAR_HEIGHT);
}

void osf_titlebar_set_active(struct osf_titlebar *titlebar, bool active) {
  if (!titlebar)
    return;

  titlebar->active = active;

  /* Fade buttons when inactive */
  float alpha = active ? 1.0f : 0.5f;

  float close_color[4] = {OSF_COLOR_CLOSE_R, OSF_COLOR_CLOSE_G,
                          OSF_COLOR_CLOSE_B, alpha};
  wlr_scene_rect_set_color(titlebar->btn_close, close_color);

  float minimize_color[4] = {OSF_COLOR_MINIMIZE_R, OSF_COLOR_MINIMIZE_G,
                             OSF_COLOR_MINIMIZE_B, alpha};
  wlr_scene_rect_set_color(titlebar->btn_minimize, minimize_color);

  float maximize_color[4] = {OSF_COLOR_MAXIMIZE_R, OSF_COLOR_MAXIMIZE_G,
                             OSF_COLOR_MAXIMIZE_B, alpha};
  wlr_scene_rect_set_color(titlebar->btn_maximize, maximize_color);
}

int osf_titlebar_hit_test(struct osf_titlebar *titlebar, int x, int y) {
  if (!titlebar)
    return 0;

  /* Y must be within titlebar (0 to TITLEBAR_HEIGHT above content) */
  if (y < -OSF_TITLEBAR_HEIGHT || y >= 0)
    return 0;

  int rel_y = y + OSF_TITLEBAR_HEIGHT; /* Convert to titlebar-relative */

  /* Check close button */
  if (x >= OSF_BUTTON_MARGIN_LEFT &&
      x < OSF_BUTTON_MARGIN_LEFT + OSF_BUTTON_SIZE &&
      rel_y >= OSF_BUTTON_MARGIN_TOP &&
      rel_y < OSF_BUTTON_MARGIN_TOP + OSF_BUTTON_SIZE) {
    return 1; /* Close */
  }

  /* Check minimize button */
  int min_x = OSF_BUTTON_MARGIN_LEFT + OSF_BUTTON_SIZE + OSF_BUTTON_SPACING;
  if (x >= min_x && x < min_x + OSF_BUTTON_SIZE &&
      rel_y >= OSF_BUTTON_MARGIN_TOP &&
      rel_y < OSF_BUTTON_MARGIN_TOP + OSF_BUTTON_SIZE) {
    return 2; /* Minimize */
  }

  /* Check maximize button */
  int max_x =
      OSF_BUTTON_MARGIN_LEFT + (OSF_BUTTON_SIZE + OSF_BUTTON_SPACING) * 2;
  if (x >= max_x && x < max_x + OSF_BUTTON_SIZE &&
      rel_y >= OSF_BUTTON_MARGIN_TOP &&
      rel_y < OSF_BUTTON_MARGIN_TOP + OSF_BUTTON_SIZE) {
    return 3; /* Maximize */
  }

  return 0; /* No button hit */
}

bool osf_titlebar_contains(struct osf_titlebar *titlebar, int x, int y) {
  if (!titlebar)
    return false;

  return (x >= 0 && x < titlebar->width && y >= -OSF_TITLEBAR_HEIGHT && y < 0);
}
