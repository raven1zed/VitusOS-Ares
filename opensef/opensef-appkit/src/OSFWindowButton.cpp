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

void OSFWindowButton::draw() {
  // TODO: Draw circle with color based on state
  // For now, this is a stub
}

} // namespace opensef
