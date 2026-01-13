#pragma once

#include "NSView.h"
#include <functional>

/**
 * NSControl - Base class for controls (buttons, text fields, etc.)
 *
 * Provides target-action pattern and cell management
 */
class NSControl : public NSView {
public:
  NSControl();
  explicit NSControl(const NSRect &frame);
  virtual ~NSControl();

  // Target-action
  void setAction(std::function<void()> action) { m_action = action; }
  std::function<void()> action() const { return m_action; }
  void sendAction();

  // Perform click
  virtual void performClick();

  // Value
  void setIntValue(int value);
  int intValue() const { return m_intValue; }
  void setDoubleValue(double value);
  double doubleValue() const { return m_doubleValue; }
  void setStringValue(const std::string &value);
  std::string stringValue() const { return m_stringValue; }

  // Enabled (override)
  virtual bool acceptsFirstResponder() const override { return isEnabled(); }

protected:
  std::function<void()> m_action;
  int m_intValue = 0;
  double m_doubleValue = 0.0;
  std::string m_stringValue;
};
