/**
 * OSFDesktopLayers.h - Desktop Scene Layer Management
 *
 * Manages the z-ordering of desktop elements using wlr_scene:
 *
 * Layer Order (bottom to top):
 * 1. Background - Wallpaper
 * 2. Bottom - Desktop widgets (future)
 * 3. Windows - Client windows with decorations
 * 4. Top - Dock, panels
 * 5. Overlay - Menu bar, notifications
 * 6. Fullscreen - Fullscreen windows
 * 7. Lock - Lock screen (above everything)
 *
 * Based on wlr-layer-shell protocol and Sway/Wayfire architecture.
 */

#pragma once

extern "C" {
#include <wlr/types/wlr_scene.h>
}

#include <cstdint>

namespace opensef {

// Forward declarations
class OSFCompositor;

/**
 * Desktop layer indices
 */
enum class DesktopLayer {
  Background = 0, // Wallpaper
  Bottom,         // Desktop widgets
  Windows,        // Normal windows
  Top,            // Dock, panels
  Overlay,        // Menu bar, popups
  Fullscreen,     // Fullscreen windows
  Lock,           // Lock screen

  Count
};

/**
 * OSFDesktopLayers - Manages scene graph layers
 */
class OSFDesktopLayers {
public:
  OSFDesktopLayers(OSFCompositor *compositor, wlr_scene *scene);

  // Get layer tree for adding content
  wlr_scene_tree *layer(DesktopLayer layer);

  // Create a solid color rect in a layer
  wlr_scene_rect *createRect(DesktopLayer layer, int width, int height,
                             float color[4]);

  // Create a buffer in a layer (for custom rendering)
  wlr_scene_buffer *createBuffer(DesktopLayer layer);

private:
  OSFCompositor *compositor_;
  wlr_scene *scene_;

  // Layer trees (owned by scene, not us)
  wlr_scene_tree *layers_[static_cast<int>(DesktopLayer::Count)];
};

} // namespace opensef
