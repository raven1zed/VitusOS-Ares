/**
 * OSFLayer.h - Scene Graph Layer with Implicit Animations
 *
 * Based on Core Animation CALayer pattern from R&D research.
 * Implements model/presentation tree split for smooth animations.
 *
 * Key concepts:
 * - Model values: What you SET (target state)
 * - Presentation values: What you SEE (current animated state)
 * - Implicit animations: Property changes automatically animate
 *
 * Usage:
 *   auto layer = std::make_unique<OSFLayer>(parentTree);
 *   layer->setOpacity(0.5f);   // Animates from current to 0.5
 *   layer->setPosition(100, 200);  // Animates to new position
 */

#pragma once

extern "C" {
#include <wlr/types/wlr_scene.h>
}

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>

namespace opensef {

// Forward declarations
class OSFPropertyAnimation;

/**
 * OSFLayer - A composable layer with implicit animation support
 */
class OSFLayer {
public:
  OSFLayer(wlr_scene_tree *parent);
  ~OSFLayer();

  // === Geometry (with implicit animation) ===

  void setPosition(float x, float y);
  float positionX() const;
  float positionY() const;

  // Presentation values (current animated state)
  float presentationX() const;
  float presentationY() const;

  void setSize(int width, int height);
  int width() const { return width_; }
  int height() const { return height_; }

  // === Visual Properties (with implicit animation) ===

  void setOpacity(float opacity);
  float opacity() const;
  float presentationOpacity() const;

  void setBackgroundColor(uint32_t color);
  uint32_t backgroundColor() const { return backgroundColor_; }

  // === Visibility ===

  void setHidden(bool hidden);
  bool isHidden() const { return hidden_; }

  // === Animation Control ===

  /**
   * Update animations. Call this every frame.
   * Returns true if any animations are still running.
   */
  bool updateAnimations(float dt);

  /**
   * Check if layer has active animations.
   */
  bool isAnimating() const;

  /**
   * Remove all animations immediately.
   */
  void removeAllAnimations();

  // === Scene Graph ===

  wlr_scene_tree *sceneTree() const { return tree_; }
  wlr_scene_rect *sceneRect() const { return rect_; }

  // === Sublayers ===

  void addSublayer(std::shared_ptr<OSFLayer> sublayer);
  void removeFromParent();

private:
  // Scene graph nodes
  wlr_scene_tree *tree_ = nullptr;
  wlr_scene_rect *rect_ = nullptr;

  // Model values (target state)
  float modelX_ = 0.0f;
  float modelY_ = 0.0f;
  int width_ = 0;
  int height_ = 0;
  float modelOpacity_ = 1.0f;
  uint32_t backgroundColor_ = 0x000000;
  bool hidden_ = false;

  // Presentation values (current animated state)
  float presentationX_ = 0.0f;
  float presentationY_ = 0.0f;
  float presentationOpacity_ = 1.0f;

  // Active animations
  std::map<std::string, std::unique_ptr<OSFPropertyAnimation>> animations_;

  // Sublayers
  std::vector<std::shared_ptr<OSFLayer>> sublayers_;
  OSFLayer *parent_ = nullptr;

  // Animation helpers
  void animateProperty(const std::string &key, float from, float to);
  void applyPresentationValues();
};

/**
 * OSFPropertyAnimation - Animates a single float property
 */
class OSFPropertyAnimation {
public:
  OSFPropertyAnimation(float duration, float from, float to);

  void start();
  bool update(float dt); // Returns true if still animating
  float currentValue() const;
  bool isAnimating() const { return !finished_; }

  // Animation curve (from R&D timing functions)
  enum class Curve { Linear, EaseIn, EaseOut, EaseInOut, Spring };
  void setCurve(Curve curve) { curve_ = curve; }

private:
  float duration_;
  float elapsed_ = 0.0f;
  float fromValue_;
  float toValue_;
  bool finished_ = false;
  Curve curve_ = Curve::EaseInOut;

  float applyCurve(float t) const;
};

} // namespace opensef
