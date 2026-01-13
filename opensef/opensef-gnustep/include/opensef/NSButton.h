#pragma once

#include "NSControl.h"
#include <string>

// Button types
enum class NSButtonType {
  MomentaryLight,
  PushOnPushOff,
  Toggle,
  Switch,
  Radio,
  MomentaryChange,
  OnOff,
  MomentaryPushIn
};

// Bezel styles (macOS button styles)
enum class NSBezelStyle {
  Rounded,
  RegularSquare,
  ThickSquare,
  Disclosure,
  ShadowlessSquare,
  Circular,
  TexturedSquare,
  SmallSquare,
  TexturedRounded,
  RoundRect,
  Recessed,
  Inline
};

// Image positions
enum class NSCellImagePosition {
  NoImage,
  ImageOnly,
  ImageLeft,
  ImageRight,
  ImageBelow,
  ImageAbove,
  ImageOverlaps
};

/**
 * NSButton - Button widget
 *
 * Complete C++ conversion of GNUstep NSButton
 * Renders via Qt Quick (no Cairo)
 */
class NSButton : public NSControl {
public:
  NSButton();
  explicit NSButton(const NSRect &frame);
  virtual ~NSButton();

  // State management
  void setState(int value);
  int state() const { return m_state; }
  void setNextState();
  void setAllowsMixedState(bool flag) { m_allowsMixedState = flag; }
  bool allowsMixedState() const { return m_allowsMixedState; }

  // Title
  void setTitle(const std::string &title);
  std::string title() const { return m_title; }
  void setAlternateTitle(const std::string &title);
  std::string alternateTitle() const { return m_alternateTitle; }

  // Button type
  void setButtonType(NSButtonType type);
  NSButtonType buttonType() const { return m_type; }

  // Appearance
  void setBordered(bool flag);
  bool isBordered() const { return m_bordered; }
  void setTransparent(bool flag);
  bool isTransparent() const { return m_transparent; }
  void setBezelStyle(NSBezelStyle style);
  NSBezelStyle bezelStyle() const { return m_bezelStyle; }

  // Image
  void setImagePosition(NSCellImagePosition position);
  NSCellImagePosition imagePosition() const { return m_imagePosition; }

  // Icon (simplified - using icon name instead of NSImage*)
  void setIconName(const std::string &iconName);
  std::string iconName() const { return m_iconName; }

  // Key equivalents (shortcuts)
  void setKeyEquivalent(const std::string &key);
  std::string keyEquivalent() const { return m_keyEquivalent; }
  void setKeyEquivalentModifierMask(unsigned int mask);
  unsigned int keyEquivalentModifierMask() const { return m_keyEquivalentMask; }

  // Events (overrides)
  virtual bool acceptsFirstMouse(NSEvent *event) override;
  virtual bool isFlipped() const override { return true; }
  virtual void performClick() override;

  // Display
  void highlight(bool flag);

protected:
  // Radio button logic
  void handleRadioStates();

  std::string m_title;
  std::string m_alternateTitle;
  std::string m_iconName;
  int m_state = 0; // 0 = off, 1 = on, -1 = mixed
  NSButtonType m_type = NSButtonType::MomentaryLight;
  NSBezelStyle m_bezelStyle = NSBezelStyle::Rounded;
  NSCellImagePosition m_imagePosition = NSCellImagePosition::ImageLeft;
  bool m_bordered = true;
  bool m_transparent = false;
  bool m_allowsMixedState = false;
  bool m_highlighted = false;
  std::string m_keyEquivalent;
  unsigned int m_keyEquivalentMask = 0;
};
