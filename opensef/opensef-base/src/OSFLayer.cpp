/**
 * OSFLayer.cpp - Animatable Layer Implementation
 */

#include <algorithm>
#include <cmath>
#include <opensef/OSFLayer.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace opensef {

bool OSFLayer::disableImplicitAnimations_ = false;

OSFLayer::OSFLayer() = default;

OSFLayerPtr OSFLayer::create() { return std::make_shared<OSFLayer>(); }

// =============================================================================
// Property Setters with Implicit Animation
// =============================================================================

void OSFLayer::animatePropertyChange(double *target, double from, double to) {
  if (disableImplicitAnimations_ || from == to) {
    *target = to;
    return;
  }

  auto anim = OSFPropertyAnimation::create(target, from, to);
  anim->setDuration(AnimationTiming::Quick);
  anim->setEasing(Easing::easeOut);
  anim->start();
  OSFAnimationManager::shared().addAnimation(anim);
}

void OSFLayer::setPosition(const OSFPoint &pos) {
  if (!disableImplicitAnimations_) {
    animatePropertyChange(&position_.x, position_.x, pos.x);
    animatePropertyChange(&position_.y, position_.y, pos.y);
  } else {
    position_ = pos;
  }
}

void OSFLayer::setBounds(const OSFRect &bounds) {
  if (!disableImplicitAnimations_) {
    animatePropertyChange(&bounds_.x, bounds_.x, bounds.x);
    animatePropertyChange(&bounds_.y, bounds_.y, bounds.y);
    animatePropertyChange(&bounds_.width, bounds_.width, bounds.width);
    animatePropertyChange(&bounds_.height, bounds_.height, bounds.height);
  } else {
    bounds_ = bounds;
  }
}

void OSFLayer::setOpacity(double opacity) {
  animatePropertyChange(&opacity_, opacity_, opacity);
}

void OSFLayer::setScale(double sx, double sy) {
  animatePropertyChange(&scaleX_, scaleX_, sx);
  animatePropertyChange(&scaleY_, scaleY_, sy);
}

void OSFLayer::setRotation(double radians) {
  animatePropertyChange(&rotation_, rotation_, radians);
}

void OSFLayer::setCornerRadius(double radius) {
  animatePropertyChange(&cornerRadius_, cornerRadius_, radius);
}

void OSFLayer::setBackgroundColor(const OSFColor &color) {
  animatePropertyChange(&backgroundColor_.r, backgroundColor_.r, color.r);
  animatePropertyChange(&backgroundColor_.g, backgroundColor_.g, color.g);
  animatePropertyChange(&backgroundColor_.b, backgroundColor_.b, color.b);
  animatePropertyChange(&backgroundColor_.a, backgroundColor_.a, color.a);
}

void OSFLayer::setBorderWidth(double width) {
  animatePropertyChange(&borderWidth_, borderWidth_, width);
}

void OSFLayer::setBorderColor(const OSFColor &color) {
  animatePropertyChange(&borderColor_.r, borderColor_.r, color.r);
  animatePropertyChange(&borderColor_.g, borderColor_.g, color.g);
  animatePropertyChange(&borderColor_.b, borderColor_.b, color.b);
  animatePropertyChange(&borderColor_.a, borderColor_.a, color.a);
}

void OSFLayer::setShadowOpacity(double opacity) {
  animatePropertyChange(&shadowOpacity_, shadowOpacity_, opacity);
}

void OSFLayer::setShadowRadius(double radius) {
  animatePropertyChange(&shadowRadius_, shadowRadius_, radius);
}

void OSFLayer::setShadowOffset(const OSFPoint &offset) {
  animatePropertyChange(&shadowOffset_.x, shadowOffset_.x, offset.x);
  animatePropertyChange(&shadowOffset_.y, shadowOffset_.y, offset.y);
}

// =============================================================================
// Layer Hierarchy
// =============================================================================

void OSFLayer::addSublayer(OSFLayerPtr layer) {
  if (layer->parent_) {
    layer->removeFromParent();
  }
  layer->parent_ = this;
  sublayers_.push_back(layer);
}

void OSFLayer::removeFromParent() {
  if (!parent_)
    return;

  auto &siblings = parent_->sublayers_;
  auto it = std::remove(siblings.begin(), siblings.end(), shared_from_this());
  siblings.erase(it, siblings.end());
  parent_ = nullptr;
}

void OSFLayer::insertSublayer(OSFLayerPtr layer, size_t index) {
  if (layer->parent_) {
    layer->removeFromParent();
  }
  layer->parent_ = this;
  if (index >= sublayers_.size()) {
    sublayers_.push_back(layer);
  } else {
    sublayers_.insert(sublayers_.begin() + index, layer);
  }
}

// =============================================================================
// Rendering
// =============================================================================

