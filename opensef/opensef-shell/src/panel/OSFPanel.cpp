#include "OSFPanel.h"
#include "OSFAresTheme.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace opensef {

OSFPanel::OSFPanel() {
  // Create Wayland surface
  surface_ = std::make_unique<OSFSurface>("osf-panel");

  // Configure for top panel
  surface_->setLayer(Layer::Top);
  surface_->setAnchor(Anchor::Top | Anchor::Left | Anchor::Right);
  surface_->setSize(0, AresTheme::PanelHeight); // 0 width = stretch
  surface_->setExclusiveZone(AresTheme::PanelHeight);

  // Bind callbacks
  surface_->onDraw([this](cairo_t *cr, int w, int h) { this->draw(cr, w, h); });
}

void OSFPanel::run() {
  if (surface_->connect()) {
    surface_->run();
  } else {
    std::cerr << "Failed to connect OSFPanel to display." << std::endl;
  }
}

void OSFPanel::draw(cairo_t *cr, int width, int height) {
  // 1. Background (Semi-transparent)
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  AresTheme::setCairoColor(cr, AresTheme::PanelBackground);
  cairo_paint(cr);

  // switch to OVER for content
  cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

  // 2. Start Button (Orange Square + Text)
  drawStartButton(cr, 12, (height - 12) / 2.0);

  // 3. Clock (Right side)
  drawClock(cr, width, height);
}

void OSFPanel::drawStartButton(cairo_t *cr, double x, double y) {
  // Orange square (12x12)
  AresTheme::setCairoColor(cr, AresTheme::MarsOrange);
  AresTheme::roundedRect(cr, x, y, 12, 12, 2);
  cairo_fill(cr);

  // "Filer" text (actually "VitusOS" or whatever currently active app, but spec
  // said 'Filer')
  cairo_select_font_face(cr, AresTheme::FontFamily, CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, AresTheme::FontSizeNormal);
  AresTheme::setCairoColor(cr, AresTheme::StarWhite);

  cairo_move_to(cr, x + 20, y + 10); // align baseline
  cairo_show_text(cr, "Filer");

  // Menu Items
  cairo_select_font_face(cr, AresTheme::FontFamily, CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);
  cairo_move_to(cr, x + 70, y + 10);
  cairo_show_text(cr, "Menu");

  cairo_move_to(cr, x + 120, y + 10);
  cairo_show_text(cr, "Settings");

  cairo_move_to(cr, x + 190, y + 10);
  cairo_show_text(cr, "Help");
}

void OSFPanel::drawClock(cairo_t *cr, int width, int height) {
  auto now = std::time(nullptr);
  auto tm = *std::localtime(&now);

  std::ostringstream oss;
  oss << std::put_time(&tm, "%H:%M");
  std::string timeStr = oss.str();

  cairo_select_font_face(cr, AresTheme::FontFamily, CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, AresTheme::FontSizeNormal);

  cairo_text_extents_t extents;
  cairo_text_extents(cr, timeStr.c_str(), &extents);

  double x = width - extents.width - 20;          // 20px padding from right
  double y = (height + extents.height) / 2.0 - 2; // vertically centered roughly

  AresTheme::setCairoColor(cr, AresTheme::StarWhite);
  cairo_move_to(cr, x, y);
  cairo_show_text(cr, timeStr.c_str());
}

} // namespace opensef
