/**
 * OSFButton.cpp - Production Button Implementation
 */

#include <cmath>
#include <opensef/OpenSEFAppKit.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace opensef {

// AresTheme colors
namespace AresTheme {
constexpr uint32_t MarsOrange = 0xFFE57C3A;
constexpr uint32_t StarWhite = 0xFFF5F5F5;
constexpr uint32_t LunarGray = 0xFF2D2D2D;
constexpr uint32_t Nebula = 0xFF3D3D3D;
constexpr double CornerRadius = 6.0;
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
                        double r) {
  cairo_new_sub_path(cr);
  cairo_arc(cr, x + w - r, y + r, r, -M_PI / 2, 0);
  cairo_arc(cr, x + w - r, y + h - r, r, 0, M_PI / 2);
  cairo_arc(cr, x + r, y + h - r, r, M_PI / 2, M_PI);
  cairo_arc(cr, x + r, y + r, r, M_PI, 3 * M_PI / 2);
  cairo_close_path(cr);
}
} // namespace AresTheme

OSFButton::OSFButton() = default;

OSFButton::OSFButton(const std::string &label, std::function<void()> action)
    : label_(label), action_(action) {}

std::shared_ptr<OSFButton> OSFButton::create(const std::string &label,
                                             std::function<void()> action) {
  return std::make_shared<OSFButton>(label, action);
}

void OSFButton::click() {
  if (action_) {
    action_();
  }
}

void OSFButton::handleMouseEnter() { hovered_ = true; }
void OSFButton::handleMouseLeave() { hovered_ = false; }
void OSFButton::handleMouseDown() { pressed_ = true; }
void OSFButton::handleMouseUp() {
  pressed_ = false;
  if (hovered_)
    click();
}

void OSFButton::render(cairo_t *cr) {
  if (hidden_)
    return;

  double x = frame_.x;
  double y = frame_.y;
  double w = frame_.width;
  double h = frame_.height;

  // Background color based on state
  uint32_t bgColor = pressed_ ? AresTheme::MarsOrange : AresTheme::LunarGray;
  uint32_t textColor = AresTheme::StarWhite;

  if (hovered_ && !pressed_) {
    bgColor = AresTheme::Nebula;
  }

  // Draw background
  AresTheme::setCairoColor(cr, bgColor);
  AresTheme::roundedRect(cr, x, y, w, h, AresTheme::CornerRadius);
  cairo_fill(cr);

  // Draw label
  AresTheme::setCairoColor(cr, textColor);
  cairo_select_font_face(cr, AresTheme::FontFamily, CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_MEDIUM);
  cairo_set_font_size(cr, AresTheme::FontSizeNormal);

  cairo_text_extents_t extents;
  cairo_text_extents(cr, label_.c_str(), &extents);

  double tx = x + (w - extents.width) / 2 - extents.x_bearing;
  double ty = y + (h - extents.height) / 2 - extents.y_bearing;

  cairo_move_to(cr, tx, ty);
  cairo_show_text(cr, label_.c_str());
}

} // namespace opensef
