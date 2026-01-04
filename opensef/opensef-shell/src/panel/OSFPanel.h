#ifndef OSF_PANEL_H
#define OSF_PANEL_H

#include "OSFSurface.h"
#include <memory>
#include <opensef/OpenSEFAppKit.h>
#include <vector>

namespace opensef {

class OSFPanel {
public:
  OSFPanel();
  ~OSFPanel() = default;

  void run();
  bool connect() { return surface_ && surface_->connect(); }
  OSFSurface *surface() const { return surface_.get(); }

private:
  std::unique_ptr<OSFSurface> surface_;

  // Widgets (using the widget library)
  std::shared_ptr<OSFButton> appNameButton_;
  std::vector<std::shared_ptr<OSFButton>> menuButtons_;
  std::shared_ptr<OSFLabel> clockLabel_;

  // List of all widgets for hit testing
  std::vector<std::shared_ptr<OSFView>> widgets_;

  // Rendering
  void initWidgets();
  void draw(cairo_t *cr, int width, int height);
  void updateClock();

  // Input dispatch
  void handlePointerMove(int x, int y);
  void handlePointerDown(int x, int y, uint32_t button);
  void handlePointerUp(int x, int y, uint32_t button);
  void clearHover();
  std::shared_ptr<OSFButton> hitTestButton(int x, int y);

  std::shared_ptr<OSFButton> hoveredButton_;
  std::shared_ptr<OSFButton> pressedButton_;
};

} // namespace opensef

#endif // OSF_PANEL_H
