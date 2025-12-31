/**
 * OSFLayer.cpp - Scene Graph Layer Implementation
 */

#include "OSFLayer.h"
#include "OSFAresTheme.h"
#include "OSFTransaction.h"


#include <algorithm>
#include <cmath>
#include <iostream>

namespace opensef {

// ============================================================================
// OSFPropertyAnimation Implementation
// ============================================================================

OSFPropertyAnimation::OSFPropertyAnimation(float duration, float from, float to)
    : duration_(duration), fromValue_(from), toValue_(to) {}

void OSFPropertyAnimation::start() {
  elapsed_ = 0.0f;
  finished_ = false;
}

bool OSFPropertyAnimation::update(float dt) {
  if (finished_)
    return false;

  elapsed_ += dt;
  if (elapsed_ >= duration_) {
    elapsed_ = duration_;
    finished_ = true;
  }

  return !finished_;
}

float OSFPropertyAnimation::currentValue() const {
  if (finished_)
    return toValue_;
  if (duration_ <= 0.0f)
    return toValue_;

  float t = elapsed_ / duration_;
  t = std::clamp(t, 0.0f, 1.0f);
  t = applyCurve(t);

  return fromValue_ + (toValue_ - fromValue_) * t;
}

float OSFPropertyAnimation::applyCurve(float t) const {
  // Animation timing functions from R&D research (Part 2.4)
  switch (curve_) {
  case Curve::Linear:
    return t;

  case Curve::EaseIn:
    return t * t * t;

  case Curve::EaseOut:
    return 1.0f - std::pow(1.0f - t, 3.0f);

  case Curve::EaseInOut:
    return t < 0.5f ? 4.0f * t * t * t
                    : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;

  case Curve::Spring: {
    // Damped spring oscillation
    float omega = 10.0f;
    float zeta = 0.5f;
    return 1.0f - std::exp(-zeta * omega * t) *
                      std::cos(omega * std::sqrt(1.0f - zeta * zeta) * t);
  }
  }
  return t;
}

// ============================================================================
// OSFLayer Implementation
// ============================================================================

OSFLayer::OSFLayer(wlr_scene_tree *parent) {
  if (parent) {
    tree_ = wlr_scene_tree_create(parent);
  }
}

OSFLayer::~OSFLayer() {
  removeAllAnimations();
  sublayers_.clear();

  if (rect_) {
    wlr_scene_node_destroy(&rect_->node);
    rect_ = nullptr;
  }
  if (tree_) {
    wlr_scene_node_destroy(&tree_->node);
    tree_ = nullptr;
  }
}

// === Position ===

void OSFLayer::setPosition(float x, float y) {
  if (OSFTransaction::animationsDisabled()) {
    // Instant change
    modelX_ = presentationX_ = x;
    modelY_ = presentationY_ = y;
    applyPresentationValues();
    return;
  }

  // Create implicit animations
  if (x != modelX_) {
    animateProperty("positionX", presentationX_, x);
  }
  if (y != modelY_) {
    animateProperty("positionY", presentationY_, y);
  }

  modelX_ = x;
  modelY_ = y;
}

float OSFLayer::positionX() const { return modelX_; }
float OSFLayer::positionY() const { return modelY_; }
float OSFLayer::presentationX() const { return presentationX_; }
float OSFLayer::presentationY() const { return presentationY_; }

// === Size ===

void OSFLayer::setSize(int width, int height) {
  width_ = width;
  height_ = height;

  if (rect_) {
    wlr_scene_rect_set_size(rect_, width, height);
  }
}

// === Opacity ===

void OSFLayer::setOpacity(float opacity) {
  opacity = std::clamp(opacity, 0.0f, 1.0f);

  if (OSFTransaction::animationsDisabled()) {
    modelOpacity_ = presentationOpacity_ = opacity;
    applyPresentationValues();
    return;
  }

  if (opacity != modelOpacity_) {
    animateProperty("opacity", presentationOpacity_, opacity);
  }
  modelOpacity_ = opacity;
}

float OSFLayer::opacity() const { return modelOpacity_; }
float OSFLayer::presentationOpacity() const { return presentationOpacity_; }

// === Background Color ===

void OSFLayer::setBackgroundColor(uint32_t color) {
  backgroundColor_ = color;

  float rgba[4];
  AresTheme::hexToRGBA(color, rgba, presentationOpacity_);

  if (!rect_ && tree_) {
    rect_ = wlr_scene_rect_create(tree_, width_, height_, rgba);
  } else if (rect_) {
    wlr_scene_rect_set_color(rect_, rgba);
  }
}

// === Visibility ===

void OSFLayer::setHidden(bool hidden) {
  hidden_ = hidden;
  if (tree_) {
    wlr_scene_node_set_enabled(&tree_->node, !hidden);
  }
}

// === Animation ===

void OSFLayer::animateProperty(const std::string &key, float from, float to) {
  float duration = OSFTransaction::animationDuration();
  auto anim = std::make_unique<OSFPropertyAnimation>(duration, from, to);

  // Set curve based on transaction
  switch (OSFTransaction::timingFunction()) {
  case OSFTransaction::TimingFunction::Linear:
    anim->setCurve(OSFPropertyAnimation::Curve::Linear);
    break;
  case OSFTransaction::TimingFunction::EaseIn:
    anim->setCurve(OSFPropertyAnimation::Curve::EaseIn);
    break;
  case OSFTransaction::TimingFunction::EaseOut:
    anim->setCurve(OSFPropertyAnimation::Curve::EaseOut);
    break;
  case OSFTransaction::TimingFunction::Spring:
    anim->setCurve(OSFPropertyAnimation::Curve::Spring);
    break;
  default:
    anim->setCurve(OSFPropertyAnimation::Curve::EaseInOut);
    break;
  }

  anim->start();
  animations_[key] = std::move(anim);
}

bool OSFLayer::updateAnimations(float dt) {
  bool anyAnimating = false;

  for (auto &[key, anim] : animations_) {
    if (anim && anim->update(dt)) {
      anyAnimating = true;
    }

    // Update presentation values
    if (anim) {
      if (key == "positionX") {
        presentationX_ = anim->currentValue();
      } else if (key == "positionY") {
        presentationY_ = anim->currentValue();
      } else if (key == "opacity") {
        presentationOpacity_ = anim->currentValue();
      }
    }
  }

  if (anyAnimating) {
    applyPresentationValues();
  }

  // Remove finished animations
  for (auto it = animations_.begin(); it != animations_.end();) {
    if (it->second && !it->second->isAnimating()) {
      it = animations_.erase(it);
    } else {
      ++it;
    }
  }

  // Update sublayer animations
  for (auto &sublayer : sublayers_) {
    if (sublayer->updateAnimations(dt)) {
      anyAnimating = true;
    }
  }

  return anyAnimating;
}

bool OSFLayer::isAnimating() const {
  for (const auto &[key, anim] : animations_) {
    if (anim && anim->isAnimating()) {
      return true;
    }
  }
  return false;
}

void OSFLayer::removeAllAnimations() {
  animations_.clear();

  // Snap to model values
  presentationX_ = modelX_;
  presentationY_ = modelY_;
  presentationOpacity_ = modelOpacity_;
  applyPresentationValues();
}

void OSFLayer::applyPresentationValues() {
  if (tree_) {
    wlr_scene_node_set_position(&tree_->node, static_cast<int>(presentationX_),
                                static_cast<int>(presentationY_));
  }

  if (rect_) {
    float rgba[4];
    AresTheme::hexToRGBA(backgroundColor_, rgba, presentationOpacity_);
    wlr_scene_rect_set_color(rect_, rgba);
  }
}

// === Sublayers ===

void OSFLayer::addSublayer(std::shared_ptr<OSFLayer> sublayer) {
  sublayer->parent_ = this;
  sublayers_.push_back(sublayer);
}

void OSFLayer::removeFromParent() {
  if (parent_) {
    auto &siblings = parent_->sublayers_;
    siblings.erase(std::remove_if(siblings.begin(), siblings.end(),
                                  [this](const std::shared_ptr<OSFLayer> &l) {
                                    return l.get() == this;
                                  }),
                   siblings.end());
    parent_ = nullptr;
  }
}

} // namespace opensef
