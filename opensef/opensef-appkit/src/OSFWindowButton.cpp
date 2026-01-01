/**
 * OSFWindowButton.cpp - Traffic light button implementation
 */

#include <cmath> // For M_PI
#include <opensef/OSFWindowDecorations.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace opensef {

OSFWindowButton::OSFWindowButton(OSFWindowButtonType type) : type_(type) {
  // Set button size
  float diameter = kButtonRadius * 2;
  frame_ = OSFRect(0, 0, diameter, diameter);
}

OSFWindowButton::~OSFWindowButton() = default;

std::shared_ptr<OSFWindowButton>
OSFWindowButton::create(OSFWindowButtonType type) {
  return std::make_shared<OSFWindowButton>(type);
}

OSFColor OSFWindowButton::normalColor() const {
  switch (type_) {
  case OSFWindowButtonType::Close:
    return OSFColor::fromHex(0xFF5F57); // Red
  case OSFWindowButtonType::Minimize:
    return OSFColor::fromHex(0xFFBD2E); // Yellow
  case OSFWindowButtonType::Maximize:
    return OSFColor::fromHex(0x28CA41); // Green
  }
  return OSFColor::fromHex(0x808080);
}

OSFColor OSFWindowButton::hoverColor() const {
  switch (type_) {
  case OSFWindowButtonType::Close:
    return OSFColor::fromHex(0xFF3B30);
  case OSFWindowButtonType::Minimize:
    return OSFColor::fromHex(0xFFB800);
  case OSFWindowButtonType::Maximize:
    return OSFColor::fromHex(0x00D400);
  }
  return OSFColor::fromHex(0x909090);
}

OSFColor OSFWindowButton::pressedColor() const {
  switch (type_) {
  case OSFWindowButtonType::Close:
    return OSFColor::fromHex(0xCC4840);
  case OSFWindowButtonType::Minimize:
    return OSFColor::fromHex(0xCCA020);
  case OSFWindowButtonType::Maximize:
    return OSFColor::fromHex(0x20A030);
  }
  return OSFColor::fromHex(0x707070);
}

void OSFWindowButton::click() {
  if (action_) {
    action_();
  }
}

void OSFWindowButton::render(cairo_t *cr) {
  // Determine color based on state
  OSFColor color;
  if (pressed_) {
    color = pressedColor();
  } else if (hovered_) {
    color = hoverColor();
  } else {
    color = normalColor();
  }

  // Calculate geometry
  float centerX = frame_.x + frame_.width / 2.0f;
  float centerY = frame_.y + frame_.height / 2.0f;
  // Use constant radius for consistency, ignoring frame stretching for now
  float radius = kButtonRadius;

  cairo_save(cr);
  color.setCairo(cr);
  cairo_arc(cr, centerX, centerY, radius, 0, 2 * M_PI);
  cairo_fill(cr);
  cairo_restore(cr);

  // Symbol drawing (X, -, +) could go here for hover states
}

} // namespace opensef
