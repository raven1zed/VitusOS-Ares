/**
 * OSFSceneWallpaper.h - Scene-based Wallpaper
 *
 * Renders wallpaper as a wlr_scene_rect (solid color) or
 * wlr_scene_buffer (image) in the background layer.
 */

#pragma once

extern "C" {
#include <wlr/types/wlr_buffer.h>
#include <wlr/types/wlr_scene.h>

}

#include <cstdint>
#include <string>


namespace opensef {

// Forward declarations
class OSFCompositor;
class OSFDesktopLayers;

/**
 * OSFSceneWallpaper - Scene-graph based wallpaper
 */
class OSFSceneWallpaper {
public:
  OSFSceneWallpaper(OSFCompositor *compositor, OSFDesktopLayers *layers);
  ~OSFSceneWallpaper();

  // Set solid color wallpaper
  void setColor(uint32_t color);

  // Load image wallpaper (TODO: implement with wlr_buffer)
  bool loadImage(const std::string &path);

  // Update size when output changes
  void resize(int width, int height);

private:
  OSFCompositor *compositor_;
  OSFDesktopLayers *layers_;

  // Scene node for wallpaper
  wlr_scene_rect *colorRect_ = nullptr;

  int width_ = 0;
  int height_ = 0;
  uint32_t color_ = 0x1A1A2E; // Default: Space Charcoal
};

} // namespace opensef
