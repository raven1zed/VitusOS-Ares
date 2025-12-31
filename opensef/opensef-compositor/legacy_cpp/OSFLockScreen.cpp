/**
 * OSFLockScreen.cpp - Lock Screen Implementation
 */

#include "OSFLockScreen.h"
#include "OSFAresTheme.h"
#include "OSFCompositor.h"
#include "OSFDesktopLayers.h"


#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace opensef {

OSFLockScreen::OSFLockScreen(OSFCompositor *compositor,
                             OSFDesktopLayers *layers)
    : compositor_(compositor), layers_(layers) {
  std::cout << "[openSEF] LockScreen initialized" << std::endl;
}

OSFLockScreen::~OSFLockScreen() { destroyNodes(); }

void OSFLockScreen::show() {
  if (visible_)
    return;

  visible_ = true;
  createNodes();

  std::cout << "[openSEF] LockScreen shown - " << formattedDate() << " "
            << formattedTime() << std::endl;
}

void OSFLockScreen::hide() {
  if (!visible_)
    return;

  visible_ = false;
  passwordBuffer_.clear();
  destroyNodes();

  std::cout << "[openSEF] LockScreen hidden" << std::endl;
}

void OSFLockScreen::updateClock() {
  // In a real implementation, this would update text nodes
  // For now, just log the time
  if (visible_) {
    std::cout << "[openSEF] LockScreen time: " << formattedTime() << std::endl;
  }
}

std::string OSFLockScreen::formattedDate() const {
  std::time_t now = std::time(nullptr);
  std::tm *local = std::localtime(&now);

  // Get day name (lowercase)
  const char *days[] = {"sunday",   "monday", "tuesday", "wednesday",
                        "thursday", "friday", "saturday"};

  std::ostringstream oss;
  oss << days[local->tm_wday] << " " << local->tm_mday << ", "
      << (1900 + local->tm_year);

  return oss.str();
}

std::string OSFLockScreen::formattedTime() const {
  std::time_t now = std::time(nullptr);
  std::tm *local = std::localtime(&now);

  int hour = local->tm_hour;
  const char *ampm = (hour >= 12) ? "PM" : "AM";
  if (hour > 12)
    hour -= 12;
  if (hour == 0)
    hour = 12;

  std::ostringstream oss;
  oss << hour << ":" << std::setfill('0') << std::setw(2) << local->tm_min
      << " " << ampm;

  return oss.str();
}

void OSFLockScreen::createNodes() {
  // Create a subtle overlay in the Lock layer
  // Text rendering requires additional infrastructure (font loading)
  // For now, we just set up the lock layer

  // Note: Actual text rendering would use a text atlas or font library
  // This is a placeholder until text rendering is implemented
}

void OSFLockScreen::destroyNodes() {
  if (background_) {
    wlr_scene_node_destroy(&background_->node);
    background_ = nullptr;
  }
}

void OSFLockScreen::onKeyPress(uint32_t keycode, bool pressed) {
  if (!visible_ || !pressed)
    return;

  // Handle password input
  // Enter key (28) submits password
  if (keycode == 28) {
    if (onPasswordEntered) {
      onPasswordEntered(passwordBuffer_);
    }
    passwordBuffer_.clear();
  }
  // Backspace (14) removes last character
  else if (keycode == 14 && !passwordBuffer_.empty()) {
    passwordBuffer_.pop_back();
  }
  // Add printable characters (simplified)
  else if (keycode >= 2 && keycode <= 57) {
    // This is a simplified mapping - real implementation needs proper keymap
    passwordBuffer_ += '*'; // Mask input
  }
}

} // namespace opensef
