/**
 * layer_shell.c - Layer Shell Support
 *
 * Handles wlr-layer-shell protocol for dock, panel, and overlay surfaces.
 * This enables external C++ UI applications to integrate with the compositor.
 */

#include "server.h"

#include <stdlib.h>
#include <wlr/types/wlr_layer_shell_v1.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/util/log.h>

static struct wlr_scene_tree *
get_layer_tree(struct osf_server *server,
               enum zwlr_layer_shell_v1_layer layer) {
  switch (layer) {
  case ZWLR_LAYER_SHELL_V1_LAYER_BACKGROUND:
    return server->layer_background;
  case ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM:
    return server->layer_bottom;
  case ZWLR_LAYER_SHELL_V1_LAYER_TOP:
    return server->layer_top;
  case ZWLR_LAYER_SHELL_V1_LAYER_OVERLAY:
    return server->layer_overlay;
  default:
    return server->layer_top;
  }
}

static void arrange_layers(struct osf_server *server) {
  /* Arrange layer surfaces for all outputs */
  struct osf_output *output;
  wl_list_for_each(output, &server->outputs, link) {
    struct wlr_box full_area = {0, 0, 0, 0};
    wlr_output_effective_resolution(output->wlr_output, &full_area.width,
                                    &full_area.height);

    struct wlr_box usable_area = full_area;

    /* Process each layer surface */
    struct osf_layer_surface *layer_surface;
    wl_list_for_each(layer_surface, &server->layer_surfaces, link) {
      struct wlr_layer_surface_v1 *wlr_layer = layer_surface->layer_surface;

      if (wlr_layer->output != output->wlr_output) {
        continue;
      }

      /* Safety check: Don't touch uninitialized surfaces */
      if (!wlr_layer->initialized || !layer_surface->scene_layer_surface) {
        continue;
      }

      /* Let wlr_scene handle the arrangement */
      wlr_scene_layer_surface_v1_configure(layer_surface->scene_layer_surface,
                                           &full_area, &usable_area);
    }
  }
}

static void layer_surface_map(struct wl_listener *listener, void *data) {
  struct osf_layer_surface *layer_surface =
      wl_container_of(listener, layer_surface, map);

  (void)data;

  layer_surface->mapped = true;
  arrange_layers(layer_surface->server);

  wlr_log(WLR_INFO, "Layer surface mapped: %s",
          layer_surface->layer_surface->namespace);
}

static void layer_surface_unmap(struct wl_listener *listener, void *data) {
  struct osf_layer_surface *layer_surface =
      wl_container_of(listener, layer_surface, unmap);

  (void)data;

  layer_surface->mapped = false;
  arrange_layers(layer_surface->server);

  wlr_log(WLR_INFO, "Layer surface unmapped");
}

static void layer_surface_commit(struct wl_listener *listener, void *data) {
  struct osf_layer_surface *layer_surface =
      wl_container_of(listener, layer_surface, commit);
  struct wlr_layer_surface_v1 *wlr_layer = layer_surface->layer_surface;

  (void)data;

  if (!wlr_layer->initialized) {
    return;
  }

  if (wlr_layer->initial_commit) {
    /* Assign to first output if not specified */
    if (!wlr_layer->output) {
      struct osf_output *output;
      wl_list_for_each(output, &layer_surface->server->outputs, link) {
        wlr_layer->output = output->wlr_output;
        break;
      }
      if (!wlr_layer->output) {
        wlr_log(WLR_ERROR, "No output available for layer surface");
        wlr_layer_surface_v1_destroy(wlr_layer);
        return;
      }
    }
  }

  arrange_layers(layer_surface->server);
}

static void layer_surface_destroy(struct wl_listener *listener, void *data) {
  struct osf_layer_surface *layer_surface =
      wl_container_of(listener, layer_surface, destroy);

  (void)data;

  wlr_log(WLR_INFO, "Layer surface destroyed: %s",
          layer_surface->layer_surface->namespace);

  wl_list_remove(&layer_surface->map.link);
  wl_list_remove(&layer_surface->unmap.link);
  wl_list_remove(&layer_surface->commit.link);
  wl_list_remove(&layer_surface->destroy.link);
  wl_list_remove(&layer_surface->link);

  free(layer_surface);
}

void osf_new_layer_surface(struct wl_listener *listener, void *data) {
  struct osf_server *server =
      wl_container_of(listener, server, new_layer_surface);
  struct wlr_layer_surface_v1 *wlr_layer = data;

  wlr_log(WLR_INFO, "New layer surface: namespace='%s' layer=%d",
          wlr_layer->namespace, wlr_layer->pending.layer);

  /* Create layer surface structure */
  struct osf_layer_surface *layer_surface = calloc(1, sizeof(*layer_surface));
  if (!layer_surface) {
    wlr_log(WLR_ERROR, "Failed to allocate layer surface");
    wlr_layer_surface_v1_destroy(wlr_layer);
    return;
  }

  layer_surface->server = server;
  layer_surface->layer_surface = wlr_layer;

  /* Get appropriate layer tree */
  struct wlr_scene_tree *parent =
      get_layer_tree(server, wlr_layer->pending.layer);

  /* Create scene layer surface */
  layer_surface->scene_layer_surface =
      wlr_scene_layer_surface_v1_create(parent, wlr_layer);

  if (!layer_surface->scene_layer_surface) {
    wlr_log(WLR_ERROR, "Failed to create scene layer surface");
    free(layer_surface);
    wlr_layer_surface_v1_destroy(wlr_layer);
    return;
  }

  /* Store reference */
  wlr_layer->data = layer_surface;

  /* Set up listeners */
  layer_surface->map.notify = layer_surface_map;
  wl_signal_add(&wlr_layer->surface->events.map, &layer_surface->map);

  layer_surface->unmap.notify = layer_surface_unmap;
  wl_signal_add(&wlr_layer->surface->events.unmap, &layer_surface->unmap);

  layer_surface->commit.notify = layer_surface_commit;
  wl_signal_add(&wlr_layer->surface->events.commit, &layer_surface->commit);

  layer_surface->destroy.notify = layer_surface_destroy;
  wl_signal_add(&wlr_layer->events.destroy, &layer_surface->destroy);

  /* Add to server list */
  wl_list_insert(&server->layer_surfaces, &layer_surface->link);
}
