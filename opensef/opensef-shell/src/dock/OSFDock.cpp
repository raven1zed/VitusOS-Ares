#include "OSFDock.h"
#include "OSFAresTheme.h"
#include <iostream>

namespace opensef {

OSFDock::OSFDock() {
  surface_ = std::make_unique<OSFSurface>("osf-dock");

  // Bottom Center Dock
  surface_->setLayer(
      Layer::Bottom); // Should be above wallpaper but below windows? Actually
                      // Layer::Top usually for docks.
  // Let's use Top so it floats above windows (standard Mac behavior) or Bottom
  // if we want windows to cover it. Mac dock is always visible -> Top or
  // Overlay. Let's stick to Top.
  surface_->setLayer(Layer::Top);

  // Anchor only to bottom to allow centering
  surface_->setAnchor(Anchor::Bottom);

  // Fixed width for now (V1), will implement dynamic sizing later
  int dockWidth = 500;
  surface_->setSize(dockWidth, AresTheme::DockHeight);

  // Margin from bottom
  surface_->setMargin(0, 0, AresTheme::DockPadding, 0);

  // Exclusive zone so windows don't maximize over it
  surface_->setExclusiveZone(AresTheme::DockHeight + AresTheme::DockPadding);

  initWidgets();

  surface_->onDraw([this](cairo_t *cr, int w, int h) { this->draw(cr, w, h); });
}

void OSFDock::initWidgets() {
  // TODO: Add actual interactive buttons
  // For V1 we just draw them in draw()
}

void OSFDock::run() {
  if (surface_->connect()) {
    surface_->run();
  } else {
    std::cerr << "Failed to connect OSFDock to display." << std::endl;
  }
}

void OSFDock::draw(cairo_t *cr, int width, int height) {
  // 1. Clear transparent
  cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
  cairo_paint(cr);

  // 2. Dock Background (Rounded Rect)
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  AresTheme::setCairoColor(cr, AresTheme::DockBackground);
  AresTheme::roundedRect(cr, 0, 0, width, height, AresTheme::DockCornerRadius);
  cairo_fill(cr);

  // 3. Draw Icons (Placeholders)
  cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

  int iconSize = 48;
  int iconCount = 5;
  int spacing = AresTheme::DockItemSpacing;

  double startX =
      (width - (iconCount * iconSize + (iconCount - 1) * spacing)) / 2.0;
  double y = (height - iconSize) / 2.0;

  // Icon 1: Finder/Filer (Blue)
  AresTheme::setCairoColor(cr, 0xFF4A9FD4);
  AresTheme::roundedRect(cr, startX, y, iconSize, iconSize, 12);
  cairo_fill(cr);

  // Icon 2: Browser (Orange)
  startX += iconSize + spacing;
  AresTheme::setCairoColor(cr, AresTheme::MarsOrange);
  AresTheme::roundedRect(cr, startX, y, iconSize, iconSize, 12);
  cairo_fill(cr);

  // Icon 3: Terminal (Black/Dark)
  startX += iconSize + spacing;
  AresTheme::setCairoColor(cr, 0xFF2D2D2D);
  AresTheme::roundedRect(cr, startX, y, iconSize, iconSize, 12);
  cairo_fill(cr);

  // Icon 4: Settings (Grey)
  startX += iconSize + spacing;
  AresTheme::setCairoColor(cr, 0xFF888888);
  AresTheme::roundedRect(cr, startX, y, iconSize, iconSize, 12);
  cairo_fill(cr);

  // Icon 5: Trash (Red)
  startX += iconSize + spacing;
  AresTheme::setCairoColor(cr, AresTheme::MarsRed);
  AresTheme::roundedRect(cr, startX, y, iconSize, iconSize, 12);
  cairo_fill(cr);
}

} // namespace opensef
