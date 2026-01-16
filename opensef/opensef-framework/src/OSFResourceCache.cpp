#include <opensef/OSFResourceCache.h>
#include <map>
#include <mutex>
#include <vector>

namespace OpenSEF {

struct OSFResourceCache::Impl {
  std::map<std::string, std::unique_ptr<OSFResourceCache::Surface>> iconCache;
  std::map<std::string, std::unique_ptr<OSFResourceCache::Surface>> imageCache;
  std::mutex mutex;
};

OSFResourceCache::OSFResourceCache() : impl_(std::make_unique<Impl>()) {}

OSFResourceCache::~OSFResourceCache() { clearAllCaches(); }

OSFResourceCache::Surface *OSFResourceCache::getIcon(const std::string &name,
                                                     int size) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  std::string key = name + "_" + std::to_string(size);

  auto it = impl_->iconCache.find(key);
  if (it != impl_->iconCache.end()) {
    return it->second.get();
  }

  return nullptr;
}

void OSFResourceCache::preloadIcon(const std::string &name) { (void)name; }

void OSFResourceCache::clearIconCache() {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  impl_->iconCache.clear();
}

OSFResourceCache::Surface *OSFResourceCache::getImage(const std::string &path) {
  std::lock_guard<std::mutex> lock(impl_->mutex);

  auto it = impl_->imageCache.find(path);
  if (it != impl_->imageCache.end()) {
    return it->second.get();
  }

  return nullptr;
}

void OSFResourceCache::clearImageCache() {
  std::lock_guard<std::mutex> lock(impl_->mutex);
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
