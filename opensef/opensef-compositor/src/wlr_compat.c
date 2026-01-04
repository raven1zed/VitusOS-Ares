/**
 * wlr_compat.c - C wrapper implementation for wlroots
 *
 * This file MUST be compiled as pure C (not C++)
 * It wraps wlroots functions that use C99 syntax incompatible with C++
 */

#define WLR_USE_UNSTABLE

#include <wlr/types/wlr_buffer.h>
#include <wlr/types/wlr_scene.h>


/* ============================================================================
 * Scene rect functions
 * ============================================================================
 */

struct wlr_scene_rect *osf_scene_rect_create(struct wlr_scene_tree *parent,
                                             int width, int height, float r,
                                             float g, float b, float a) {
  float color[4] = {r, g, b, a};
  return wlr_scene_rect_create(parent, width, height, color);
}

void osf_scene_rect_set_color(struct wlr_scene_rect *rect, float r, float g,
                              float b, float a) {
  float color[4] = {r, g, b, a};
  wlr_scene_rect_set_color(rect, color);
}

void osf_scene_rect_set_size(struct wlr_scene_rect *rect, int width,
                             int height) {
  wlr_scene_rect_set_size(rect, width, height);
}

/* ============================================================================
 * Scene node functions
 * ============================================================================
 */

void osf_scene_node_set_position(struct wlr_scene_node *node, int x, int y) {
  wlr_scene_node_set_position(node, x, y);
}

void osf_scene_node_set_enabled(struct wlr_scene_node *node, bool enabled) {
  wlr_scene_node_set_enabled(node, enabled);
}

void osf_scene_node_destroy(struct wlr_scene_node *node) {
  wlr_scene_node_destroy(node);
}

struct wlr_scene_node *osf_scene_rect_get_node(struct wlr_scene_rect *rect) {
  return &rect->node;
}

struct wlr_scene_node *
osf_scene_buffer_get_node(struct wlr_scene_buffer *buffer) {
  return &buffer->node;
}

/* ============================================================================
 * Scene tree functions
 * ============================================================================
 */

struct wlr_scene_tree *osf_scene_tree_create(struct wlr_scene_tree *parent) {
  return wlr_scene_tree_create(parent);
}

struct wlr_scene_tree *osf_scene_get_tree(struct wlr_scene *scene) {
  return &scene->tree;
}

/* ============================================================================
 * Buffer functions
 * ============================================================================
 */

void osf_buffer_drop(struct wlr_buffer *buffer) { wlr_buffer_drop(buffer); }
