#include "opensef/NSButton.h"
#include <algorithm>

NSButton::NSButton() : NSControl() { setFrame(NSRect{0, 0, 100, 30}); }

NSButton::NSButton(const NSRect &frame) : NSControl(frame) {}

NSButton::~NSButton() {}

// State management
void NSButton::setState(int value) {
  if (!m_allowsMixedState && value < 0) {
    value = 0;
  }

  if (m_state != value) {
    m_state = value;
    setNeedsDisplay(true);
  }
}

void NSButton::setNextState() {
  if (m_allowsMixedState) {
    // Cycle: off → on → mixed → off
    if (m_state == 0)
      setState(1);
    else if (m_state == 1)
      setState(-1);
    else
      setState(0);
  } else {
    // Toggle: off ↔ on
    setState(m_state == 0 ? 1 : 0);
  }
}

// Title
void NSButton::setTitle(const std::string &title) {
  if (m_title != title) {
    m_title = title;
    setStringValue(title);
    setNeedsDisplay(true);
  }
}

void NSButton::setAlternateTitle(const std::string &title) {
  if (m_alternateTitle != title) {
    m_alternateTitle = title;
    setNeedsDisplay(true);
  }
}

// Button type
void NSButton::setButtonType(NSButtonType type) {
  if (m_type != type) {
    m_type = type;

    // Configure behavior based on type
    switch (type) {
    case NSButtonType::MomentaryLight:
    case NSButtonType::MomentaryPushIn:
    case NSButtonType::MomentaryChange:
      setState(0); // Always off
      break;
    case NSButtonType::Toggle:
    case NSButtonType::OnOff:
    case NSButtonType::PushOnPushOff:
      // Can be on or off
      break;
    case NSButtonType::Radio:
      // Radio buttons - handle in group
      break;
    default:
      break;
    }

    setNeedsDisplay(true);
  }
}

// Appearance
void NSButton::setBordered(bool flag) {
  if (m_bordered != flag) {
    m_bordered = flag;
    setNeedsDisplay(true);
  }
}

void NSButton::setTransparent(bool flag) {
  if (m_transparent != flag) {
    m_transparent = flag;
    setNeedsDisplay(true);
  }
}

void NSButton::setBezelStyle(NSBezelStyle style) {
  if (m_bezelStyle != style) {
    m_bezelStyle = style;
    setNeedsDisplay(true);
  }
}

// Image
void NSButton::setImagePosition(NSCellImagePosition position) {
  if (m_imagePosition != position) {
    m_imagePosition = position;
    setNeedsDisplay(true);
  }
}

void NSButton::setIconName(const std::string &iconName) {
  if (m_iconName != iconName) {
    m_iconName = iconName;
    setNeedsDisplay(true);
  }
}

// Key equivalents
void NSButton::setKeyEquivalent(const std::string &key) {
  m_keyEquivalent = key;
}

void NSButton::setKeyEquivalentModifierMask(unsigned int mask) {
  m_keyEquivalentMask = mask;
}

// Events
bool NSButton::acceptsFirstMouse(NSEvent *event) {
  return true; // Buttons always accept first mouse
}

void NSButton::performClick() {
  if (!isEnabled())
    return;

  // Highlight briefly
  highlight(true);

  // Change state based on button type
  switch (m_type) {
  case NSButtonType::MomentaryLight:
  case NSButtonType::MomentaryPushIn:
  case NSButtonType::MomentaryChange:
    // Momentary buttons don't change state
    break;
  case NSButtonType::Toggle:
  case NSButtonType::OnOff:
  case NSButtonType::PushOnPushOff:
    setNextState();
    break;
  case NSButtonType::Radio:
    setState(1);
    handleRadioStates();
    break;
  default:
    break;
  }

  // Send action
  sendAction();

  // Remove highlight
  highlight(false);
}

void NSButton::highlight(bool flag) {
  if (m_highlighted != flag) {
    m_highlighted = flag;
    setNeedsDisplay(true);
  }
}

// Radio button logic
void NSButton::handleRadioStates() {
  if (m_type != NSButtonType::Radio)
    return;
  if (m_state != 1)
    return; // Only when turned on

  // Find sibling radio buttons and turn them off
  if (m_superview) {
    for (auto *subview : m_superview->subviews()) {
      NSButton *otherButton = dynamic_cast<NSButton *>(subview);
      if (otherButton && otherButton != this) {
        if (otherButton->buttonType() == NSButtonType::Radio) {
          // Same action = same radio group
          if (otherButton->action().target_type() == m_action.target_type()) {
            otherButton->setState(0);
          }
        }
      }
    }
  }
}
