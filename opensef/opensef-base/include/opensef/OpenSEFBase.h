/**
 * openSEF Base: Foundation Layer (C++ Version)
 *
 * Open SeagrEnv Framework - Core object system
 * Originally designed for GNUstep, now pure C++ for portability
 */

#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <vector>


namespace opensef {

// ============================================================================
// OSFObject - Root class for all openSEF objects
// ============================================================================

class OSFObject {
public:
  OSFObject();
  virtual ~OSFObject();

  /** Unique identifier for this object */
  const std::string &objectID() const { return objectID_; }

  /** Create a new openSEF object */
  static std::shared_ptr<OSFObject> create();

protected:
  std::string objectID_;

private:
  static std::atomic<uint64_t> nextID_;
};

// ============================================================================
// OSFString - Enhanced string (thin wrapper for now)
// ============================================================================

using OSFString = std::string;

// ============================================================================
// OSFArray - Enhanced array (thin wrapper for now)
// ============================================================================

template <typename T> using OSFArray = std::vector<T>;

// ============================================================================
// Basic types (replacing Foundation types)
// ============================================================================

struct OSFRect {
  float x = 0, y = 0;
  float width = 0, height = 0;

  OSFRect() = default;
  OSFRect(float x, float y, float w, float h)
      : x(x), y(y), width(w), height(h) {}

  static OSFRect Zero() { return OSFRect(0, 0, 0, 0); }
};

struct OSFPoint {
  float x = 0, y = 0;

  OSFPoint() = default;
  OSFPoint(float x, float y) : x(x), y(y) {}
};

struct OSFSize {
  float width = 0, height = 0;

  OSFSize() = default;
  OSFSize(float w, float h) : width(w), height(h) {}
};

// ============================================================================
// Color (RGBA)
// ============================================================================

struct OSFColor {
  float r = 0, g = 0, b = 0, a = 1;

  OSFColor() = default;
  OSFColor(float r, float g, float b, float a = 1.0f)
      : r(r), g(g), b(b), a(a) {}

  static OSFColor fromHex(uint32_t hex, float alpha = 1.0f) {
    return OSFColor(((hex >> 16) & 0xFF) / 255.0f, ((hex >> 8) & 0xFF) / 255.0f,
                    (hex & 0xFF) / 255.0f, alpha);
  }

  OSFColor withAlpha(float alpha) const { return OSFColor(r, g, b, alpha); }
};

} // namespace opensef
