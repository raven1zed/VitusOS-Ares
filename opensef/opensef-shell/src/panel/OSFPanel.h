#ifndef OSF_PANEL_H
#define OSF_PANEL_H

#include "OSFSurface.h"
#include <memory>

namespace opensef {

class OSFPanel {
public:
  OSFPanel();
  ~OSFPanel() = default;

  void run();

private:
  std::unique_ptr<OSFSurface> surface_;

  // Rendering
  void draw(cairo_t *cr, int width, int height);
  void drawStartButton(cairo_t *cr, double x, double y);
  void drawClock(cairo_t *cr, int width, int height);
};

} // namespace opensef

#endif // OSF_PANEL_H
