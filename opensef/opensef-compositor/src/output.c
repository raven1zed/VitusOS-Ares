/**
 * output.c - Output (Monitor) Handling
 *
 * Handles new monitors, frame rendering, and output configuration.
 */

#include "server.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wayland-server-core.h>
#include <wlr/backend/wayland.h>
#include <wlr/backend/x11.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/util/log.h>

static void output_frame(struct wl_listener *listener, void *data) {
  struct osf_output *output = wl_container_of(listener, output, frame);
  struct wlr_scene_output *scene_output = output->scene_output;

  (void)data;

  /* Render the scene */
  if (!wlr_scene_output_commit(scene_output, NULL)) {
    wlr_log(WLR_ERROR, "Failed to commit scene output frame");
  }

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

  struct wlr_output_mode *preferred = wlr_output_preferred_mode(wlr_output);
  if (preferred) {
    wlr_log(WLR_INFO, "  Preferred mode detected: %dx%d@%dmHz",
            preferred->width, preferred->height, preferred->refresh);
  } else {
    wlr_log(WLR_INFO, "  No preferred mode reported by backend");
  }

  /* Initialize output with allocator (required for wlroots 0.16+) */
  wlr_output_init_render(wlr_output, server->allocator, server->renderer);

  /* Force sane dimensions for nested backends (Wayland/X11) */
  int width = 1280;
  int height = 720;

  bool is_nested = (getenv("VITUS_BACKEND") != NULL);
  if (!is_nested && wlr_output->make) {
    if (strcmp(wlr_output->make, "wayland") == 0 ||
        strcmp(wlr_output->make, "x11") == 0) {
      is_nested = true;
    }
  }

  struct wlr_output_state state;
  wlr_output_state_init(&state);
  wlr_output_state_set_enabled(&state, true);

  bool committed = false;
  if (is_nested) {
    wlr_log(WLR_INFO, "  Attempting nested mode negotiation...");

    /* Strategy 1: Preferred mode (if not bogus) */
    if (preferred && preferred->width < 10000) {
      wlr_output_state_set_mode(&state, preferred);
      if (wlr_output_commit_state(wlr_output, &state)) {
        wlr_log(WLR_INFO, "  SUCCESS: Committed backend preferred mode");
        committed = true;
      }
    }

    /* Strategy 2: 720p @ 60Hz */
    if (!committed) {
      wlr_output_state_set_custom_mode(&state, 1280, 720, 60000);
      if (wlr_output_commit_state(wlr_output, &state)) {
        wlr_log(WLR_INFO, "  SUCCESS: Committed 1280x720 @ 60Hz");
        committed = true;
      }
    }

    /* Strategy 3: 720p @ 0Hz (WSLg special) */
    if (!committed) {
      wlr_output_state_set_custom_mode(&state, 1280, 720, 0);
      if (wlr_output_commit_state(wlr_output, &state)) {
        wlr_log(WLR_INFO, "  SUCCESS: Committed 1280x720 @ 0Hz");
        committed = true;
      }
    }

    /* Strategy 4: 800x600 @ 60Hz (Ultra-safe) */
    if (!committed) {
      wlr_output_state_set_custom_mode(&state, 800, 600, 60000);
      if (wlr_output_commit_state(wlr_output, &state)) {
        wlr_log(WLR_INFO, "  SUCCESS: Committed 800x600 @ 60Hz");
        width = 800;
        height = 600;
        committed = true;
      }
    }
  } else {
    /* Native mode */
    if (preferred) {
      wlr_output_state_set_mode(&state, preferred);
      width = preferred->width;
      height = preferred->height;
    } else {
      wlr_output_state_set_custom_mode(&state, width, height, 60000);
    }
    if (wlr_output_commit_state(wlr_output, &state)) {
      committed = true;
    }
  }
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

  /* Set backend-specific window title before first commit to help host mapping
   */
  if (wlr_output_is_wl(wlr_output)) {
    wlr_wl_output_set_title(wlr_output, "VitusOS Ares");
  } else if (wlr_output_is_x11(wlr_output)) {
    wlr_x11_output_set_title(wlr_output, "VitusOS Ares");
  }

  /* CRITICAL: Force an initial frame to host to map the window */
  wlr_log(WLR_INFO, "  Forcing initial scene commit to map window...");
  if (wlr_scene_output_commit(output->scene_output, NULL)) {
    wlr_log(WLR_INFO, "  Initial frame delivered successfully");
  } else {
    wlr_log(WLR_ERROR, "  Failed to deliver initial frame");
  }

  /* Schedule the next frame */
  wlr_output_schedule_frame(wlr_output);
}
