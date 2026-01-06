/**
 * OSFStackView.h - Stack-based Layout Container
 *
 * Phase 3: Layout & Input
 *
 * OSFStackView arranges its subviews in a horizontal or vertical stack.
 * Similar to UIStackView (iOS) or NSStackView (macOS).
 */

#pragma once

#include <opensef/OSFView.h>

namespace opensef {

/**
 * Stack axis determines arrangement direction.
 */
enum class OSFStackAxis {
  Horizontal, // Arrange left-to-right
  Vertical    // Arrange top-to-bottom
};

/**
 * Alignment along the cross-axis.
 */
enum class OSFStackAlignment {
  Fill,    // Stretch to fill cross-axis
  Leading, // Align to start (top for horizontal, left for vertical)
  Center,  // Center along cross-axis
  Trailing // Align to end (bottom for horizontal, right for vertical)
};

/**
 * Distribution along the main axis.
 */
enum class OSFStackDistribution {
  Fill,        // Subviews fill available space proportionally
  FillEqually, // All subviews get equal size
  EqualSpacing // Subviews use intrinsic size, spacing distributed equally
};

/**
 * OSFStackView - Flexible stack-based layout container
 *
 * Example usage:
 *   auto stack = std::make_shared<OSFStackView>(OSFStackAxis::Vertical);
 *   stack->setSpacing(8);
 *   stack->addSubview(label);
 *   stack->addSubview(button);
 */
class OSFStackView : public OSFView {
public:
  explicit OSFStackView(OSFStackAxis axis = OSFStackAxis::Vertical);
  ~OSFStackView() override = default;

  // === Configuration ===

  OSFStackAxis axis() const { return axis_; }
  void setAxis(OSFStackAxis axis) {
    axis_ = axis;
    setNeedsLayout();
  }

  double spacing() const { return spacing_; }
  void setSpacing(double spacing) {
    spacing_ = spacing;
    setNeedsLayout();
  }

  OSFStackAlignment alignment() const { return alignment_; }
  void setAlignment(OSFStackAlignment alignment) {
    alignment_ = alignment;
    setNeedsLayout();
  }

  OSFStackDistribution distribution() const { return distribution_; }
  void setDistribution(OSFStackDistribution distribution) {
    distribution_ = distribution;
    setNeedsLayout();
  }

  // === Padding ===

  void setPadding(double top, double right, double bottom, double left) {
    paddingTop_ = top;
    paddingRight_ = right;
    paddingBottom_ = bottom;
    paddingLeft_ = left;
    setNeedsLayout();
  }

  // === Layout Overrides ===

  void layoutSubviews() override;
  OSFRect sizeThatFits(const OSFRect &proposedSize) override;
  OSFRect intrinsicContentSize() const override;

  // === Rendering ===

  void render(cairo_t *cr) override;

  // === Factory ===

  static std::shared_ptr<OSFStackView>
  create(OSFStackAxis axis = OSFStackAxis::Vertical);

private:
  OSFStackAxis axis_ = OSFStackAxis::Vertical;
  double spacing_ = 0;
  OSFStackAlignment alignment_ = OSFStackAlignment::Fill;
  OSFStackDistribution distribution_ = OSFStackDistribution::Fill;

  double paddingTop_ = 0;
  double paddingRight_ = 0;
  double paddingBottom_ = 0;
  double paddingLeft_ = 0;
};

} // namespace opensef
