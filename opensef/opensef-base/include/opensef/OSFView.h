/**
 * OSFView.h - Base class for all visual elements
 * Part of opensef-base foundation
 *
 * Phase 3: Inherits from OSFResponder + Layout Support
 */

#pragma once

#include <cairo/cairo.h>
#include <functional>
#include <memory>
#include <opensef/OSFGeometry.h>
#include <opensef/OSFResponder.h>
#include <vector>

namespace opensef {

class OSFWindow;
class OSFLayer;
using OSFLayerPtr = std::shared_ptr<OSFLayer>;

class OSFView : public OSFResponder {
public:
  OSFView();
  virtual ~OSFView() = default;

  // === Layer Support ===
  OSFLayerPtr layer() const { return layer_; }

  // === Geometry ===

  OSFRect frame() const { return frame_; }
  void setFrame(const OSFRect &frame);

  OSFRect bounds() const { return OSFRect(0, 0, frame_.width, frame_.height); }

  // === Visibility ===

  double alpha() const { return alpha_; }
  void setAlpha(double alpha);
  bool isHidden() const { return hidden_; }
  void setHidden(bool hidden) { hidden_ = hidden; }

  // === Hierarchy ===

  OSFView *superview() const { return superview_; }
  const std::vector<std::shared_ptr<OSFView>> &subviews() const {
    return subviews_;
  }
  void addSubview(std::shared_ptr<OSFView> view);
  void removeFromSuperview();

  // Get the window this view belongs to
  OSFWindow *window() const { return window_; }
  void setWindow(OSFWindow *window);

  // === Layout System ===

  void setNeedsLayout() { needsLayout_ = true; }
  bool needsLayout() const { return needsLayout_; }

  virtual void layoutSubviews() {}
  void layoutIfNeeded();

  /**
   * Returns the size that best fits the proposed size.
   * Override in subclasses to provide intrinsic sizing.
   * Default returns the proposed size (flexible).
   */
  virtual OSFRect sizeThatFits(const OSFRect &proposedSize) {
    return proposedSize;
  }

  /**
   * Returns the natural size of this view's content.
   * Override in widgets (buttons, labels) to return their preferred size.
   * Default returns zero (no intrinsic size).
   */
  virtual OSFRect intrinsicContentSize() const { return OSFRect(0, 0, 0, 0); }

  /**
   * Mark the view as needing a redraw.
   */
  void setNeedsDisplay();

  // === Display ===
  bool needsDisplay() const { return needsDisplay_; }

  // === Hit Testing ===

  virtual OSFView *hitTest(double x, double y);
  void convertPoint(double &x, double &y, OSFView *toView);

  // === Responder Chain ===

  OSFResponder *nextResponder() const override;

  bool acceptsFirstResponder() const override { return acceptsFirstResponder_; }
  void setAcceptsFirstResponder(bool accepts) {
    acceptsFirstResponder_ = accepts;
  }

  bool mouseDown(OSFEvent &event) override;
  bool mouseUp(OSFEvent &event) override;
  bool keyDown(OSFEvent &event) override;

  // === Rendering ===

  virtual void draw() {}
  virtual void render(cairo_t *cr);

protected:
  OSFLayerPtr layer_;
  OSFRect frame_;
  double alpha_ = 1.0;
  bool hidden_ = false;
  bool needsLayout_ = false;
  bool needsDisplay_ = false;
  bool acceptsFirstResponder_ = false;

  OSFView *superview_ = nullptr;
  OSFWindow *window_ = nullptr;
  std::vector<std::shared_ptr<OSFView>> subviews_;
};

} // namespace opensef
