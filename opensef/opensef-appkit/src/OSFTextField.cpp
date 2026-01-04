/**
 * OSFTextField.cpp - Text Input Widget
 */

#include <cmath>
#include <opensef/OpenSEFAppKit.h>
#include <opensef/OSFWindow.h>
#include <pango/pangocairo.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace opensef {

namespace AresTheme {
constexpr uint32_t StarWhite = 0xFFF5F5F5;
constexpr uint32_t LunarGray = 0xFF2D2D2D;
constexpr uint32_t MarsOrange = 0xFFE57C3A;
constexpr uint32_t DeepSpace = 0xFF1A1A1A;
constexpr double CornerRadius = 6.0;
constexpr double BorderWidth = 1.5;
constexpr const char *FontFamily = "Inter";
constexpr double FontSizeNormal = 13.0;

inline void setCairoColor(cairo_t *cr, uint32_t color) {
  double a = ((color >> 24) & 0xFF) / 255.0;
  double r = ((color >> 16) & 0xFF) / 255.0;
  double g = ((color >> 8) & 0xFF) / 255.0;
  double b = (color & 0xFF) / 255.0;
  cairo_set_source_rgba(cr, r, g, b, a);
}

inline void roundedRect(cairo_t *cr, double x, double y, double w, double h,
                        double rad) {
  cairo_new_sub_path(cr);
  cairo_arc(cr, x + w - rad, y + rad, rad, -M_PI / 2, 0);
  cairo_arc(cr, x + w - rad, y + h - rad, rad, 0, M_PI / 2);
  cairo_arc(cr, x + rad, y + h - rad, rad, M_PI / 2, M_PI);
  cairo_arc(cr, x + rad, y + rad, rad, M_PI, 3 * M_PI / 2);
  cairo_close_path(cr);
}
} // namespace AresTheme

OSFTextField::OSFTextField() = default;
OSFTextField::OSFTextField(const std::string &placeholder)
    : placeholder_(placeholder) {}

std::shared_ptr<OSFTextField>
OSFTextField::create(const std::string &placeholder) {
  return std::make_shared<OSFTextField>(placeholder);
}

void OSFTextField::setSecure(bool secure) { isSecure_ = secure; }

void OSFTextField::handleKeyPress(uint32_t key, const std::string &text) {
  if (key == 0xFF08) { // Backspace
    text_.pop_back();
  } else if (!text.empty()) {
    text_ += text;
  }
  if (onTextChanged_)
    onTextChanged_(text_);
}

// === OSFResponder Overrides ===

bool OSFTextField::mouseDown(OSFEvent &event) {
  // Claim focus on click
  if (window()) {
    window()->makeFirstResponder(this);
  }
  return OSFView::mouseDown(event);
}

bool OSFTextField::keyDown(OSFEvent &event) {
  if (!focused_) {
    return OSFView::keyDown(event);
  }

  // Basic text input mapping
  uint32_t keyCode = event.keyCode();

  // Handle backspace (XKB/Linux keycodes)
  if (keyCode == 0xFF08 || keyCode == 22) {
    handleKeyPress(0xFF08, "");
    event.setHandled(true);
    return true;
  }

  // Handle enter
  if (keyCode == 0xFF0D || keyCode == 36) {
    if (onSubmit_)
      onSubmit_(text_);
    event.setHandled(true);
    return true;
  }

  // Simple ASCII mapping for validation demo (A-Z, 0-9)
  // In a real app we'd use xkb_keysym_to_utf8
  char ch = 0;
  if (keyCode >= 'a' && keyCode <= 'z')
    ch = (char)keyCode;
  else if (keyCode >= 'A' && keyCode <= 'Z')
    ch = (char)keyCode;
  else if (keyCode >= '0' && keyCode <= '9')
    ch = (char)keyCode;
  else if (keyCode == ' ')
    ch = ' ';

  if (ch != 0) {
    std::string s(1, ch);
    handleKeyPress(0, s);
    event.setHandled(true);
    return true;
  }

  return OSFView::keyDown(event);
}

void OSFTextField::becomeFirstResponder() { handleFocus(true); }

void OSFTextField::resignFirstResponder() { handleFocus(false); }

void OSFTextField::handleFocus(bool focused) {
  focused_ = focused;
  cursorVisible_ = focused;
}

void OSFTextField::setOnTextChanged(
    std::function<void(const std::string &)> cb) {
  onTextChanged_ = cb;
}

void OSFTextField::setOnSubmit(std::function<void(const std::string &)> cb) {
  onSubmit_ = cb;
}

void OSFTextField::render(cairo_t *cr) {
  if (hidden_)
    return;

  double x = frame_.x, y = frame_.y, w = frame_.width, h = frame_.height;
  double padding = 10.0;

  // Background
  AresTheme::setCairoColor(cr, AresTheme::DeepSpace);
  AresTheme::roundedRect(cr, x, y, w, h, AresTheme::CornerRadius);
  cairo_fill(cr);

  // Border
  uint32_t borderColor =
      focused_ ? AresTheme::MarsOrange : AresTheme::LunarGray;
  AresTheme::setCairoColor(cr, borderColor);
  cairo_set_line_width(cr, AresTheme::BorderWidth);
  AresTheme::roundedRect(cr, x, y, w, h, AresTheme::CornerRadius);
  cairo_stroke(cr);

  // Text
  std::string displayText = text_.empty() ? placeholder_ : text_;
  bool isPlaceholder = text_.empty();
  if (isSecure_ && !text_.empty()) {
    displayText = std::string(text_.length(), '*');
  }

  PangoLayout *layout = pango_cairo_create_layout(cr);
  pango_layout_set_text(layout, displayText.c_str(), -1);

  PangoFontDescription *fontDesc = pango_font_description_new();
  pango_font_description_set_family(fontDesc, AresTheme::FontFamily);
  pango_font_description_set_size(
      fontDesc, static_cast<int>(AresTheme::FontSizeNormal * PANGO_SCALE));
  pango_layout_set_font_description(layout, fontDesc);

  uint32_t textColor = isPlaceholder ? 0xFF666666 : AresTheme::StarWhite;
  AresTheme::setCairoColor(cr, textColor);

  int textWidth, textHeight;
  pango_layout_get_pixel_size(layout, &textWidth, &textHeight);
  double ty = y + (h - textHeight) / 2.0;

  cairo_move_to(cr, x + padding, ty);
  pango_cairo_show_layout(cr, layout);

  // Cursor
  if (focused_ && cursorVisible_) {
    double cursorX = x + padding + textWidth + 2;
    AresTheme::setCairoColor(cr, AresTheme::MarsOrange);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr, cursorX, y + 6);
    cairo_line_to(cr, cursorX, y + h - 6);
    cairo_stroke(cr);
  }

  pango_font_description_free(fontDesc);
  g_object_unref(layout);
}

} // namespace opensef
