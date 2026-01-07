#pragma once

#include <cairo.h>
#include <memory>
#include <string>

namespace OpenSEF {

/**
 * OSFResourceCache - Shared Resource Management
 *
 * Single cache for icons, fonts, images.
 * All components share resources instead of loading duplicates.
 *
 * Usage:
 *   auto* cache = desktop->resourceCache();
 *   auto* icon = cache->getIcon("application-menu", 24);
 *   auto* font = cache->getFont("Sans", 12);
 */
class OSFResourceCache {
public:
  OSFResourceCache();
  ~OSFResourceCache();

  // Icon cache
  cairo_surface_t *getIcon(const std::string &name, int size);
  void preloadIcon(const std::string &name);
  void clearIconCache();

  // Image cache
  cairo_surface_t *getImage(const std::string &path);
  void clearImageCache();

  // Cache management
  void clearAllCaches();
  size_t cacheSize();

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace OpenSEF
