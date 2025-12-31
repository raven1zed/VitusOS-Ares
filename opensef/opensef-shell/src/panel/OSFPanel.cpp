#include "OSFPanel.h"
#include "OSFAresTheme.h"
#include <opensef/OpenSEFAppKit.h>

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

  // Create widgets using the widget library
  initWidgets();

  // Bind callbacks
  surface_->onDraw([this](cairo_t *cr, int w, int h) { this->draw(cr, w, h); });
}

void OSFPanel::initWidgets() {
  // App name button (bold, orange indicator)
  appNameButton_ = OSFButton::create("Filer");
  appNameButton_->setFrame(OSFRect(12, 8, 50, 16));

  // Menu items as labels
  menuFile_ = OSFLabel::create("Menu");
  menuFile_->setFrame(OSFRect(70, 8, 40, 16));

  menuSettings_ = OSFLabel::create("Settings");
  menuSettings_->setFrame(OSFRect(120, 8, 60, 16));

  menuHelp_ = OSFLabel::create("Help");
  menuHelp_->setFrame(OSFRect(190, 8, 40, 16));

  // Clock label (positioned in draw based on width)
  clockLabel_ = OSFLabel::create("");
  clockLabel_->setAlignment(TextAlignment::Right);
}

void OSFPanel::run() {
  if (surface_->connect()) {
    surface_->run();
  } else {
    std::cerr << "Failed to connect OSFPanel to display." << std::endl;
  }
}

void OSFPanel::draw(cairo_t *cr, int width, int height) {
  // 1. Background (Semi-transparent Theme Color)
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  AresTheme::setCairoColor(cr, AresTheme::PanelBackground);
  cairo_paint(cr);

  // switch to OVER for content
  cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

  // 2. Orange indicator square (signature element)
  AresTheme::setCairoColor(cr, AresTheme::MarsOrange);
  AresTheme::roundedRect(cr, 12, (height - 12) / 2.0, 12, 12, 2);
  cairo_fill(cr);

  // 3. App name (bold)
  cairo_select_font_face(cr, AresTheme::FontFamily, CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, AresTheme::FontSizeNormal);
  AresTheme::setCairoColor(cr, AresTheme::StarWhite);
  cairo_move_to(cr, 32, (height + 12) / 2.0);
  cairo_show_text(cr, "Filer");

  // 4. Menu items using OSFLabel widgets
  menuFile_->setFrame(OSFRect(80, (height - 16) / 2.0, 40, 16));
  menuFile_->render(cr);

  menuSettings_->setFrame(OSFRect(130, (height - 16) / 2.0, 60, 16));
  menuSettings_->render(cr);

  menuHelp_->setFrame(OSFRect(200, (height - 16) / 2.0, 40, 16));
  menuHelp_->render(cr);

  // 5. Clock using widget
  updateClock();
  // Adjust clock position for longer text
  clockLabel_->setFrame(OSFRect(width - 180, (height - 16) / 2.0, 160, 16));
  clockLabel_->render(cr);
}

void OSFPanel::updateClock() {
  auto now = std::time(nullptr);
  auto tm = *std::localtime(&now);

  std::ostringstream oss;
  // Format: Tuesday,12 10:15 PM
  oss << std::put_time(&tm, "%A,%d %I:%M %p");
  clockLabel_->setText(oss.str());
}

} // namespace opensef
