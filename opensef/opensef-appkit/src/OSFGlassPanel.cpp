/**
 * OSFGlassPanel.cpp - Frosted Glass Effect Widget
 */

#include <cmath>
#include <opensef/OpenSEFAppKit.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace opensef {

namespace AresTheme {
constexpr uint32_t GlassBorder = 0x33FFFFFF;
constexpr double CornerRadius = 12.0;
constexpr double BorderWidth = 1.0;

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

OSFGlassPanel::OSFGlassPanel() = default;

std::shared_ptr<OSFGlassPanel> OSFGlassPanel::create(const OSFRect &frame) {
  auto panel = std::make_shared<OSFGlassPanel>();
  panel->setFrame(frame);
  return panel;
}

void OSFGlassPanel::setShadowEnabled(bool enabled) { shadowEnabled_ = enabled; }

void OSFGlassPanel::render(cairo_t *cr) {
  if (hidden_)
    return;

  double x = frame_.x, y = frame_.y, w = frame_.width, h = frame_.height;

  // Shadow
  if (shadowEnabled_) {
    cairo_set_source_rgba(cr, 0, 0, 0, 0.3);
    AresTheme::roundedRect(cr, x + 2, y + 4, w, h, AresTheme::CornerRadius);
    cairo_fill(cr);
  }

  // Background with tint
  tintColor_.setCairo(cr);
  AresTheme::roundedRect(cr, x, y, w, h, AresTheme::CornerRadius);
  cairo_fill(cr);

  // Inner glow
  cairo_pattern_t *gradient = cairo_pattern_create_linear(x, y, x, y + 30);
  cairo_pattern_add_color_stop_rgba(gradient, 0, 1, 1, 1, 0.1);
  cairo_pattern_add_color_stop_rgba(gradient, 1, 1, 1, 1, 0.0);
  cairo_set_source(cr, gradient);
  AresTheme::roundedRect(cr, x, y, w, h, AresTheme::CornerRadius);
  cairo_fill(cr);
  cairo_pattern_destroy(gradient);

  // Border
  AresTheme::setCairoColor(cr, AresTheme::GlassBorder);
  cairo_set_line_width(cr, AresTheme::BorderWidth);
  AresTheme::roundedRect(cr, x, y, w, h, AresTheme::CornerRadius);
  cairo_stroke(cr);

  // Render subviews
  OSFView::render(cr);
}

} // namespace opensef
