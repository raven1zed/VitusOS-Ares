/**
 * titlebar.h - OpenSEF Custom Window Titlebar
 *
 * Implements Ares Design Language window decorations:
 * - Traffic lights (Orange/Gold/Blue)
 * - Warm Ceramic background (#FAFAF9)
 * - 32px title bar height
 */

#ifndef OSF_TITLEBAR_H
#define OSF_TITLEBAR_H

#include <stdbool.h>
#include <wlr/types/wlr_scene.h>

/* Forward declarations */
struct osf_view;

/* Ares Design Colors */
#define OSF_COLOR_CERAMIC_R 0.98f
#define OSF_COLOR_CERAMIC_G 0.98f
#define OSF_COLOR_CERAMIC_B 0.976f

#define OSF_COLOR_CLOSE_R 0.91f
#define OSF_COLOR_CLOSE_G 0.36f
#define OSF_COLOR_CLOSE_B 0.02f

#define OSF_COLOR_MINIMIZE_R 0.83f
#define OSF_COLOR_MINIMIZE_G 0.66f
#define OSF_COLOR_MINIMIZE_B 0.24f

#define OSF_COLOR_MAXIMIZE_R 0.24f
#define OSF_COLOR_MAXIMIZE_G 0.35f
#define OSF_COLOR_MAXIMIZE_B 0.50f

/* Dimensions (from openSEF Design Reference) */
#define OSF_TITLEBAR_HEIGHT 32
#define OSF_BUTTON_SIZE 12
#define OSF_BUTTON_SPACING 8
#define OSF_BUTTON_MARGIN_LEFT 12
#define OSF_BUTTON_MARGIN_TOP 10
#define OSF_CORNER_RADIUS 16

/**
 * Titlebar structure
 */
struct osf_titlebar {
  struct wlr_scene_tree *tree;

  /* Background */
  struct wlr_scene_rect *background;

  /* Traffic light buttons */
  struct wlr_scene_rect *btn_close;
  struct wlr_scene_rect *btn_minimize;
  struct wlr_scene_rect *btn_maximize;

  /* Title text (future: wlr_scene_buffer with rendered text) */
  /* For now, buttons only */

  int width;
  bool active;

  /* Parent view */
  struct osf_view *view;
};

/**
 * Create a titlebar for a view.
 * The titlebar is positioned ABOVE the window content (y = -32).
 */
struct osf_titlebar *osf_titlebar_create(struct wlr_scene_tree *parent,
                                         struct osf_view *view);

/**
 * Destroy a titlebar.
 */
void osf_titlebar_destroy(struct osf_titlebar *titlebar);

/**
 * Update titlebar width to match window width.
 */
void osf_titlebar_set_width(struct osf_titlebar *titlebar, int width);

/**
 * Update titlebar active state (changes button opacity).
 */
void osf_titlebar_set_active(struct osf_titlebar *titlebar, bool active);

/**
 * Hit test: returns which button (if any) was clicked.
 * Returns: 0 = none, 1 = close, 2 = minimize, 3 = maximize
 */
int osf_titlebar_hit_test(struct osf_titlebar *titlebar, int x, int y);

/**
 * Check if position is within titlebar (for dragging).
 */
bool osf_titlebar_contains(struct osf_titlebar *titlebar, int x, int y);

#endif /* OSF_TITLEBAR_H */
