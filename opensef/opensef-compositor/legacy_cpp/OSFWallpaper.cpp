/**
 * OSFWallpaper.cpp - Desktop Wallpaper Implementation
 */

#include "OSFWallpaper.h"
#include "OSFCompositor.h"

#include <cmath>
#include <cstring>
#include <iostream>


// Define STB_IMAGE_IMPLEMENTATION in exactly one source file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace opensef {

OSFWallpaper::OSFWallpaper(OSFCompositor *compositor)
    : compositor_(compositor) {}

OSFWallpaper::~OSFWallpaper() { freePixels(); }

bool OSFWallpaper::load(const std::string &path) {
  freePixels();

  // Load image using stb_image
  int channels;
  stbi_uc *data = stbi_load(path.c_str(), &width_, &height_, &channels, 4);

  if (!data) {
    std::cerr << "[openSEF] Failed to load wallpaper: " << path << std::endl;
    std::cerr << "[openSEF] Reason: " << stbi_failure_reason() << std::endl;
    return false;
  }

  // Convert RGBA to ARGB (stb gives us RGBA, we need ARGB for compositor)
  pixels_ = new uint32_t[width_ * height_];
  for (int i = 0; i < width_ * height_; i++) {
    uint8_t r = data[i * 4 + 0];
    uint8_t g = data[i * 4 + 1];
    uint8_t b = data[i * 4 + 2];
    uint8_t a = data[i * 4 + 3];
    pixels_[i] = (a << 24) | (r << 16) | (g << 8) | b;
  }

  stbi_image_free(data);

  std::cout << "[openSEF] Loaded wallpaper: " << path << " (" << width_ << "x"
            << height_ << ")" << std::endl;

  return true;
}

void OSFWallpaper::render(uint32_t *buffer, int screenW, int screenH) {
  if (!pixels_) {
    // No wallpaper loaded - fill with dark background
    uint32_t bgColor = 0xFF1A1A2E; // Space Charcoal
    for (int i = 0; i < screenW * screenH; i++) {
      buffer[i] = bgColor;
    }
    return;
  }

  // Check if we need to recreate scaled cache
  if (scaleMode_ == WallpaperScaleMode::Fill ||
      scaleMode_ == WallpaperScaleMode::Fit ||
      scaleMode_ == WallpaperScaleMode::Stretch) {
    if (scaledWidth_ != screenW || scaledHeight_ != screenH) {
      createScaledCache(screenW, screenH);
    }
  }

  switch (scaleMode_) {
  case WallpaperScaleMode::Fill:
  case WallpaperScaleMode::Fit:
  case WallpaperScaleMode::Stretch:
    // Use scaled cache
    if (scaledPixels_) {
      std::memcpy(buffer, scaledPixels_, screenW * screenH * sizeof(uint32_t));
    }
    break;

  case WallpaperScaleMode::Center: {
    // Center the wallpaper
    int offsetX = (screenW - width_) / 2;
    int offsetY = (screenH - height_) / 2;

    // Fill background first
    uint32_t bgColor = 0xFF1A1A2E;
    for (int i = 0; i < screenW * screenH; i++) {
      buffer[i] = bgColor;
    }

    // Copy image to center
    for (int y = 0; y < height_ && y + offsetY < screenH; y++) {
      if (y + offsetY < 0)
        continue;
      for (int x = 0; x < width_ && x + offsetX < screenW; x++) {
        if (x + offsetX < 0)
          continue;
        buffer[(y + offsetY) * screenW + (x + offsetX)] =
            pixels_[y * width_ + x];
      }
    }
    break;
  }

  case WallpaperScaleMode::Tile:
    // Tile the image
    for (int y = 0; y < screenH; y++) {
      for (int x = 0; x < screenW; x++) {
        int srcX = x % width_;
        int srcY = y % height_;
        buffer[y * screenW + x] = pixels_[srcY * width_ + srcX];
      }
    }
    break;
  }
}

void OSFWallpaper::freePixels() {
  if (pixels_) {
    delete[] pixels_;
    pixels_ = nullptr;
  }
  if (scaledPixels_) {
    delete[] scaledPixels_;
    scaledPixels_ = nullptr;
  }
  width_ = 0;
  height_ = 0;
  scaledWidth_ = 0;
  scaledHeight_ = 0;
}

