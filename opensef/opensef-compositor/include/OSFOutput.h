/**
 * OSFOutput.h - Monitor/Output management
 */

#pragma once

extern "C" {
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_scene.h>
}

#include <wayland-server-core.h>

namespace opensef {

class OSFCompositor;

/**
 * OSFOutput - Represents a physical display
 */
class OSFOutput {
public:
  OSFOutput(OSFCompositor *compositor, wlr_output *output);
  ~OSFOutput();

  wlr_output *output() const { return output_; }

private:
  OSFCompositor *compositor_;
  wlr_output *output_;
  wlr_scene_output *sceneOutput_;

  wl_listener frame_;
  wl_listener requestState_;
  wl_listener destroy_;

  static void handleFrame(wl_listener *listener, void *data);
  static void handleRequestState(wl_listener *listener, void *data);
  static void handleDestroy(wl_listener *listener, void *data);
};

} // namespace opensef
