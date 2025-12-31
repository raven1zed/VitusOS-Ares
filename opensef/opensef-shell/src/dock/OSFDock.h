#ifndef OSF_DOCK_H
#define OSF_DOCK_H

#include <memory>
#include <opensef/OpenSEFAppKit.h>
#include <vector>


namespace opensef {

class OSFDock {
public:
  OSFDock();
  void run();

private:
  void draw(cairo_t *cr, int width, int height);
  void initWidgets();

  std::unique_ptr<OSFSurface> surface_;

  // Dock items
  std::vector<std::shared_ptr<OSFButton>> dockItems_;
};

} // namespace opensef

#endif // OSF_DOCK_H
