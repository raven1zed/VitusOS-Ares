/**
 * openSEF AppKit: Production GUI Widgets
 *
 * Phase 3: Responder chain + Layout system integration
 * Cairo/Pango-based widgets for VitusOS Ares
 * Integrated with OSFLayer animation framework
 */

#pragma once

#include <cairo/cairo.h>
#include <cstdint>
#include <functional>
#include <memory>
#include <opensef/OSFResponder.h>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>


// Forward declare Pango types
typedef struct _PangoLayout PangoLayout;

namespace opensef {

// Forward declarations
class OSFWindow;

// Global Vulkan state access for widgets (AppKit/Compositor integration)
// The application must set this before calling draw() on widgets using Vulkan.
extern VkCommandBuffer g_CurrentVulkanCommandBuffer;

/**
 * Initialize the AppKit Vulkan subsystem.
 * Must be called once after Vulkan initialization.
 */
void OSFAppKitInitializeVulkan(VkDevice device, VkPhysicalDevice physicalDevice,
                               VkQueue queue, VkCommandPool commandPool,
                               VkRenderPass renderPass);

// =============================================================================
// Geometry Types
// =============================================================================

struct OSFRect {
  double x = 0, y = 0, width = 0, height = 0;
  OSFRect() = default;
  OSFRect(double x_, double y_, double w, double h)
      : x(x_), y(y_), width(w), height(h) {}

  static OSFRect Zero() { return OSFRect(0, 0, 0, 0); }

  bool contains(double px, double py) const {
    return px >= x && px < x + width && py >= y && py < y + height;
  }
};

struct OSFColor {
  double r = 0, g = 0, b = 0, a = 1;
  OSFColor() = default;
  OSFColor(double r_, double g_, double b_, double a_ = 1.0)
      : r(r_), g(g_), b(b_), a(a_) {}

  static OSFColor fromHex(uint32_t hex, double alpha = 1.0) {
    return OSFColor(((hex >> 16) & 0xFF) / 255.0, ((hex >> 8) & 0xFF) / 255.0,
                    (hex & 0xFF) / 255.0, alpha);
  }

  static OSFColor fromARGB(uint32_t argb) {
    return OSFColor(((argb >> 16) & 0xFF) / 255.0, ((argb >> 8) & 0xFF) / 255.0,
                    (argb & 0xFF) / 255.0, ((argb >> 24) & 0xFF) / 255.0);
  }

  void setCairo(cairo_t *cr) const { cairo_set_source_rgba(cr, r, g, b, a); }
};

// =============================================================================
// Font/Text Types
// =============================================================================

enum class FontWeight { Normal, Medium, Bold };
enum class TextAlignment { Left, Center, Right };

// =============================================================================
// OSFView - Base class for all visual elements (Phase 3: Inherits OSFResponder)
// =============================================================================

class OSFView : public OSFResponder {
public:
  OSFView() = default;
  virtual ~OSFView() = default;

  // === Geometry ===

  OSFRect frame() const { return frame_; }
  void setFrame(const OSFRect &frame) {
    frame_ = frame;
    setNeedsLayout();
  }

  OSFRect bounds() const { return OSFRect(0, 0, frame_.width, frame_.height); }

  // === Visibility ===

  double alpha() const { return alpha_; }
  void setAlpha(double alpha) { alpha_ = alpha; }
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

  // === Layout System (Phase 3) ===

  /**
   * Mark this view as needing layout.
   * Layout will be performed before the next render.
   */
  void setNeedsLayout() { needsLayout_ = true; }

  /**
   * Returns true if this view needs layout.
   */
  bool needsLayout() const { return needsLayout_; }

  /**
   * Override to perform custom layout of subviews.
   * Called when the view's frame changes or setNeedsLayout() was called.
   */
  virtual void layoutSubviews() {}

  /**
   * Performs layout if needed, then layouts subviews recursively.
   */
  void layoutIfNeeded();

  // === Display ===

  /**
   * Mark this view as needing redraw.
   */
  void setNeedsDisplay() { needsDisplay_ = true; }
  bool needsDisplay() const { return needsDisplay_; }

  // === Hit Testing (Phase 3) ===

  /**
   * Returns the deepest view that contains the point.
   * Point is in the view's local coordinate system.
   */
  virtual OSFView *hitTest(double x, double y);

  /**
   * Convert a point from this view's coordinate system to another view's.
   */
  void convertPoint(double &x, double &y, OSFView *toView);

  // === Responder Chain (Phase 3) ===

  /**
   * Next responder is the superview, or the window if no superview.
   */
  OSFResponder *nextResponder() const override;

  /**
   * Views can become first responder if they're focusable.
   */
  bool acceptsFirstResponder() const override { return acceptsFirstResponder_; }
  void setAcceptsFirstResponder(bool accepts) {
    acceptsFirstResponder_ = accepts;
  }

  // Event handling overrides
  bool mouseDown(OSFEvent &event) override;
  bool mouseUp(OSFEvent &event) override;
  bool keyDown(OSFEvent &event) override;

