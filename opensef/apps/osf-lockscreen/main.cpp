/**
 * main.cpp - VitusOS Lock Screen
 */

#include <OSFAuth.h>
#include <OSFSurface.h>
#include <cairo.h>
#include <ctime>
#include <iostream>
#include <opensef/OSFAresTheme.h>

using namespace opensef;

class OSFLockScreen {
public:
  OSFLockScreen() {
    surface_ = std::make_unique<OSFSurface>("osf-lockscreen");

    surface_->setLayer(Layer::Overlay);
    surface_->setAnchor(Anchor::Top | Anchor::Bottom | Anchor::Left |
                        Anchor::Right);
    surface_->setExclusiveZone(-1);

    username_ = OSFAuth::getCurrentUser();
    if (username_.empty()) {
      username_ = "user";
    }

    surface_->onDraw(
        [this](cairo_t *cr, int w, int h) { this->draw(cr, w, h); });
  }

  void run() {
    if (surface_->connect()) {
      std::cout << "Lock screen connected to display" << std::endl;
      surface_->run();
    } else {
      std::cerr << "Failed to connect lock screen" << std::endl;
    }
  }

private:
  void draw(cairo_t *cr, int width, int height) {
    // Dark gradient background (Mars-inspired)
    cairo_pattern_t *gradient = cairo_pattern_create_linear(0, 0, 0, height);
    cairo_pattern_add_color_stop_rgb(gradient, 0, 0.1, 0.05, 0.05);
    cairo_pattern_add_color_stop_rgb(gradient, 1, 0.15, 0.08, 0.05);
    cairo_set_source(cr, gradient);
    cairo_paint(cr);
    cairo_pattern_destroy(gradient);

    // Get current time
    time_t now = time(nullptr);
    struct tm *t = localtime(&now);
    char dateStr[64];
    char timeStr[32];

    strftime(dateStr, sizeof(dateStr), "%A %d, %Y", t);
    strftime(timeStr, sizeof(timeStr), "%I:%M %p", t);

    // Convert date to lowercase
    for (char *p = dateStr; *p; ++p) {
      *p = tolower(*p);
    }

    cairo_text_extents_t extents;

    // Draw date (top center)
    cairo_select_font_face(cr, AresTheme::FontFamily, CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 16);
    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.9);

    cairo_text_extents(cr, dateStr, &extents);
    cairo_move_to(cr, (width - extents.width) / 2, 100);
    cairo_show_text(cr, dateStr);

    // Draw time (center, Mars Orange)
    cairo_select_font_face(cr, AresTheme::FontFamily, CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 72);
    AresTheme::setCairoColor(cr, AresTheme::MarsOrange);

    cairo_text_extents(cr, timeStr, &extents);
    cairo_move_to(cr, (width - extents.width) / 2, height / 2 - 50);
    cairo_show_text(cr, timeStr);

    // Draw password field placeholder
    int fieldWidth = 300;
    int fieldX = (width - fieldWidth) / 2;
    int fieldY = height / 2 + 50;

    cairo_select_font_face(cr, AresTheme::FontFamily, CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 14);

    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.5);
    cairo_move_to(cr, fieldX, fieldY + 20);
    cairo_show_text(cr, "password");

    // Draw underline (Mars Orange)
    AresTheme::setCairoColor(cr, AresTheme::MarsOrange);
    cairo_rectangle(cr, fieldX, fieldY + 30, fieldWidth, 3);
    cairo_fill(cr);
  }

  std::unique_ptr<OSFSurface> surface_;
  std::string username_;
};

int main() {
  std::cout << "=== VitusOS Lock Screen ===" << std::endl;
  std::cout << "Minimal, premium login interface" << std::endl;

  OSFLockScreen lockScreen;
  lockScreen.run();

  return 0;
}
