#include "opensef/NSView.h"
#include <algorithm>

NSView::NSView() : NSView(NSRect{0, 0, 100, 100}) {}

NSView::NSView(const NSRect &frame)
    : m_frame(frame), m_bounds{0, 0, frame.size.width, frame.size.height} {}

NSView::~NSView() {
  // Clean up subviews
  for (auto *subview : m_subviews) {
    subview->m_superview = nullptr;
  }
}

void NSView::setFrame(const NSRect &frame) {
  m_frame = frame;
  m_bounds.size = frame.size;
  setNeedsDisplay(true);
}

void NSView::setBounds(const NSRect &bounds) {
  m_bounds = bounds;
  setNeedsDisplay(true);
}

void NSView::addSubview(NSView *view) {
  if (!view)
    return;

  view->removeFromSuperview();
  view->m_superview = this;
  view->m_window = m_window;
  m_subviews.push_back(view);
  setNeedsDisplay(true);
}

void NSView::removeFromSuperview() {
  if (m_superview) {
    m_superview->removeSubview(this);
  }
}

void NSView::removeSubview(NSView *view) {
  auto it = std::find(m_subviews.begin(), m_subviews.end(), view);
  if (it != m_subviews.end()) {
    (*it)->m_superview = nullptr;
    m_subviews.erase(it);
    setNeedsDisplay(true);
  }
}

void NSView::setNeedsDisplay(bool flag) {
  m_needsDisplay = flag;
  if (flag && m_superview) {
    m_superview->setNeedsDisplay(true);
  }
}

void NSView::display() {
  if (m_needsDisplay) {
    drawRect(m_bounds);
    m_needsDisplay = false;
  }

  // Display subviews
  for (auto *subview : m_subviews) {
    if (!subview->isHidden()) {
      subview->display();
    }
  }
}

void NSView::setHidden(bool hidden) {
  m_hidden = hidden;
  setNeedsDisplay(true);
}

NSPoint NSView::convertPoint(const NSPoint &point, NSView *fromView) {
  // Simplified conversion - would need full coordinate transformation
  return point;
}
