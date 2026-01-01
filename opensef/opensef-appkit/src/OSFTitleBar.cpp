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

void OSFTitleBar::layoutButtons() {
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
  // Layout buttons
  layoutButtons();

  // TODO: Draw title bar background
  // TODO: Draw title text centered

  // Draw buttons
  closeButton_->render(cr);
  minimizeButton_->render(cr);
  maximizeButton_->render(cr);
}

} // namespace opensef
