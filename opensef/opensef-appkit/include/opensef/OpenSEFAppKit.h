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
#include <opensef/OSFGeometry.h>
#include <opensef/OSFResponder.h>
#include <opensef/OSFView.h>
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
// Font/Text Types
// =============================================================================

enum class FontWeight { Normal, Medium, Bold };
enum class TextAlignment { Left, Center, Right };

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
  bool mouseDown(OSFEvent &event) override;
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
