/**
 * OSFButton.cpp - Button implementation
 */

#include <iostream>
#include <opensef/OpenSEFAppKit.h>


namespace opensef {

OSFButton::OSFButton() = default;

OSFButton::OSFButton(const std::string &label, std::function<void()> action)
    : label_(label), action_(action) {}

OSFButton::~OSFButton() = default;

std::shared_ptr<OSFButton> OSFButton::create(const std::string &label,
                                             std::function<void()> action) {
  return std::make_shared<OSFButton>(label, action);
}

void OSFButton::click() {
  if (action_) {
    action_();
  }
}

void OSFButton::draw() {
  if (hidden_)
    return;

  // TODO: Vulkan rendering
  // Draw rounded rect background (Space Orange when pressed)
  // Draw label text centered

  OSFView::draw();
}

} // namespace opensef
