/**
 * OSFLayer.h - Animatable Layer (like CALayer)
 *
 * Provides implicit animations for UI properties.
 * When you change a layer property, it animates automatically.
 */

#pragma once

#include <cairo/cairo.h>
#include <memory>
#include <opensef/OSFAnimation.h>
#include <opensef/OSFGeometry.h>
#include <string>
#include <unordered_map>
#include <vector>


namespace opensef {

// Forward declarations
class OSFLayer;
using OSFLayerPtr = std::shared_ptr<OSFLayer>;

// =============================================================================
// OSFLayer - Animatable Layer
// =============================================================================

class OSFLayer : public std::enable_shared_from_this<OSFLayer> {
public:
  OSFLayer();
  virtual ~OSFLayer() = default;

  static OSFLayerPtr create();

  // =========================================================================
  // Animatable Properties
  // =========================================================================

  // Position (implicit animation on change)
  OSFPoint position() const { return position_; }
  void setPosition(const OSFPoint &pos);
  void setPosition(double x, double y) { setPosition({x, y}); }

  // Bounds (size relative to anchor)
  OSFRect bounds() const { return bounds_; }
  void setBounds(const OSFRect &bounds);

  // Opacity (0.0 to 1.0)
  double opacity() const { return opacity_; }
  void setOpacity(double opacity);

  // Scale
  double scaleX() const { return scaleX_; }
  double scaleY() const { return scaleY_; }
  void setScale(double scale) { setScale(scale, scale); }
  void setScale(double sx, double sy);

  // Rotation (radians)
  double rotation() const { return rotation_; }
  void setRotation(double radians);

  // Corner radius
  double cornerRadius() const { return cornerRadius_; }
  void setCornerRadius(double radius);

  // Background color
  OSFColor backgroundColor() const { return backgroundColor_; }
  void setBackgroundColor(const OSFColor &color);

  // Border
  double borderWidth() const { return borderWidth_; }
  OSFColor borderColor() const { return borderColor_; }
  void setBorderWidth(double width);
  void setBorderColor(const OSFColor &color);

  // Shadow
  double shadowOpacity() const { return shadowOpacity_; }
  double shadowRadius() const { return shadowRadius_; }
  OSFPoint shadowOffset() const { return shadowOffset_; }
  void setShadowOpacity(double opacity);
  void setShadowRadius(double radius);
  void setShadowOffset(const OSFPoint &offset);

  // =========================================================================
  // Layer Hierarchy
  // =========================================================================

  OSFLayer *parent() const { return parent_; }
  const std::vector<OSFLayerPtr> &sublayers() const { return sublayers_; }

  void addSublayer(OSFLayerPtr layer);
  void removeFromParent();
  void insertSublayer(OSFLayerPtr layer, size_t index);

  // =========================================================================
  // Rendering
  // =========================================================================

  // Render this layer and sublayers to Cairo context
  virtual void render(cairo_t *cr);

  // Override for custom drawing
  virtual void draw(cairo_t *cr);

  // =========================================================================
  // Animation Control
  // =========================================================================

  // Add explicit animation
  void addAnimation(std::shared_ptr<OSFAnimation> animation,
                    const std::string &key);

  // Remove animation by key
  void removeAnimation(const std::string &key);

  // Remove all animations
  void removeAllAnimations();

  // Disable implicit animations temporarily
  static void setDisableImplicitAnimations(bool disable);
  static bool implicitAnimationsDisabled();

protected:
  // Helper to create implicit animation for property change
  void animatePropertyChange(double *target, double from, double to);

private:
  // Animatable properties
  OSFPoint position_;
  OSFRect bounds_;
  double opacity_ = 1.0;
  double scaleX_ = 1.0;
  double scaleY_ = 1.0;
  double rotation_ = 0.0;
  double cornerRadius_ = 0.0;
  OSFColor backgroundColor_;
  double borderWidth_ = 0.0;
  OSFColor borderColor_;
  double shadowOpacity_ = 0.0;
  double shadowRadius_ = 0.0;
  OSFPoint shadowOffset_;

  // Hierarchy
  OSFLayer *parent_ = nullptr;
  std::vector<OSFLayerPtr> sublayers_;

  // Animations
  std::unordered_map<std::string, std::shared_ptr<OSFAnimation>> animations_;

  // Static flag for disabling implicit animations
  static bool disableImplicitAnimations_;
};

} // namespace opensef
