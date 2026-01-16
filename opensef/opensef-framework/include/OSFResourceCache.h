#pragma once

#include <memory>
#include <string>

namespace OpenSEF {

/**
 * OSFResourceCache - Shared Resource Management (Renderer Agnostic)
 */
class OSFResourceCache {
public:
  struct Surface {
    void *m_data = nullptr;
    int width = 0;
    int height = 0;
    int stride = 0;
  };

  OSFResourceCache();
  ~OSFResourceCache();

  // Icon cache
  Surface *getIcon(const std::string &name, int size);
  void preloadIcon(const std::string &name);
  void clearIconCache();

  // Image cache
  Surface *getImage(const std::string &path);
  void clearImageCache();

  // Cache management
  void clearAllCaches();
  size_t cacheSize();

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace OpenSEF
