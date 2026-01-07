/**
 * OSFGeometry.h - Basic geometry types
 * Part of opensef-base foundation
 */
#pragma once

#include <cairo/cairo.h>
#include <cmath>
#include <cstdint>

namespace opensef {

struct OSFPoint {
  double x = 0.0;
  double y = 0.0;

  OSFPoint() = default;
  OSFPoint(double x_, double y_) : x(x_), y(y_) {}

  static OSFPoint Zero() { return OSFPoint(0, 0); }
};

struct OSFSize {
  double width = 0.0;
  double height = 0.0;

  OSFSize() = default;
  OSFSize(double w, double h) : width(w), height(h) {}

  static OSFSize Zero() { return OSFSize(0, 0); }
};

struct OSFRect {
  double x = 0, y = 0, width = 0, height = 0;
  OSFRect() = default;
  OSFRect(double x_, double y_, double w, double h)
      : x(x_), y(y_), width(w), height(h) {}

  OSFRect(OSFPoint origin, OSFSize size)
      : x(origin.x), y(origin.y), width(size.width), height(size.height) {}

  static OSFRect Zero() { return OSFRect(0, 0, 0, 0); }

  OSFPoint origin() const { return {x, y}; }
  OSFSize size() const { return {width, height}; }

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
