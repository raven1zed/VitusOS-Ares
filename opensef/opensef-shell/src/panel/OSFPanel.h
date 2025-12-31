#ifndef OSF_PANEL_H
#define OSF_PANEL_H

#include "OSFSurface.h"
#include <memory>
#include <opensef/OpenSEFAppKit.h>


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

  // Rendering
  void initWidgets();
  void draw(cairo_t *cr, int width, int height);
  void updateClock();
};

} // namespace opensef

#endif // OSF_PANEL_H
