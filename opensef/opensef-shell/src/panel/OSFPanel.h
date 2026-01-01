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

private:
  std::unique_ptr<OSFSurface> surface_;

  // Widgets (using the widget library)
  std::shared_ptr<OSFButton> appNameButton_;
  std::shared_ptr<OSFLabel> menuFile_;
  std::shared_ptr<OSFLabel> menuSettings_;
  std::shared_ptr<OSFLabel> menuHelp_;
  std::shared_ptr<OSFLabel> clockLabel_;

  // List of all widgets for hit testing
  std::vector<std::shared_ptr<OSFView>> widgets_;

  // Rendering
  void initWidgets();
  void draw(cairo_t *cr, int width, int height);
  void updateClock();

  // Input dispatch
  void handlePointerEvent(const PointerEvent& event);
};

} // namespace opensef

#endif // OSF_PANEL_H
