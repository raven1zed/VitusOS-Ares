/**
 * OSFSceneWallpaper.h - Scene-based Wallpaper
 *
 * Renders wallpaper as a wlr_scene_rect (solid color) or
 * wlr_scene_buffer (image) in the background layer.
 *
 * Now supports actual image loading with stb_image!
 */

#pragma once

extern "C" {
#include <wlr/types/wlr_buffer.h>
#include <wlr/types/wlr_scene.h>
}

#include <cstdint>
#include <string>
#include <vector>

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

  // Load image wallpaper
  bool loadImage(const std::string &path);

  // Load default Mars wallpaper
  void loadDefault();

  // Update size when output changes
  void resize(int width, int height);

private:
  void applyImageToScene();

  OSFCompositor *compositor_;
  OSFDesktopLayers *layers_;

  // Scene nodes
  wlr_scene_rect *colorRect_ = nullptr;
  wlr_scene_buffer *imageNode_ = nullptr;
  wlr_buffer *imageBuffer_ = nullptr;

  // Screen size
  int width_ = 0;
  int height_ = 0;
  uint32_t color_ = 0xB5651D; // Default: Mars desert sand

  // Image data
  std::vector<uint8_t> imagePixels_;
  int imageWidth_ = 0;
  int imageHeight_ = 0;
};

} // namespace opensef
