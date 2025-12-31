/**
 * OSFOutput.cpp - Monitor/Output management
 */

#include "OSFOutput.h"
#include "OSFCompositor.h"

#include <iostream>

namespace opensef {

#define WL_CONTAINER_OF(ptr, type, member)                                     \
  reinterpret_cast<type *>(reinterpret_cast<char *>(ptr) -                     \
                           offsetof(type, member))

OSFOutput::OSFOutput(OSFCompositor *compositor, wlr_output *output)
    : compositor_(compositor), output_(output) {

  // Configure output with preferred mode
  wlr_output_init_render(output, compositor->allocator(),
                         compositor->renderer());

  wlr_output_state state;
  wlr_output_state_init(&state);
  wlr_output_state_set_enabled(&state, true);

  // Use preferred mode if available
  wlr_output_mode *mode = wlr_output_preferred_mode(output);
  if (mode) {
    wlr_output_state_set_mode(&state, mode);
    std::cout << "[openSEF] Output mode: " << mode->width << "x" << mode->height
              << "@" << (mode->refresh / 1000.0f) << "Hz" << std::endl;
  }

  wlr_output_commit_state(output, &state);
  wlr_output_state_finish(&state);

  // Add to output layout
  wlr_output_layout_add_auto(compositor->outputLayout(), output);

  // Create scene output
  sceneOutput_ = wlr_scene_output_create(compositor->scene(), output);

  // Set up event listeners
  frame_.notify = handleFrame;
  wl_signal_add(&output->events.frame, &frame_);

  requestState_.notify = handleRequestState;
  wl_signal_add(&output->events.request_state, &requestState_);

  destroy_.notify = handleDestroy;
  wl_signal_add(&output->events.destroy, &destroy_);
}

OSFOutput::~OSFOutput() {
  wl_list_remove(&frame_.link);
  wl_list_remove(&requestState_.link);
  wl_list_remove(&destroy_.link);
}

void OSFOutput::handleFrame(wl_listener *listener, void *data) {
  OSFOutput *self = WL_CONTAINER_OF(listener, OSFOutput, frame_);
  (void)data;

  wlr_scene_output *sceneOutput = self->sceneOutput_;
  wlr_scene_output_commit(sceneOutput, nullptr);

  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  wlr_scene_output_send_frame_done(sceneOutput, &now);
}

void OSFOutput::handleRequestState(wl_listener *listener, void *data) {
  OSFOutput *self = WL_CONTAINER_OF(listener, OSFOutput, requestState_);
  wlr_output_event_request_state *event =
      static_cast<wlr_output_event_request_state *>(data);
  wlr_output_commit_state(self->output_, event->state);
}

void OSFOutput::handleDestroy(wl_listener *listener, void *data) {
  (void)listener;
  (void)data;
  // Output will be cleaned up by compositor
}

} // namespace opensef
