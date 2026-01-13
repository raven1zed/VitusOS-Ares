#pragma once

#include "NSObject.h"
#include <functional>
#include <vector>


// Forward declarations
class NSEvent;
class NSWindow;

// Geometry structures
struct NSPoint {
  double x = 0;
  double y = 0;
  NSPoint() = default;
  NSPoint(double x_, double y_) : x(x_), y(y_) {}
};

struct NSSize {
  double width = 0;
  double height = 0;
  NSSize() = default;
  NSSize(double w, double h) : width(w), height(h) {}
};

struct NSRect {
  NSPoint origin;
  NSSize size;
  NSRect() = default;
  NSRect(double x, double y, double w, double h) : origin(x, y), size(w, h) {}
};

/**
 * NSView - Base view class
 *
 * Equivalent to Cocoa NSView, manages view hierarchy and rendering
 */
class NSView : public NSObject {
public:
  NSView();
  explicit NSView(const NSRect &frame);
  virtual ~NSView();

  // Geometry
  void setFrame(const NSRect &frame);
  NSRect frame() const { return m_frame; }
  void setBounds(const NSRect &bounds);
  NSRect bounds() const { return m_bounds; }

  // Hierarchy
  void addSubview(NSView *view);
  void removeFromSuperview();
  void removeSubview(NSView *view);
  NSView *superview() const { return m_superview; }
  const std::vector<NSView *> &subviews() const { return m_subviews; }

  // Window
  NSWindow *window() const { return m_window; }
  void setWindow(NSWindow *window) { m_window = window; }

  // Display
  void setNeedsDisplay(bool flag);
  bool needsDisplay() const { return m_needsDisplay; }
  virtual void display();
  virtual void drawRect(const NSRect &rect) {}

  // Coordinate system
  virtual bool isFlipped() const { return false; }
  NSPoint convertPoint(const NSPoint &point, NSView *fromView);

  // Events
  virtual bool acceptsFirstMouse(NSEvent *event) { return false; }
  virtual void mouseDown(NSEvent *event) {}
  virtual void mouseUp(NSEvent *event) {}
  virtual void mouseMoved(NSEvent *event) {}
  virtual void keyDown(NSEvent *event) {}
  virtual void keyUp(NSEvent *event) {}

  // First responder
  virtual bool acceptsFirstResponder() const { return false; }
  virtual bool becomeFirstResponder() { return false; }
  virtual bool resignFirstResponder() { return false; }

  // Enabled state
  void setEnabled(bool enabled) { m_enabled = enabled; }
  bool isEnabled() const { return m_enabled; }

  // Hidden state
  void setHidden(bool hidden);
  bool isHidden() const { return m_hidden; }

  // Alpha
  void setAlphaValue(double alpha) {
    m_alpha = alpha;
    setNeedsDisplay(true);
  }
  double alphaValue() const { return m_alpha; }

protected:
  NSRect m_frame;
  NSRect m_bounds;
  NSView *m_superview = nullptr;
  NSWindow *m_window = nullptr;
  std::vector<NSView *> m_subviews;
  bool m_needsDisplay = false;
  bool m_enabled = true;
  bool m_hidden = false;
  double m_alpha = 1.0;
};