void OSFLayer::render(cairo_t *cr) {
  cairo_save(cr);

  // Apply transforms
  cairo_translate(cr, position_.x, position_.y);

  if (scaleX_ != 1.0 || scaleY_ != 1.0) {
    cairo_translate(cr, bounds_.width / 2, bounds_.height / 2);
    cairo_scale(cr, scaleX_, scaleY_);
    cairo_translate(cr, -bounds_.width / 2, -bounds_.height / 2);
  }

  if (rotation_ != 0.0) {
    cairo_translate(cr, bounds_.width / 2, bounds_.height / 2);
    cairo_rotate(cr, rotation_);
    cairo_translate(cr, -bounds_.width / 2, -bounds_.height / 2);
  }

  // Apply opacity
  cairo_push_group(cr);

  // Draw shadow
  if (shadowOpacity_ > 0.0 && shadowRadius_ > 0.0) {
    cairo_set_source_rgba(cr, 0, 0, 0, shadowOpacity_ * 0.5);
    double sr = cornerRadius_;
    double sx = bounds_.x + shadowOffset_.x;
    double sy = bounds_.y + shadowOffset_.y;
    double sw = bounds_.width;
    double sh = bounds_.height;

    cairo_new_sub_path(cr);
    cairo_arc(cr, sx + sw - sr, sy + sr, sr + shadowRadius_, -M_PI / 2, 0);
    cairo_arc(cr, sx + sw - sr, sy + sh - sr, sr + shadowRadius_, 0, M_PI / 2);
    cairo_arc(cr, sx + sr, sy + sh - sr, sr + shadowRadius_, M_PI / 2, M_PI);
    cairo_arc(cr, sx + sr, sy + sr, sr + shadowRadius_, M_PI, 3 * M_PI / 2);
    cairo_close_path(cr);
    cairo_fill(cr);
  }

  // Draw background
  if (backgroundColor_.a > 0.0) {
    backgroundColor_.setCairo(cr);

    if (cornerRadius_ > 0.0) {
      double r = cornerRadius_;
      cairo_new_sub_path(cr);
      cairo_arc(cr, bounds_.x + bounds_.width - r, bounds_.y + r, r, -M_PI / 2,
                0);
      cairo_arc(cr, bounds_.x + bounds_.width - r,
                bounds_.y + bounds_.height - r, r, 0, M_PI / 2);
      cairo_arc(cr, bounds_.x + r, bounds_.y + bounds_.height - r, r, M_PI / 2,
                M_PI);
      cairo_arc(cr, bounds_.x + r, bounds_.y + r, r, M_PI, 3 * M_PI / 2);
      cairo_close_path(cr);
    } else {
      cairo_rectangle(cr, bounds_.x, bounds_.y, bounds_.width, bounds_.height);
    }
    cairo_fill(cr);
  }

  // Custom drawing
  draw(cr);

  // Draw border
  if (borderWidth_ > 0.0 && borderColor_.a > 0.0) {
    borderColor_.setCairo(cr);
    cairo_set_line_width(cr, borderWidth_);

    if (cornerRadius_ > 0.0) {
      double r = cornerRadius_;
      cairo_new_sub_path(cr);
      cairo_arc(cr, bounds_.x + bounds_.width - r, bounds_.y + r, r, -M_PI / 2,
                0);
      cairo_arc(cr, bounds_.x + bounds_.width - r,
                bounds_.y + bounds_.height - r, r, 0, M_PI / 2);
      cairo_arc(cr, bounds_.x + r, bounds_.y + bounds_.height - r, r, M_PI / 2,
                M_PI);
      cairo_arc(cr, bounds_.x + r, bounds_.y + r, r, M_PI, 3 * M_PI / 2);
      cairo_close_path(cr);
    } else {
      cairo_rectangle(cr, bounds_.x, bounds_.y, bounds_.width, bounds_.height);
    }
    cairo_stroke(cr);
  }

  // Draw sublayers
  for (auto &sublayer : sublayers_) {
    sublayer->render(cr);
  }

  // Apply opacity
  cairo_pop_group_to_source(cr);
  cairo_paint_with_alpha(cr, opacity_);

  cairo_restore(cr);
}

void OSFLayer::draw(cairo_t *cr) { (void)cr; }

// =============================================================================
// Animation Management
// =============================================================================

void OSFLayer::addAnimation(std::shared_ptr<OSFAnimation> animation,
                            const std::string &key) {
  removeAnimation(key);
  animations_[key] = animation;
  animation->start();
  OSFAnimationManager::shared().addAnimation(animation);
}

void OSFLayer::removeAnimation(const std::string &key) {
  auto it = animations_.find(key);
  if (it != animations_.end()) {
    it->second->stop();
    animations_.erase(it);
  }
}

void OSFLayer::removeAllAnimations() {
  for (auto &[key, anim] : animations_) {
    anim->stop();
  }
  animations_.clear();
}

void OSFLayer::setDisableImplicitAnimations(bool disable) {
  disableImplicitAnimations_ = disable;
}

bool OSFLayer::implicitAnimationsDisabled() {
  return disableImplicitAnimations_;
}

} // namespace opensef
