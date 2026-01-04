/**
 * OSFView.cpp - Base View Implementation
 * Part of opensef-base foundation
 *
 * Implements responder chain, layout system, and hit testing.
 */

#include <algorithm>
#include <opensef/OSFView.h>
#include <opensef/OSFWindow.h>


namespace opensef {

// =============================================================================
// OSFView - Hierarchy
// =============================================================================

void OSFView::addSubview(std::shared_ptr<OSFView> view) {
  if (view->superview_) {
    view->removeFromSuperview();
  }
  view->superview_ = this;
  view->setWindow(window_); // Propagate window reference
  subviews_.push_back(view);
  setNeedsLayout();
}

void OSFView::removeFromSuperview() {
  if (!superview_)
    return;

  auto &siblings = superview_->subviews_;
  siblings.erase(std::remove_if(siblings.begin(), siblings.end(),
                                [this](const std::shared_ptr<OSFView> &v) {
                                  return v.get() == this;
                                }),
                 siblings.end());
  superview_ = nullptr;
  window_ = nullptr;
}

void OSFView::setWindow(OSFWindow *window) {
  window_ = window;
  // Propagate to subviews
  for (auto &subview : subviews_) {
    subview->setWindow(window);
  }
}

// =============================================================================
// OSFView - Layout System
// =============================================================================

void OSFView::layoutIfNeeded() {
  if (needsLayout_) {
    layoutSubviews();
    needsLayout_ = false;
  }

  // Layout subviews recursively
  for (auto &subview : subviews_) {
    subview->layoutIfNeeded();
  }
}

// =============================================================================
// OSFView - Hit Testing
// =============================================================================

OSFView *OSFView::hitTest(double x, double y) {
  if (hidden_ || alpha_ <= 0.01) {
    return nullptr;
  }

  // Check if point is within bounds
  if (!bounds().contains(x, y)) {
    return nullptr;
  }

  // Check subviews in reverse order (front to back)
  for (auto it = subviews_.rbegin(); it != subviews_.rend(); ++it) {
    auto &subview = *it;
    // Convert point to subview's coordinate system
    double subX = x - subview->frame_.x;
    double subY = y - subview->frame_.y;

    OSFView *hit = subview->hitTest(subX, subY);
    if (hit) {
      return hit;
    }
  }

  // No subview hit, return self
  return this;
}

void OSFView::convertPoint(double &x, double &y, OSFView *toView) {
  // Convert to window coordinates first
  OSFView *current = this;
  while (current) {
    x += current->frame_.x;
    y += current->frame_.y;
    current = current->superview_;
  }

  // Then convert from window coordinates to target view
  if (toView) {
    std::vector<OSFView *> ancestors;
    current = toView;
    while (current) {
      ancestors.push_back(current);
      current = current->superview_;
    }

    for (auto it = ancestors.rbegin(); it != ancestors.rend(); ++it) {
      x -= (*it)->frame_.x;
      y -= (*it)->frame_.y;
    }
  }
}

// =============================================================================
// OSFView - Responder Chain
// =============================================================================

OSFResponder *OSFView::nextResponder() const {
  // Next responder is superview, or window if no superview
  if (superview_) {
    return superview_;
  }
  return window_;
}

bool OSFView::mouseDown(OSFEvent &event) {
  // Default: try to become first responder if we accept it
  if (acceptsFirstResponder_ && window_) {
    window_->makeFirstResponder(this);
  }
  // Forward to next responder
  return OSFResponder::mouseDown(event);
}

bool OSFView::mouseUp(OSFEvent &event) { return OSFResponder::mouseUp(event); }

bool OSFView::keyDown(OSFEvent &event) { return OSFResponder::keyDown(event); }

// =============================================================================
// OSFView - Rendering
// =============================================================================

void OSFView::render(cairo_t *cr) {
  if (hidden_)
    return;

  // Render subviews
  for (auto &subview : subviews_) {
    cairo_save(cr);
    cairo_translate(cr, subview->frame_.x, subview->frame_.y);
    subview->render(cr);
    cairo_restore(cr);
  }
}

} // namespace opensef
