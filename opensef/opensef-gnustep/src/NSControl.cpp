#include "opensef/NSControl.h"

NSControl::NSControl() : NSView() {}

NSControl::NSControl(const NSRect &frame) : NSView(frame) {}

NSControl::~NSControl() {}

void NSControl::sendAction() {
  if (m_action && isEnabled()) {
    m_action();
  }
}

void NSControl::performClick() { sendAction(); }

void NSControl::setIntValue(int value) {
  m_intValue = value;
  m_doubleValue = static_cast<double>(value);
  m_stringValue = std::to_string(value);
  setNeedsDisplay(true);
}

void NSControl::setDoubleValue(double value) {
  m_doubleValue = value;
  m_intValue = static_cast<int>(value);
  m_stringValue = std::to_string(value);
  setNeedsDisplay(true);
}

void NSControl::setStringValue(const std::string &value) {
  m_stringValue = value;
  try {
    m_doubleValue = std::stod(value);
    m_intValue = static_cast<int>(m_doubleValue);
  } catch (...) {
    m_doubleValue = 0.0;
    m_intValue = 0;
  }
  setNeedsDisplay(true);
}
