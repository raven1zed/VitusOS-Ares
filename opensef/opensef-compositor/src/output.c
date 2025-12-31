/**
 * output.c - Output (Monitor) Handling
 *
 * Handles new monitors, frame rendering, and output configuration.
 */

#define WLR_USE_UNSTABLE

#include "server.h"

#include <stdlib.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/util/log.h>

static void output_frame(struct wl_listener *listener, void *data) {
  struct osf_output *output = wl_container_of(listener, output, frame);
  struct wlr_scene *scene = output->server->scene;
  struct wlr_scene_output *scene_output = output->scene_output;

  (void)data;

  /* Render the scene */
  wlr_scene_output_commit(scene_output, NULL);

  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  wlr_scene_output_send_frame_done(scene_output, &now);
}

static void output_request_state(struct wl_listener *listener, void *data) {
  struct osf_output *output = wl_container_of(listener, output, request_state);
  const struct wlr_output_event_request_state *event = data;

  wlr_output_commit_state(output->wlr_output, event->state);
}

static void output_destroy(struct wl_listener *listener, void *data) {
  struct osf_output *output = wl_container_of(listener, output, destroy);

  (void)data;

  wlr_log(WLR_INFO, "Output '%s' disconnected", output->wlr_output->name);

  wl_list_remove(&output->frame.link);
  wl_list_remove(&output->request_state.link);
  wl_list_remove(&output->destroy.link);
  wl_list_remove(&output->link);
  free(output);
}

void osf_new_output(struct wl_listener *listener, void *data) {
  struct osf_server *server = wl_container_of(listener, server, new_output);
  struct wlr_output *wlr_output = data;

  wlr_log(WLR_INFO, "New output: %s (%s %s)", wlr_output->name,
          wlr_output->make ? wlr_output->make : "unknown",
          wlr_output->model ? wlr_output->model : "unknown");

  /* Initialize output with allocator (required for wlroots 0.16+) */
  wlr_output_init_render(wlr_output, server->allocator, server->renderer);

  /* Set preferred mode */
  struct wlr_output_state state;
  wlr_output_state_init(&state);
  wlr_output_state_set_enabled(&state, true);

  struct wlr_output_mode *mode = wlr_output_preferred_mode(wlr_output);
  if (mode) {
    wlr_output_state_set_mode(&state, mode);
    wlr_log(WLR_INFO, "  Mode: %dx%d @ %.3f Hz", mode->width, mode->height,
            mode->refresh / 1000.0f);
  }

  wlr_output_commit_state(wlr_output, &state);
  wlr_output_state_finish(&state);

  /* Create output structure */
  struct osf_output *output = calloc(1, sizeof(*output));
  if (!output) {
    wlr_log(WLR_ERROR, "Failed to allocate output");
    return;
  }

  output->server = server;
  output->wlr_output = wlr_output;

  /* Add to output layout */
  struct wlr_output_layout_output *l_output =
      wlr_output_layout_add_auto(server->output_layout, wlr_output);

  /* Create scene output */
  output->scene_output = wlr_scene_output_create(server->scene, wlr_output);
  wlr_scene_output_layout_add_output(server->scene_layout, l_output,
                                     output->scene_output);

  /* Set up listeners */
  output->frame.notify = output_frame;
  wl_signal_add(&wlr_output->events.frame, &output->frame);

  output->request_state.notify = output_request_state;
  wl_signal_add(&wlr_output->events.request_state, &output->request_state);

  output->destroy.notify = output_destroy;
  wl_signal_add(&wlr_output->events.destroy, &output->destroy);

  /* Add to server list */
  wl_list_insert(&server->outputs, &output->link);

  wlr_log(WLR_INFO, "Output '%s' configured successfully", wlr_output->name);

  /* Set background color (VitusOS Space Grey) */
  float color[4] = {0.1f, 0.1f, 0.1f, 1.0f}; // #1a1a1a
  int width = wlr_output->width;
  int height = wlr_output->height;
  if (width == 0)
    width = 1920; // Fallback
  if (height == 0)
    height = 1080;

  wlr_scene_rect_create(server->layer_background, width, height, color);
}
