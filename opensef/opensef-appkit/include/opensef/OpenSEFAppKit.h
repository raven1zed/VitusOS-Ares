/**
 * openSEF AppKit: GUI Widgets (C++ Version)
 *
 * Windows, views, buttons for VitusOS
 */

#pragma once

#include <functional>
#include <memory>
#include <opensef/OpenSEFBase.h>
#include <opensef/OpenSEFUI.h>
#include <string>
#include <vector>


namespace opensef {

// Forward declarations
class OSFView;
class OSFWindow;

// ============================================================================
// OSFView - Base class for all visual elements
// ============================================================================

class OSFView : public OSFObject {
public:
  OSFView();
  virtual ~OSFView();

  // Geometry
  OSFRect frame() const { return frame_; }
  void setFrame(const OSFRect &frame) { frame_ = frame; }

  // Visibility
  float alpha() const { return alpha_; }
  void setAlpha(float alpha) { alpha_ = alpha; }
  bool isHidden() const { return hidden_; }
  void setHidden(bool hidden) { hidden_ = hidden; }

  // Hierarchy
  OSFView *superview() const { return superview_; }
  const std::vector<std::shared_ptr<OSFView>> &subviews() const {
    return subviews_;
  }
  void addSubview(std::shared_ptr<OSFView> view);
  void removeFromSuperview();

  // Drawing
  virtual void draw();

protected:
  OSFRect frame_;
  float alpha_ = 1.0f;
  bool hidden_ = false;
  OSFView *superview_ = nullptr;
  std::vector<std::shared_ptr<OSFView>> subviews_;
};

// ============================================================================
// OSFWindow - Top-level window
// ============================================================================

class OSFWindow : public OSFView {
public:
  OSFWindow();
  OSFWindow(const std::string &title, const OSFRect &frame);
  virtual ~OSFWindow();

  static std::shared_ptr<OSFWindow> create(const std::string &title,
                                           const OSFRect &frame);

  // Properties
  const std::string &title() const { return title_; }
  void setTitle(const std::string &title) { title_ = title; }
  bool isVisible() const { return visible_; }

  // Actions
  void show();
  void close();
  void setContentView(std::shared_ptr<OSFView> view);

private:
  std::string title_;
  bool visible_ = false;
  std::shared_ptr<OSFView> contentView_;
  void *waylandSurface_ = nullptr; // Wayland surface handle
};

// ============================================================================
// OSFButton - Clickable button
// ============================================================================

class OSFButton : public OSFView {
public:
  OSFButton();
  OSFButton(const std::string &label, std::function<void()> action);
  virtual ~OSFButton();

  static std::shared_ptr<OSFButton> create(const std::string &label,
                                           std::function<void()> action);

  const std::string &label() const { return label_; }
  void setLabel(const std::string &label) { label_ = label; }
  void setAction(std::function<void()> action) { action_ = action; }

  void click();
  virtual void draw() override;

private:
  std::string label_;
  std::function<void()> action_;
  bool pressed_ = false;
};

// ============================================================================
// OSFLabel - Text display
// ============================================================================

class OSFLabel : public OSFView {
public:
  OSFLabel();
  OSFLabel(const std::string &text);
  virtual ~OSFLabel();

  const std::string &text() const { return text_; }
  void setText(const std::string &text) { text_ = text; }

  OSFColor textColor() const { return textColor_; }
  void setTextColor(const OSFColor &color) { textColor_ = color; }

  virtual void draw() override;

private:
  std::string text_;
  OSFColor textColor_ = OSFColors::textPrimary();
  float fontSize_ = 14.0f;
};

// ============================================================================
// OSFGlassPanel - Frosted glass effect (Ares signature)
// ============================================================================

class OSFGlassPanel : public OSFView {
public:
  OSFGlassPanel();
  virtual ~OSFGlassPanel();

  static std::shared_ptr<OSFGlassPanel> create(const OSFRect &frame);

  float blurRadius() const { return blurRadius_; }
  void setBlurRadius(float radius) { blurRadius_ = radius; }

  OSFColor tintColor() const { return tintColor_; }
  void setTintColor(const OSFColor &color) { tintColor_ = color; }

  virtual void draw() override;

private:
  float blurRadius_ = OSFStyle::blurRadiusMedium();
  float tintAlpha_ = 0.85f;
  OSFColor tintColor_ = OSFColors::surface();
};

} // namespace opensef
