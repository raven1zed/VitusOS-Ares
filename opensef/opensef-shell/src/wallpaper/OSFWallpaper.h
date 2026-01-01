#ifndef OSF_WALLPAPER_H
#define OSF_WALLPAPER_H

#include "OSFSurface.h"
#include <memory>
#include <string>

namespace opensef {

class OSFWallpaper {
public:
  OSFWallpaper(const std::string& imagePath);
  ~OSFWallpaper();
  void run();

private:
  void draw(cairo_t* cr, int width, int height);

  std::unique_ptr<OSFSurface> surface_;
  std::string imagePath_;
  cairo_surface_t* cachedImage_ = nullptr;
};

} // namespace opensef

#endif // OSF_WALLPAPER_H
