/**
 * OSFWallpaper.h - Desktop Wallpaper
 *
 * Loads and renders the desktop wallpaper behind windows.
 * Supports JPG, PNG via stb_image.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <string>


namespace opensef {

// Forward declarations
class OSFCompositor;

// Scaling modes
enum class WallpaperScaleMode {
  Fill,    // Scale to fill screen (may crop)
  Fit,     // Scale to fit (may letterbox)
  Stretch, // Stretch to exact size
  Center,  // Center at original size
  Tile     // Tile the image
};

/**
 * OSFWallpaper - Desktop wallpaper manager
 */
class OSFWallpaper {
public:
  OSFWallpaper(OSFCompositor *compositor);
  ~OSFWallpaper();

  // Load wallpaper from file
  bool load(const std::string &path);

  // Render wallpaper to buffer
  void render(uint32_t *buffer, int screenW, int screenH);

  // Settings
  void setScaleMode(WallpaperScaleMode mode) { scaleMode_ = mode; }
  WallpaperScaleMode scaleMode() const { return scaleMode_; }

  // Is wallpaper loaded?
  bool isLoaded() const { return pixels_ != nullptr; }

  // Image dimensions
  int width() const { return width_; }
  int height() const { return height_; }

private:
  OSFCompositor *compositor_;

  // Image data
  uint32_t *pixels_ = nullptr;
  int width_ = 0;
  int height_ = 0;

  // Scaled cache
  uint32_t *scaledPixels_ = nullptr;
  int scaledWidth_ = 0;
  int scaledHeight_ = 0;

  WallpaperScaleMode scaleMode_ = WallpaperScaleMode::Fill;

  // Helpers
  void freePixels();
  void createScaledCache(int targetW, int targetH);
  uint32_t sampleBilinear(float x, float y);
};

} // namespace opensef
