#include "OSFResourceCache.h"
#include <map>
#include <mutex>

namespace OpenSEF {

struct OSFResourceCache::Impl {
  std::map<std::string, cairo_surface_t *> iconCache;
  std::map<std::string, cairo_surface_t *> imageCache;
  std::mutex mutex;
};

OSFResourceCache::OSFResourceCache() : impl_(std::make_unique<Impl>()) {}

OSFResourceCache::~OSFResourceCache() { clearAllCaches(); }

cairo_surface_t *OSFResourceCache::getIcon(const std::string &name, int size) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  std::string key = name + "_" + std::to_string(size);

  auto it = impl_->iconCache.find(key);
  if (it != impl_->iconCache.end()) {
    return it->second;
  }

  // Would load icon from theme here
  // For now, return nullptr
  return nullptr;
}

void OSFResourceCache::preloadIcon(const std::string &name) {
  // Would preload icon in background
  (void)name;
}

void OSFResourceCache::clearIconCache() {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  for (auto &pair : impl_->iconCache) {
    if (pair.second) {
      cairo_surface_destroy(pair.second);
    }
  }
  impl_->iconCache.clear();
}

cairo_surface_t *OSFResourceCache::getImage(const std::string &path) {
  std::lock_guard<std::mutex> lock(impl_->mutex);

  auto it = impl_->imageCache.find(path);
  if (it != impl_->imageCache.end()) {
    return it->second;
  }

  // Would load image from file here
  // For now, return nullptr
  return nullptr;
}

void OSFResourceCache::clearImageCache() {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  for (auto &pair : impl_->imageCache) {
    if (pair.second) {
      cairo_surface_destroy(pair.second);
    }
  }
  impl_->imageCache.clear();
}

void OSFResourceCache::clearAllCaches() {
  clearIconCache();
  clearImageCache();
}

size_t OSFResourceCache::cacheSize() {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  return impl_->iconCache.size() + impl_->imageCache.size();
}

} // namespace OpenSEF
