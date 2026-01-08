#include "OSFWallpaper.h"
#include <cstring>
#include <iostream>


// Include stb_image from compositor includes or local copy
// Since we don't have easy access to compositor includes via CMake yet without
// modifying paths, and to keep it simple, we will support PNG via Cairo first.
// If the user wants JPG, we can add stb_image later or use librsvg for SVG.
// Wait, the requirement says "Load image".

// Let's try to include stb_image if possible.
// The include path for opensef-compositor is not globally exported.
// I will stick to cairo_image_surface_create_from_png for now.
// If it fails, I'll print an error.

namespace opensef {

OSFWallpaper::OSFWallpaper(const std::string &imagePath)
    : imagePath_(imagePath) {
  surface_ = std::make_unique<OSFSurface>("osf-wallpaper");

  // Configure for background layer
  surface_->setLayer(Layer::Background);

  // Anchor to all edges to fill screen
  surface_->setAnchor(Anchor::Top | Anchor::Bottom | Anchor::Left |
                      Anchor::Right);

  // Exclusive zone 0 because windows can cover it
  surface_->setExclusiveZone(0); // -1 often means ignore exclusive zone logic,
                                 // 0 means don't push others.

  // Load image cache
  if (!imagePath_.empty()) {
    cachedImage_ = cairo_image_surface_create_from_png(imagePath_.c_str());
    if (cairo_surface_status(cachedImage_) != CAIRO_STATUS_SUCCESS) {
      std::cerr << "Failed to load image: " << imagePath_
                << " (Only PNG is supported currently)" << std::endl;
      cairo_surface_destroy(cachedImage_);
      cachedImage_ = nullptr;
    }
  }

  // Bind callbacks
  surface_->onDraw([this](cairo_t *cr, int w, int h) { this->draw(cr, w, h); });
}

OSFWallpaper::~OSFWallpaper() {
  if (cachedImage_) {
    cairo_surface_destroy(cachedImage_);
  }
}

void OSFWallpaper::run() {
  if (surface_->connect()) {
    surface_->requestRedraw(); // Force initial render
    surface_->run();
  } else {
    std::cerr << "Failed to connect OSFWallpaper to display." << std::endl;
  }
}

void OSFWallpaper::draw(cairo_t *cr, int width, int height) {
  // Fill with default color first (in case image load fails)
  cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Dark gray
  cairo_paint(cr);

  if (!cachedImage_) {
    return;
  }

  int imgW = cairo_image_surface_get_width(cachedImage_);
  int imgH = cairo_image_surface_get_height(cachedImage_);

  double scaleX = (double)width / imgW;
  double scaleY = (double)height / imgH;

  // "Cover" style scaling (preserve aspect ratio, crop excess)
  double scale = std::max(scaleX, scaleY);

  double scaledW = imgW * scale;
  double scaledH = imgH * scale;

  double offsetX = (width - scaledW) / 2.0;
  double offsetY = (height - scaledH) / 2.0;

  cairo_save(cr);
  cairo_translate(cr, offsetX, offsetY);
  cairo_scale(cr, scale, scale);

  cairo_set_source_surface(cr, cachedImage_, 0, 0);
  cairo_paint(cr);

  cairo_restore(cr);
}

} // namespace opensef
