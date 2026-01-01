/**
 * openSEF AppKit: Production GUI Widgets
 *
 * Cairo/Pango-based widgets for VitusOS Ares
 * Integrated with OSFLayer animation framework
 */

#pragma once

#include <cairo/cairo.h>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>


// Forward declare Pango types
typedef struct _PangoLayout PangoLayout;

namespace opensef {

// Global Vulkan state access for widgets (AppKit/Compositor integration)
// The application must set this before calling draw() on widgets using Vulkan.
extern VkCommandBuffer g_CurrentVulkanCommandBuffer;

/**
 * Initialize the AppKit Vulkan subsystem.
 * Must be called once after Vulkan initialization.
 */
void OSFAppKitInitializeVulkan(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue queue, VkCommandPool commandPool, VkRenderPass renderPass);

// =============================================================================
// Geometry Types
// =============================================================================

struct OSFRect {
  double x = 0, y = 0, width = 0, height = 0;
  OSFRect() = default;
  OSFRect(double x_, double y_, double w, double h)
      : x(x_), y(y_), width(w), height(h) {}

  static OSFRect Zero() { return OSFRect(0, 0, 0, 0); }
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
// OSFView - Base class for all visual elements
// =============================================================================

class OSFView {
public:
  OSFView() = default;
  virtual ~OSFView() = default;

  // Geometry
  OSFRect frame() const { return frame_; }
  void setFrame(const OSFRect &frame) { frame_ = frame; }

  // Visibility
  double alpha() const { return alpha_; }
  void setAlpha(double alpha) { alpha_ = alpha; }
  bool isHidden() const { return hidden_; }
  void setHidden(bool hidden) { hidden_ = hidden; }

  // Hierarchy
  OSFView *superview() const { return superview_; }
  const std::vector<std::shared_ptr<OSFView>> &subviews() const {
    return subviews_;
  }
  void addSubview(std::shared_ptr<OSFView> view);
  void removeFromSuperview();

  // Rendering
  virtual void draw() {}
  virtual void render(cairo_t *cr);

protected:
  OSFRect frame_;
  double alpha_ = 1.0;
  bool hidden_ = false;
  OSFView *superview_ = nullptr;
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

// =============================================================================
// OSFWindow - Window abstraction for AppKit
// =============================================================================

class OSFWindow {
public:
  OSFWindow();
  OSFWindow(const std::string &title, const OSFRect &frame);
  virtual ~OSFWindow();

  static std::shared_ptr<OSFWindow> create(const std::string &title,
                                           const OSFRect &frame);

  const std::string &title() const { return title_; }
  OSFRect frame() const { return frame_; }
  bool isVisible() const { return visible_; }

  void show();
  void close();
  void setContentView(std::shared_ptr<OSFView> view);

private:
  std::string title_;
  OSFRect frame_;
  bool visible_ = false;
  std::shared_ptr<OSFView> contentView_;
};

} // namespace opensef
