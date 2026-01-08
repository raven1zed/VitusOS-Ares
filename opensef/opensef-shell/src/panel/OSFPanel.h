#ifndef OSF_PANEL_H
#define OSF_PANEL_H

#include "OSFDesktop.h"
#include "OSFEventBus.h"
#include "OSFSurface.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace opensef {

// Menu item with action
struct MenuItem {
  std::string label;
  std::function<void()> action;
  bool isSeparator = false;
};

// Dropdown menu definition
struct MenuDef {
  std::string title;
  std::vector<MenuItem> items;
  double x = 0;
  double width = 0;
};

class OSFPanel {
public:
  OSFPanel();
  ~OSFPanel() = default;

  void run();
  bool connect() { return surface_ && surface_->connect(); }
  OSFSurface *surface() const { return surface_.get(); }

private:
  std::unique_ptr<OSFSurface> surface_;

  // Menu definitions
  std::vector<MenuDef> menus_;
  int openMenuIndex_ = -1; // -1 = no menu open
  int hoveredMenuIndex_ = -1;
  int hoveredItemIndex_ = -1;

  // Layout cache
  double multitaskButtonX_ = 8;
  double multitaskButtonSize_ = 80; // Updated from 20px to 80px
  double menuStartX_ = 100;         // Account for 8 + 80 + padding
  double clockEndX_ = 0;
  std::string currentClockText_;
  cairo_rectangle_int_t multitaskButtonRect_ = {8, 4, 80, 20}; // Hit rect

  // Rendering
  void initMenus();
  void draw(cairo_t *cr, int width, int height);
  void drawMultitaskButton(cairo_t *cr);
  void drawMenuTitles(cairo_t *cr, int width);
  void drawClock(cairo_t *cr, int width, int height);
  void drawSystemTray(cairo_t *cr, int width, int height); // New
  void drawDropdown(cairo_t *cr, int menuIndex);
  void updateClock();

  // Input
  void handlePointerMove(int x, int y);
  void handlePointerDown(int x, int y, uint32_t button);
  void handlePointerUp(int x, int y, uint32_t button);
  void clearHover();

  // Hit testing
  int hitTestMenuTitle(int x, int y);
  int hitTestDropdownItem(int x, int y);

  // Framework Integration
  OpenSEF::OSFDesktop *desktop_ = nullptr;
  std::string activeWindowTitle_;
  std::string activeAppId_;

  void subscribeToEvents();
  void onWindowFocused(const OpenSEF::OSFEvent &event);
  void onWindowTitleChanged(const OpenSEF::OSFEvent &event);
};

} // namespace opensef

#endif // OSF_PANEL_H
