/**
 * OSFPasswordField.cpp - Password input implementation
 */

#include "OSFPasswordField.h"
#include <OSFAresTheme.h>
#include <cmath>
#include <linux/input-event-codes.h>


namespace opensef {

OSFPasswordField::OSFPasswordField() {
  placeholder_ = "password";
  setFrame(OSFRect(0, 0, 300, 50));
}

void OSFPasswordField::setValue(const std::string &value) {
  value_ = value;
  setNeedsDisplay();
}

void OSFPasswordField::clear() {
  value_.clear();
  setNeedsDisplay();
}

void OSFPasswordField::setError(bool error) {
  error_ = error;
  if (error) {
    startShakeAnimation();
  }
  setNeedsDisplay();
}

void OSFPasswordField::startShakeAnimation() {
  shakeOffset_ = 10.0;
  setNeedsDisplay();
}

void OSFPasswordField::render(cairo_t *cr) {
  auto frame = this->frame();
  double x = frame.x + shakeOffset_;
  double y = frame.y;
  double w = frame.width;
  double h = frame.height;

  cairo_save(cr);
  cairo_translate(cr, x, y);

  cairo_select_font_face(cr, AresTheme::FontFamily, CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, 14);

  if (value_.empty()) {
    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.5);
    cairo_move_to(cr, 0, h / 2 + 5);
    cairo_show_text(cr, placeholder_.c_str());
  } else {
    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
    std::string dots(value_.length(), '*');
    cairo_move_to(cr, 0, h / 2 + 5);
    cairo_show_text(cr, dots.c_str());
  }

  if (error_) {
    cairo_set_source_rgb(cr, 0.9, 0.2, 0.2);
  } else {
    AresTheme::setCairoColor(cr, AresTheme::MarsOrange);
  }

  cairo_rectangle(cr, 0, h - 3, w, 3);
  cairo_fill(cr);

  if (focused_) {
    cairo_set_source_rgba(cr, 0.9, 0.49, 0.23, 0.3);
    cairo_rectangle(cr, -5, -5, w + 10, h + 10);
    cairo_set_line_width(cr, 2);
    cairo_stroke(cr);
  }

  cairo_restore(cr);
  shakeOffset_ = 0.0;
}

bool OSFPasswordField::keyDown(OSFEvent &event) {
  uint32_t keyCode = event.keyCode();

  if (keyCode == KEY_ENTER) {
    if (submitCallback_) {
      submitCallback_(value_);
    }
    return true;
  }

  if (keyCode == KEY_BACKSPACE) {
    if (!value_.empty()) {
      value_.pop_back();
      setNeedsDisplay();
    }
    return true;
  }

  return true;
}

} // namespace opensef
