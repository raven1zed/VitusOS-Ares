/**
 * OSFSceneWallpaper.cpp - Scene-based Wallpaper Implementation
 *
 * Now with actual image loading using stb_image!
 */

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "OSFAresTheme.h"
#include "OSFCompositor.h"
#include "OSFDesktopLayers.h"
#include "OSFSceneWallpaper.h"


#include <cstring>
#include <iostream>
#include <vector>

namespace opensef {

// Default wallpaper path
static const char *kDefaultWallpaperPaths[] = {
    "../resources/wallpapers/mars_default.jpg",
    "resources/wallpapers/mars_default.jpg",
    "/run/current-system/sw/share/opensef/wallpapers/mars_default.jpg",
    "/usr/share/opensef/wallpapers/mars_default.jpg", nullptr};

OSFSceneWallpaper::OSFSceneWallpaper(OSFCompositor *compositor,
                                     OSFDesktopLayers *layers)
    : compositor_(compositor), layers_(layers) {}

OSFSceneWallpaper::~OSFSceneWallpaper() {
  // Scene nodes are owned by the scene tree, will be cleaned up automatically
  if (imageBuffer_) {
    wlr_buffer_drop(imageBuffer_);
    imageBuffer_ = nullptr;
  }
}

void OSFSceneWallpaper::setColor(uint32_t color) {
  color_ = color;

  // Convert ARGB to float RGBA
  float rgba[4];
  AresTheme::hexToRGBA(color, rgba);

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
  std::string actualPath = path;

  // If no path specified, try default paths
  if (actualPath.empty()) {
    for (const char **p = kDefaultWallpaperPaths; *p; ++p) {
      // Check if file exists
      FILE *f = fopen(*p, "rb");
      if (f) {
        fclose(f);
        actualPath = *p;
        break;
      }
    }
  }

  if (actualPath.empty()) {
    std::cerr << "[openSEF] No wallpaper file found, using fallback color"
              << std::endl;
    setColor(0x8B4513); // Mars-like brown
    return false;
  }

  // Load image with stb_image
  int imgWidth, imgHeight, channels;
  stbi_uc *pixels = stbi_load(actualPath.c_str(), &imgWidth, &imgHeight,
                              &channels, 4); // Force RGBA

  if (!pixels) {
    std::cerr << "[openSEF] Failed to load wallpaper: " << actualPath
              << std::endl;
    std::cerr << "[openSEF] Reason: " << stbi_failure_reason() << std::endl;
    setColor(0x8B4513); // Fallback
    return false;
  }

  std::cout << "[openSEF] Loaded wallpaper: " << actualPath << " (" << imgWidth
            << "x" << imgHeight << ")" << std::endl;

  // Store image dimensions
  imageWidth_ = imgWidth;
  imageHeight_ = imgHeight;

  // Convert RGBA to ARGB (Wayland format is ARGB)
  for (int i = 0; i < imgWidth * imgHeight; i++) {
    uint8_t r = pixels[i * 4 + 0];
    uint8_t g = pixels[i * 4 + 1];
    uint8_t b = pixels[i * 4 + 2];
    uint8_t a = pixels[i * 4 + 3];

    // Reorder to ARGB (or BGRA for little-endian)
    pixels[i * 4 + 0] = b;
    pixels[i * 4 + 1] = g;
    pixels[i * 4 + 2] = r;
    pixels[i * 4 + 3] = a;
  }

  // Create wlr_buffer from pixels
  // Note: This requires proper buffer implementation with wlr_buffer_impl
  // For now, we'll use wlr_scene_buffer_create with a data buffer

  // Create scene buffer node
  if (imageNode_) {
    wlr_scene_node_destroy(&imageNode_->node);
    imageNode_ = nullptr;
  }

  // Store pixels for later use (scene buffer creation happens in resize)
  imagePixels_.assign(pixels, pixels + (imgWidth * imgHeight * 4));

  stbi_image_free(pixels);

  // Apply to current screen size if already set
  if (width_ > 0 && height_ > 0) {
    applyImageToScene();
  }

  return true;
}

void OSFSceneWallpaper::applyImageToScene() {
  if (imagePixels_.empty() || width_ <= 0 || height_ <= 0) {
    return;
  }

  // For wlroots scene API, we need to create a proper buffer
  // This is a simplified version - real implementation needs wlr_buffer_impl

  // For now, hide the color rect and show a message
  // The proper implementation requires creating a wlr_buffer with the image
  // data

  // Hide color fallback if image loaded
  if (colorRect_) {
    wlr_scene_node_set_enabled(&colorRect_->node, false);
  }

  // Create a simple indicator that image loaded
  // Real implementation: wlr_scene_buffer_create() with proper buffer
  std::cout << "[openSEF] Wallpaper image ready (" << imageWidth_ << "x"
            << imageHeight_ << " -> " << width_ << "x" << height_ << ")"
            << std::endl;

  // For actual rendering with wlroots, you would need:
  // 1. Create a wlr_buffer_impl for raw pixel data
  // 2. Create wlr_buffer with those pixels
  // 3. Create wlr_scene_buffer from the wlr_buffer
  // This requires deeper wlroots integration

  // Temporary: just show the color that matches the wallpaper tone
  // until proper buffer implementation
  setColor(0xB5651D); // Desert sand color matching Mars wallpaper
}

void OSFSceneWallpaper::resize(int width, int height) {
  if (width_ == width && height_ == height) {
    return;
  }

  width_ = width;
  height_ = height;

  if (!imagePixels_.empty()) {
    applyImageToScene();
  } else if (colorRect_) {
    wlr_scene_rect_set_size(colorRect_, width, height);
  } else {
    // Create rect with current color
    setColor(color_);
  }
}

void OSFSceneWallpaper::loadDefault() {
  loadImage(""); // Will try default paths
}

} // namespace opensef
