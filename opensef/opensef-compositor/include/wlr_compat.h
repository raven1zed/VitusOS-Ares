/**
 * wlr_compat.h - Complete C wrapper for wlroots
 *
 * wlroots 0.19 uses C99 syntax that is not valid in C++.
 * This header provides C++ compatible types and function declarations.
 *
 * C++ code should include ONLY this header for wlroots functionality.
 * Never include wlroots headers directly in C++ code.
 */

#ifndef OSF_WLR_COMPAT_H
#define OSF_WLR_COMPAT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Opaque type declarations (C++ doesn't need to know the struct layout)
 * ============================================================================
 */

struct wl_display;
struct wl_event_loop;
struct wl_listener;
struct wl_signal;
struct wl_list;

struct wlr_backend;
struct wlr_renderer;
struct wlr_allocator;
struct wlr_compositor;
struct wlr_subcompositor;
struct wlr_data_device_manager;
struct wlr_output;
struct wlr_output_layout;
struct wlr_scene;
struct wlr_scene_output_layout;
struct wlr_scene_tree;
struct wlr_scene_node;
struct wlr_scene_rect;
struct wlr_scene_buffer;
struct wlr_xdg_shell;
struct wlr_xdg_toplevel;
struct wlr_xdg_surface;
struct wlr_seat;
struct wlr_cursor;
struct wlr_xcursor_manager;
struct wlr_keyboard;
struct wlr_surface;
struct wlr_buffer;
struct wlr_texture;

/* ============================================================================
 * Scene rect functions (wrapped due to C99 [static 4] syntax)
 * ============================================================================
 */

/* Create a scene rect - wrapped for C++ compatibility */
struct wlr_scene_rect *osf_scene_rect_create(struct wlr_scene_tree *parent,
                                             int width, int height, float r,
                                             float g, float b, float a);

/* Set rect color - wrapped for C++ compatibility */
void osf_scene_rect_set_color(struct wlr_scene_rect *rect, float r, float g,
                              float b, float a);

/* Set rect size */
void osf_scene_rect_set_size(struct wlr_scene_rect *rect, int width,
                             int height);

/* ============================================================================
 * Scene node functions
 * ============================================================================
 */

/* Set node position */
void osf_scene_node_set_position(struct wlr_scene_node *node, int x, int y);

/* Set node enabled */
void osf_scene_node_set_enabled(struct wlr_scene_node *node, bool enabled);

/* Destroy node */
void osf_scene_node_destroy(struct wlr_scene_node *node);

/* Get node from rect */
struct wlr_scene_node *osf_scene_rect_get_node(struct wlr_scene_rect *rect);

/* Get node from buffer */
struct wlr_scene_node *
osf_scene_buffer_get_node(struct wlr_scene_buffer *buffer);

/* ============================================================================
 * Scene tree functions
 * ============================================================================
 */

/* Create scene tree */
struct wlr_scene_tree *osf_scene_tree_create(struct wlr_scene_tree *parent);

/* Get tree from scene */
struct wlr_scene_tree *osf_scene_get_tree(struct wlr_scene *scene);

/* ============================================================================
 * Buffer functions
 * ============================================================================
 */

void osf_buffer_drop(struct wlr_buffer *buffer);

#ifdef __cplusplus
}
#endif

#endif /* OSF_WLR_COMPAT_H */
