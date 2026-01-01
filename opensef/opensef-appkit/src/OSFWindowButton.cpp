/**
 * OSFWindowButton.cpp - Traffic light button implementation
 */

#include <opensef/OSFWindowDecorations.h>

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
  // Calculate center
  double centerX = frame_.x + frame_.width / 2.0;
  double centerY = frame_.y + frame_.height / 2.0;
  double radius = frame_.width / 2.0; // Assume square

  // Pick color
  OSFColor color = normalColor();
void OSFWindowButton::draw(cairo_t *cr) {
  // Determine color based on state
  OSFColor color;
  if (pressed_) {
    color = pressedColor();
  } else if (hovered_) {
    color = hoverColor();
  }

  // Draw circle
  cairo_save(cr);
  color.setCairo(cr);
  cairo_arc(cr, centerX, centerY, radius, 0, 2 * M_PI);
  cairo_fill(cr);

  // Optional: Border for better visibility on dark backgrounds?
  // macOS doesn't really have a border, but let's keep it simple.

  } else {
    color = normalColor();
  }

  // Draw circle
  float radius = kButtonRadius;
  float centerX = frame_.x + frame_.width / 2.0f;
  float centerY = frame_.y + frame_.height / 2.0f;

  cairo_save(cr);
  color.setCairo(cr);
  cairo_arc(cr, centerX, centerY, radius, 0, 2 * 3.1415926535);
  cairo_fill(cr);
  cairo_restore(cr);
}

} // namespace opensef
