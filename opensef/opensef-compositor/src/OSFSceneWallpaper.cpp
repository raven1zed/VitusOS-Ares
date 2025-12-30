/**
 * OSFSceneWallpaper.cpp - Scene-based Wallpaper Implementation
 */

#include "OSFSceneWallpaper.h"
#include "OSFCompositor.h"
#include "OSFDecorations.h"
#include "OSFDesktopLayers.h"


#include <iostream>

namespace opensef {

using namespace AresColors;

OSFSceneWallpaper::OSFSceneWallpaper(OSFCompositor *compositor,
                                     OSFDesktopLayers *layers)
    : compositor_(compositor), layers_(layers) {}

OSFSceneWallpaper::~OSFSceneWallpaper() {
  // Scene nodes are owned by the scene tree, will be cleaned up automatically
}

void OSFSceneWallpaper::setColor(uint32_t color) {
  color_ = color;

  // Convert ARGB to float RGBA
  float rgba[4];
  rgba[0] = ((color >> 16) & 0xFF) / 255.0f; // R
  rgba[1] = ((color >> 8) & 0xFF) / 255.0f;  // G
  rgba[2] = (color & 0xFF) / 255.0f;         // B
  rgba[3] = 1.0f;                            // A

  if (colorRect_) {
    wlr_scene_rect_set_color(colorRect_, rgba);
  } else if (width_ > 0 && height_ > 0) {
    // Create new rect
    colorRect_ =
        layers_->createRect(DesktopLayer::Background, width_, height_, rgba);
    if (colorRect_) {
      wlr_scene_node_set_position(&colorRect_->node, 0, 0);
    }
  }

  std::cout << "[openSEF] Wallpaper color set: #" << std::hex << color
            << std::dec << std::endl;
}

bool OSFSceneWallpaper::loadImage(const std::string &path) {
  // TODO: Implement image loading with wlr_buffer
  // For now, use a warm color matching the wallpaper tone
  (void)path;

  // Mars-like warm color
  setColor(0x8B4513); // Saddle brown (Mars-ish)

  std::cout << "[openSEF] Image wallpaper not yet implemented, using color"
            << std::endl;

  return true;
}

void OSFSceneWallpaper::resize(int width, int height) {
  if (width_ == width && height_ == height) {
    return;
  }

  width_ = width;
  height_ = height;

  if (colorRect_) {
    wlr_scene_rect_set_size(colorRect_, width, height);
  } else {
    // Create rect with current color
    setColor(color_);
  }
}

} // namespace opensef
