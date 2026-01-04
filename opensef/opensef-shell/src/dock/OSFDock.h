#ifndef OSF_DOCK_H
#define OSF_DOCK_H

#include "OSFSurface.h"
#include <librsvg/rsvg.h>
#include <memory>
#include <opensef/OpenSEFAppKit.h>
#include <string>
#include <vector>


namespace opensef {

struct DockItem {
  std::string name;
  std::string iconPath;
  std::string command;
  std::shared_ptr<RsvgHandle> svgHandle;
  double x = 0, y = 0, size = 48; // Hit detection logic
};

class OSFDock {
public:
  OSFDock();
  ~OSFDock();
  void run();
  bool connect() { return surface_->connect(); }
  OSFSurface *surface() const { return surface_.get(); }

private:
  void draw(cairo_t *cr, int width, int height);
  void initWidgets();
  void onMouseUp(int x, int y, uint32_t button);
  void onMouseMove(int x, int y);
  void clearHover();

  std::unique_ptr<OSFSurface> surface_;
  std::vector<DockItem> items_;
  int hoveredIndex_ = -1;

  // Dock items
  // std::vector<std::shared_ptr<OSFButton>> dockItems_; // Using custom struct
  // for now
};

} // namespace opensef

#endif // OSF_DOCK_H
