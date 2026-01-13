/**
 * NSWindowButton.cpp - Traffic Light Window Button Implementation
 */

#include "opensef/NSWindowButton.h"

namespace OpenSEF {

NSWindowButton::NSWindowButton(NSWindowButtonType type)
    : NSButton(), m_windowType(type) {
  // Set circular button size
  setFrame(NSRect{0, 0, ButtonSize, ButtonSize});

  // Configure button behavior
  setButtonType(NSButtonType::MomentaryLight);
  setBordered(false);

  // No title for window buttons
  setTitle("");
}

NSWindowButton::~NSWindowButton() = default;

uint32_t NSWindowButton::baseColor() const {
  switch (m_windowType) {
  case NSWindowButtonType::Close:
    return CloseColor;
  case NSWindowButtonType::Minimize:
    return MinimizeColor;
  case NSWindowButtonType::Maximize:
    return MaximizeColor;
  }
  return 0x808080; // Fallback gray
}

uint32_t NSWindowButton::hoverColor() const {
  switch (m_windowType) {
  case NSWindowButtonType::Close:
    return CloseHoverColor;
  case NSWindowButtonType::Minimize:
    return MinimizeHoverColor;
  case NSWindowButtonType::Maximize:
    return MaximizeHoverColor;
  }
  return 0x909090;
}

uint32_t NSWindowButton::currentColor() const {
  if (!isEnabled()) {
    return 0xCCCCCC; // Disabled gray
  }
  return m_hovered ? hoverColor() : baseColor();
}

void NSWindowButton::setHovered(bool hovered) {
  if (m_hovered != hovered) {
    m_hovered = hovered;
    setNeedsDisplay(true);
  }
}

} // namespace OpenSEF
