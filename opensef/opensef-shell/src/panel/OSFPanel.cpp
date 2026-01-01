#include "OSFPanel.h"
#include "OSFAresTheme.h"
#include <opensef/OpenSEFAppKit.h>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <linux/input-event-codes.h>
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
  surface_->onMouseMove(
      [this](int x, int y) { this->handlePointerMove(x, y); });
  surface_->onMouseDown(
      [this](int x, int y, uint32_t button) { this->handlePointerDown(x, y, button); });
  surface_->onMouseUp(
      [this](int x, int y, uint32_t button) { this->handlePointerUp(x, y, button); });
  surface_->onMouseLeave([this]() { this->clearHover(); });
  surface_->onTick([this]() { surface_->requestRedraw(); });
  surface_->setFrameTimer(1000);
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

  widgets_.clear();
  widgets_.push_back(appNameButton_);
}

void OSFPanel::run() {
  if (surface_->connect()) {
    // Add a 1-second timer to update the clock
    surface_->addTimer(1000, [this]() {
      if (surface_->width() > 0 && surface_->height() > 0) {
        // Create paint context manually to force redraw
        cairo_t *cr = surface_->beginPaint();
        if (cr) {
          this->draw(cr, surface_->width(), surface_->height());
          cairo_destroy(cr);
          surface_->endPaint();
          surface_->damage(0, 0, surface_->width(), surface_->height());
          surface_->commit();
        }
      }
    });

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

  // 3. App name button
  appNameButton_->setFrame(OSFRect(32, (height - 18) / 2.0, 60, 18));
  appNameButton_->render(cr);

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

std::shared_ptr<OSFButton> OSFPanel::hitTestButton(int x, int y) {
  for (const auto &widget : widgets_) {
    auto button = std::dynamic_pointer_cast<OSFButton>(widget);
    if (!button) {
      continue;
    }

    auto frame = button->frame();
    if (x >= frame.x && x <= frame.x + frame.width && y >= frame.y &&
        y <= frame.y + frame.height) {
      return button;
    }
  }

  return nullptr;
}

void OSFPanel::handlePointerMove(int x, int y) {
  auto button = hitTestButton(x, y);
  if (button == hoveredButton_) {
    return;
  }

  if (hoveredButton_) {
    hoveredButton_->handleMouseLeave();
  }

  hoveredButton_ = button;

  if (hoveredButton_) {
    hoveredButton_->handleMouseEnter();
  }

  surface_->requestRedraw();
}

void OSFPanel::handlePointerDown(int x, int y, uint32_t button) {
  if (button != BTN_LEFT) {
    return;
  }

  handlePointerMove(x, y);

  pressedButton_ = hoveredButton_;
  if (pressedButton_) {
    pressedButton_->handleMouseDown();
    surface_->requestRedraw();
  }
}

void OSFPanel::handlePointerUp(int x, int y, uint32_t button) {
  if (button != BTN_LEFT) {
    return;
  }

  handlePointerMove(x, y);

  if (pressedButton_) {
    pressedButton_->handleMouseUp();
    pressedButton_.reset();
    surface_->requestRedraw();
  }
}

void OSFPanel::clearHover() {
  if (hoveredButton_) {
    hoveredButton_->handleMouseLeave();
    hoveredButton_.reset();
    surface_->requestRedraw();
  }
}

} // namespace opensef
