/**
 * OSFTitleBar.cpp - Window title bar with traffic lights
 */

#include <opensef/OSFWindowDecorations.h>

namespace opensef {

OSFTitleBar::OSFTitleBar() {
  // Create traffic light buttons
  closeButton_ = OSFWindowButton::create(OSFWindowButtonType::Close);
  minimizeButton_ = OSFWindowButton::create(OSFWindowButtonType::Minimize);
  maximizeButton_ = OSFWindowButton::create(OSFWindowButtonType::Maximize);

  // Add as subviews
  addSubview(closeButton_);
  addSubview(minimizeButton_);
  addSubview(maximizeButton_);

  // Set default height
  setFrame(OSFRect(0, 0, 0, kTitleBarHeight));
}

OSFTitleBar::~OSFTitleBar() = default;

std::shared_ptr<OSFTitleBar> OSFTitleBar::create(const std::string &title) {
  auto bar = std::make_shared<OSFTitleBar>();
  bar->setTitle(title);
  return bar;
}

void OSFTitleBar::setTitle(const std::string &title) { title_ = title; }

void OSFTitleBar::setOnClose(std::function<void()> action) {
  closeButton_->setAction(action);
}

void OSFTitleBar::setOnMinimize(std::function<void()> action) {
  minimizeButton_->setAction(action);
}

void OSFTitleBar::setOnMaximize(std::function<void()> action) {
  maximizeButton_->setAction(action);
}

void OSFTitleBar::layoutSubviews() {
  float buttonDiameter = OSFWindowButton::kButtonRadius * 2;
  float y = (kTitleBarHeight - buttonDiameter) / 2;
  float x = kButtonPadding;

  // Close button (red, leftmost)
  closeButton_->setFrame(OSFRect(x, y, buttonDiameter, buttonDiameter));
  x += buttonDiameter + OSFWindowButton::kButtonSpacing;

  // Minimize button (yellow)
  minimizeButton_->setFrame(OSFRect(x, y, buttonDiameter, buttonDiameter));
  x += buttonDiameter + OSFWindowButton::kButtonSpacing;

  // Maximize button (green)
  maximizeButton_->setFrame(OSFRect(x, y, buttonDiameter, buttonDiameter));
}

void OSFTitleBar::render(cairo_t *cr) {
  // Draw title bar background (Ares Dark Glass style)
  cairo_save(cr);
  cairo_set_source_rgba(cr, 0.1, 0.1, 0.1, 0.85); // Dark semi-transparent
  cairo_rectangle(cr, 0, 0, frame_.width, frame_.height);
  cairo_fill(cr);
  cairo_restore(cr);

  // Draw title text
  if (!title_.empty()) {
    cairo_save(cr);
    cairo_set_source_rgb(cr, 0.9, 0.9, 0.9);
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 13.0);

    cairo_text_extents_t extents;
    cairo_text_extents(cr, title_.c_str(), &extents);

    float tx = (frame_.width - extents.width) / 2.0f;
    float ty = (frame_.height / 2.0f) + (extents.height / 2.0f) -
               extents.y_bearing / 4.0f;

    cairo_move_to(cr, tx, ty);
    cairo_show_text(cr, title_.c_str());
    cairo_restore(cr);
  }

  // OSFView::render will handle subview buttons
  OSFView::render(cr);
}

} // namespace opensef