  // === Rendering ===

  virtual void draw() {}
  virtual void render(cairo_t *cr);

protected:
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

// =============================================================================
// OSFButton - Clickable button with Cairo rendering
// =============================================================================

class OSFButton : public OSFView {
public:
  OSFButton();
  OSFButton(const std::string &label, std::function<void()> action = nullptr);
  virtual ~OSFButton() = default;

  static std::shared_ptr<OSFButton>
  create(const std::string &label, std::function<void()> action = nullptr);

  // Properties
  const std::string &label() const { return label_; }
  void setLabel(const std::string &label) { label_ = label; }
  void setAction(std::function<void()> action) { action_ = action; }

  // State
  bool isHovered() const { return hovered_; }
  bool isPressed() const { return pressed_; }

  // Events
  void click();
  void handleMouseEnter();
  void handleMouseLeave();
  void handleMouseDown();
  void handleMouseUp();

  // OSFResponder events (Phase 3)
  bool mouseDown(OSFEvent &event) override;
  bool mouseUp(OSFEvent &event) override;
  bool acceptsFirstResponder() const override { return true; }

  // Rendering
  void render(cairo_t *cr) override;

private:
  std::string label_;
  std::function<void()> action_;
  bool hovered_ = false;
  bool pressed_ = false;
};

// =============================================================================
// OSFLabel - Text display with Pango rendering
// =============================================================================

class OSFLabel : public OSFView {
public:
  OSFLabel();
  OSFLabel(const std::string &text);
  virtual ~OSFLabel() = default;

  static std::shared_ptr<OSFLabel> create(const std::string &text);
  static std::shared_ptr<OSFLabel> createTitle(const std::string &text);
  static std::shared_ptr<OSFLabel> createSubtitle(const std::string &text);

  // Properties
  const std::string &text() const { return text_; }
  void setText(const std::string &text) { text_ = text; }

  OSFColor textColor() const { return textColor_; }
  void setTextColor(const OSFColor &color) { textColor_ = color; }

  double fontSize() const { return fontSize_; }
  void setFontSize(double size);

  FontWeight fontWeight() const { return fontWeight_; }
  void setFontWeight(FontWeight weight);

  TextAlignment alignment() const { return alignment_; }
  void setAlignment(TextAlignment alignment);

  // Rendering
  void render(cairo_t *cr) override;
  void draw() override;

private:
  std::string text_;
  OSFColor textColor_{1.0, 1.0, 1.0, 1.0}; // White default
  double fontSize_ = 13.0;
  FontWeight fontWeight_ = FontWeight::Normal;
  TextAlignment alignment_ = TextAlignment::Left;
};

// =============================================================================
// OSFTextField - Text input
// =============================================================================

class OSFTextField : public OSFView {
public:
  OSFTextField();
  OSFTextField(const std::string &placeholder);
  virtual ~OSFTextField() = default;

  static std::shared_ptr<OSFTextField>
  create(const std::string &placeholder = "");

  // Properties
  const std::string &text() const { return text_; }
  void setText(const std::string &text) { text_ = text; }
  const std::string &placeholder() const { return placeholder_; }
  void setPlaceholder(const std::string &placeholder) {
    placeholder_ = placeholder;
  }
  void setSecure(bool secure);

  // Events
  void handleKeyPress(uint32_t key, const std::string &text);
  void handleFocus(bool focused);
  void setOnTextChanged(std::function<void(const std::string &)> callback);
  void setOnSubmit(std::function<void(const std::string &)> callback);

  // OSFResponder overrides (Phase 3)
  bool acceptsFirstResponder() const override { return true; }
  bool keyDown(OSFEvent &event) override;
  void becomeFirstResponder() override;
  void resignFirstResponder() override;

  // Rendering
  void render(cairo_t *cr) override;

private:
  std::string text_;
  std::string placeholder_;
  bool focused_ = false;
  bool cursorVisible_ = true;
  bool isSecure_ = false;
  std::function<void(const std::string &)> onTextChanged_;
  std::function<void(const std::string &)> onSubmit_;
};

// =============================================================================
// OSFGlassPanel - Frosted glass effect (Ares signature)
// =============================================================================

class OSFGlassPanel : public OSFView {
public:
  OSFGlassPanel();
  virtual ~OSFGlassPanel() = default;

  static std::shared_ptr<OSFGlassPanel> create(const OSFRect &frame);

  // Properties
  double blurRadius() const { return blurRadius_; }
  void setBlurRadius(double radius) { blurRadius_ = radius; }

  OSFColor tintColor() const { return tintColor_; }
  void setTintColor(const OSFColor &color) { tintColor_ = color; }

  void setShadowEnabled(bool enabled);

  // Rendering
  void render(cairo_t *cr) override;
  void draw() override;

private:
  double blurRadius_ = 20.0;
  double tintAlpha_ = 0.85;
  OSFColor tintColor_{0.1, 0.1, 0.1, 0.85};
  bool shadowEnabled_ = true;
};

} // namespace opensef
