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
  setFrame(OSFRect(0, 0, diameter, diameter));

  // Initialize layer
  if (layer_) {
    layer_->setCornerRadius(kButtonRadius);
    layer_->setBackgroundColor(normalColor());
  }
}

OSFWindowButton::~OSFWindowButton() = default;

std::shared_ptr<OSFWindowButton>
OSFWindowButton::create(OSFWindowButtonType type) {
  return std::make_shared<OSFWindowButton>(type);
}

OSFColor OSFWindowButton::normalColor() const {
  switch (type_) {
  case OSFWindowButtonType::Close:
    return OSFColor::fromHex(0xFF4D00); // Mars Orange
  case OSFWindowButtonType::Minimize:
    return OSFColor::fromHex(0xFFD700); // Mars Gold
  case OSFWindowButtonType::Maximize:
    return OSFColor::fromHex(0x007AFF); // Ares Blue
  }
  return OSFColor::fromHex(0x808080);
}

OSFColor OSFWindowButton::hoverColor() const {
  switch (type_) {
  case OSFWindowButtonType::Close:
    return OSFColor::fromHex(0xFF6347); // Tomato (lighter orange)
  case OSFWindowButtonType::Minimize:
    return OSFColor::fromHex(0xFFE066); // Lighter Gold
  case OSFWindowButtonType::Maximize:
    return OSFColor::fromHex(0x3395FF); // Lighter Blue
  }
  return OSFColor::fromHex(0x909090);
}

OSFColor OSFWindowButton::pressedColor() const {
  switch (type_) {
  case OSFWindowButtonType::Close:
    return OSFColor::fromHex(0xCC3D00);
  case OSFWindowButtonType::Minimize:
    return OSFColor::fromHex(0xCCAC00);
  case OSFWindowButtonType::Maximize:
    return OSFColor::fromHex(0x0062CC);
  }
  return OSFColor::fromHex(0x707070);
}

void OSFWindowButton::setHovered(bool hovered) {
  if (hovered_ == hovered)
    return;
  hovered_ = hovered;

  // Animate color change
  if (layer_) {
    layer_->setBackgroundColor(hovered_ ? hoverColor() : normalColor());
  }
}

void OSFWindowButton::setPressed(bool pressed) {
  if (pressed_ == pressed)
    return;
  pressed_ = pressed;

  if (layer_) {
    layer_->setBackgroundColor(
        pressed_ ? pressedColor() : (hovered_ ? hoverColor() : normalColor()));
    // Slight scale on press
    layer_->setScale(pressed_ ? 0.92 : 1.0);
  }
}

bool OSFWindowButton::mouseDown(OSFEvent &event) {
  (void)event;
  setPressed(true);
  return true;
}

bool OSFWindowButton::mouseUp(OSFEvent &event) {
  (void)event;
  if (pressed_) {
    setPressed(false);
    click();
  }
  return true;
}

void OSFWindowButton::click() {
  if (action_) {
    action_();
  }
}

void OSFWindowButton::render(cairo_t *cr) {
  // OSFView::render will handle the layer drawing
  OSFView::render(cr);

  // TODO: Add symbol drawing on top if alpha > 0
}

} // namespace opensef
