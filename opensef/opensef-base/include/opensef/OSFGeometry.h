/**
 * OSFGeometry.h - Basic geometry types
 * Part of opensef-base foundation
 */
#pragma once

#include <cairo/cairo.h>
#include <cstdint>

namespace opensef {

struct OSFRect {
  double x = 0, y = 0, width = 0, height = 0;
  OSFRect() = default;
  OSFRect(double x_, double y_, double w, double h)
      : x(x_), y(y_), width(w), height(h) {}

  static OSFRect Zero() { return OSFRect(0, 0, 0, 0); }

  bool contains(double px, double py) const {
    return px >= x && px < x + width && py >= y && py < y + height;
  }
};

struct OSFColor {
  double r = 0, g = 0, b = 0, a = 1;
  OSFColor() = default;
  OSFColor(double r_, double g_, double b_, double a_ = 1.0)
      : r(r_), g(g_), b(b_), a(a_) {}

  static OSFColor fromHex(uint32_t hex, double alpha = 1.0) {
    return OSFColor(((hex >> 16) & 0xFF) / 255.0, ((hex >> 8) & 0xFF) / 255.0,
                    (hex & 0xFF) / 255.0, alpha);
  }

  static OSFColor fromARGB(uint32_t argb) {
    return OSFColor(((argb >> 16) & 0xFF) / 255.0, ((argb >> 8) & 0xFF) / 255.0,
                    (argb & 0xFF) / 255.0, ((argb >> 24) & 0xFF) / 255.0);
  }

  void setCairo(cairo_t *cr) const { cairo_set_source_rgba(cr, r, g, b, a); }
};

} // namespace opensef