void OSFWallpaper::createScaledCache(int targetW, int targetH) {
  if (scaledPixels_) {
    delete[] scaledPixels_;
  }

  scaledWidth_ = targetW;
  scaledHeight_ = targetH;
  scaledPixels_ = new uint32_t[targetW * targetH];

  float scaleX, scaleY;
  int offsetX = 0, offsetY = 0;

  if (scaleMode_ == WallpaperScaleMode::Fill) {
    // Scale to fill (crop if needed)
    float aspectSrc = static_cast<float>(width_) / height_;
    float aspectDst = static_cast<float>(targetW) / targetH;

    if (aspectSrc > aspectDst) {
      // Image is wider - scale by height, crop width
      scaleY = static_cast<float>(height_) / targetH;
      scaleX = scaleY;
      offsetX = static_cast<int>((width_ - targetW * scaleX) / 2);
    } else {
      // Image is taller - scale by width, crop height
      scaleX = static_cast<float>(width_) / targetW;
      scaleY = scaleX;
      offsetY = static_cast<int>((height_ - targetH * scaleY) / 2);
    }
  } else if (scaleMode_ == WallpaperScaleMode::Fit) {
    // Scale to fit (letterbox if needed)
    float aspectSrc = static_cast<float>(width_) / height_;
    float aspectDst = static_cast<float>(targetW) / targetH;

    // Fill with background first
    uint32_t bgColor = 0xFF1A1A2E;
    for (int i = 0; i < targetW * targetH; i++) {
      scaledPixels_[i] = bgColor;
    }

    if (aspectSrc > aspectDst) {
      scaleX = static_cast<float>(width_) / targetW;
      scaleY = scaleX;
    } else {
      scaleY = static_cast<float>(height_) / targetH;
      scaleX = scaleY;
    }
  } else {
    // Stretch
    scaleX = static_cast<float>(width_) / targetW;
    scaleY = static_cast<float>(height_) / targetH;
  }

  // Perform scaling with bilinear interpolation
  for (int y = 0; y < targetH; y++) {
    for (int x = 0; x < targetW; x++) {
      float srcX = x * scaleX + offsetX;
      float srcY = y * scaleY + offsetY;

      if (srcX >= 0 && srcX < width_ - 1 && srcY >= 0 && srcY < height_ - 1) {
        scaledPixels_[y * targetW + x] = sampleBilinear(srcX, srcY);
      }
    }
  }
}

uint32_t OSFWallpaper::sampleBilinear(float x, float y) {
  int x0 = static_cast<int>(x);
  int y0 = static_cast<int>(y);
  int x1 = x0 + 1;
  int y1 = y0 + 1;

  float fx = x - x0;
  float fy = y - y0;

  // Clamp to image bounds
  x1 = std::min(x1, width_ - 1);
  y1 = std::min(y1, height_ - 1);

  // Get four corner pixels
  uint32_t p00 = pixels_[y0 * width_ + x0];
  uint32_t p10 = pixels_[y0 * width_ + x1];
  uint32_t p01 = pixels_[y1 * width_ + x0];
  uint32_t p11 = pixels_[y1 * width_ + x1];

  // Interpolate each channel
  auto lerp = [](uint8_t a, uint8_t b, float t) -> uint8_t {
    return static_cast<uint8_t>(a * (1 - t) + b * t);
  };

  auto lerpPixel = [&](uint32_t a, uint32_t b, float t) -> uint32_t {
    uint8_t rA = lerp((a >> 16) & 0xFF, (b >> 16) & 0xFF, t);
    uint8_t rG = lerp((a >> 8) & 0xFF, (b >> 8) & 0xFF, t);
    uint8_t rB = lerp(a & 0xFF, b & 0xFF, t);
    return 0xFF000000 | (rA << 16) | (rG << 8) | rB;
  };

  uint32_t top = lerpPixel(p00, p10, fx);
  uint32_t bot = lerpPixel(p01, p11, fx);
  return lerpPixel(top, bot, fy);
}

} // namespace opensef
