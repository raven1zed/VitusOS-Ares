/**
 * OSFStackView.cpp - Stack-based Layout Container Implementation
 *
 * Phase 3: Layout & Input
 *
 * Implements flexible stack-based layout with spacing, alignment, and
 * distribution.
 */

#include <opensef/OSFStackView.h>

#include <algorithm>
#include <numeric>

namespace opensef {

// =============================================================================
// Construction
// =============================================================================

OSFStackView::OSFStackView(OSFStackAxis axis) : axis_(axis) {}

std::shared_ptr<OSFStackView> OSFStackView::create(OSFStackAxis axis) {
  return std::make_shared<OSFStackView>(axis);
}

// =============================================================================
// Layout
// =============================================================================

void OSFStackView::layoutSubviews() {
  const auto &children = subviews();
  if (children.empty())
    return;

  // Calculate available content area
  double contentX = paddingLeft_;
  double contentY = paddingTop_;
  double contentWidth = frame_.width - paddingLeft_ - paddingRight_;
  double contentHeight = frame_.height - paddingTop_ - paddingBottom_;

  // Total spacing between items
  double totalSpacing = spacing_ * (children.size() - 1);

  if (axis_ == OSFStackAxis::Horizontal) {
    // === Horizontal Layout ===
    double availableWidth = contentWidth - totalSpacing;

    // Calculate sizes based on distribution
    std::vector<double> widths(children.size());

    if (distribution_ == OSFStackDistribution::FillEqually) {
      double equalWidth = availableWidth / children.size();
      std::fill(widths.begin(), widths.end(), equalWidth);
    } else if (distribution_ == OSFStackDistribution::EqualSpacing) {
      // Use intrinsic sizes
      double totalIntrinsic = 0;
      for (size_t i = 0; i < children.size(); ++i) {
        OSFRect intrinsic = children[i]->intrinsicContentSize();
        widths[i] = intrinsic.width > 0 ? intrinsic.width : 50; // Fallback
        totalIntrinsic += widths[i];
      }
      // Distribute remaining space as spacing (override calculated spacing)
      // For now, just use intrinsic sizes as-is
    } else {
      // Fill: distribute proportionally (equal for now)
      double equalWidth = availableWidth / children.size();
      std::fill(widths.begin(), widths.end(), equalWidth);
    }

    // Position children
    double x = contentX;
    for (size_t i = 0; i < children.size(); ++i) {
      auto &child = children[i];

      double childY = contentY;
      double childHeight = contentHeight;

      if (alignment_ == OSFStackAlignment::Leading) {
        childHeight =
            std::min(childHeight, child->intrinsicContentSize().height);
        if (childHeight <= 0)
          childHeight = contentHeight;
      } else if (alignment_ == OSFStackAlignment::Trailing) {
        double intrinsicH = child->intrinsicContentSize().height;
        if (intrinsicH > 0) {
          childHeight = std::min(contentHeight, intrinsicH);
          childY = contentY + contentHeight - childHeight;
        }
      } else if (alignment_ == OSFStackAlignment::Center) {
        double intrinsicH = child->intrinsicContentSize().height;
        if (intrinsicH > 0) {
          childHeight = std::min(contentHeight, intrinsicH);
          childY = contentY + (contentHeight - childHeight) / 2;
        }
      }
      // Fill uses full height

      child->setFrame(OSFRect(x, childY, widths[i], childHeight));
      x += widths[i] + spacing_;
    }

  } else {
    // === Vertical Layout ===
    double availableHeight = contentHeight - totalSpacing;

    std::vector<double> heights(children.size());

    if (distribution_ == OSFStackDistribution::FillEqually) {
      double equalHeight = availableHeight / children.size();
      std::fill(heights.begin(), heights.end(), equalHeight);
    } else if (distribution_ == OSFStackDistribution::EqualSpacing) {
      for (size_t i = 0; i < children.size(); ++i) {
        OSFRect intrinsic = children[i]->intrinsicContentSize();
        heights[i] = intrinsic.height > 0 ? intrinsic.height : 30; // Fallback
      }
    } else {
      // Fill: equal for now
      double equalHeight = availableHeight / children.size();
      std::fill(heights.begin(), heights.end(), equalHeight);
    }

    // Position children
    double y = contentY;
    for (size_t i = 0; i < children.size(); ++i) {
      auto &child = children[i];

      double childX = contentX;
      double childWidth = contentWidth;

      if (alignment_ == OSFStackAlignment::Leading) {
        double intrinsicW = child->intrinsicContentSize().width;
        if (intrinsicW > 0) {
          childWidth = std::min(contentWidth, intrinsicW);
        }
      } else if (alignment_ == OSFStackAlignment::Trailing) {
        double intrinsicW = child->intrinsicContentSize().width;
        if (intrinsicW > 0) {
          childWidth = std::min(contentWidth, intrinsicW);
          childX = contentX + contentWidth - childWidth;
        }
      } else if (alignment_ == OSFStackAlignment::Center) {
        double intrinsicW = child->intrinsicContentSize().width;
        if (intrinsicW > 0) {
          childWidth = std::min(contentWidth, intrinsicW);
          childX = contentX + (contentWidth - childWidth) / 2;
        }
      }

      child->setFrame(OSFRect(childX, y, childWidth, heights[i]));
      y += heights[i] + spacing_;
    }
  }

  // Recursively layout children
  for (auto &child : children) {
    child->layoutIfNeeded();
  }
}

OSFRect OSFStackView::sizeThatFits(const OSFRect &proposedSize) {
  const auto &children = subviews();
  if (children.empty()) {
    return OSFRect(0, 0, paddingLeft_ + paddingRight_,
                   paddingTop_ + paddingBottom_);
  }

  double totalSpacing = spacing_ * (children.size() - 1);

  if (axis_ == OSFStackAxis::Horizontal) {
    double totalWidth = 0;
    double maxHeight = 0;
    for (const auto &child : children) {
      OSFRect intrinsic = child->intrinsicContentSize();
      totalWidth += intrinsic.width > 0 ? intrinsic.width : 50;
      maxHeight = std::max(maxHeight, intrinsic.height);
    }
    return OSFRect(0, 0,
                   paddingLeft_ + totalWidth + totalSpacing + paddingRight_,
                   paddingTop_ + maxHeight + paddingBottom_);
  } else {
    double totalHeight = 0;
    double maxWidth = 0;
    for (const auto &child : children) {
      OSFRect intrinsic = child->intrinsicContentSize();
      totalHeight += intrinsic.height > 0 ? intrinsic.height : 30;
      maxWidth = std::max(maxWidth, intrinsic.width);
    }
    return OSFRect(0, 0, paddingLeft_ + maxWidth + paddingRight_,
                   paddingTop_ + totalHeight + totalSpacing + paddingBottom_);
  }
}

OSFRect OSFStackView::intrinsicContentSize() const {
  // Delegate to sizeThatFits with zero proposed size
  return const_cast<OSFStackView *>(this)->sizeThatFits(OSFRect::Zero());
}

// =============================================================================
// Rendering
// =============================================================================

void OSFStackView::render(cairo_t *cr) {
  if (isHidden())
    return;

  // OSFStackView is typically transparent - just render children
  // Subclasses or users can override to add background

  // Render subviews
  OSFView::render(cr);
}

} // namespace opensef
