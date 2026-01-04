/**
 * SeaDropTray.cpp - System tray integration (pure C++)
 */

#include "SeaDrop.h"
#include <iostream>

namespace vitusos::seadrop {

SeaDropTray::SeaDropTray() = default;
SeaDropTray::~SeaDropTray() = default;

void SeaDropTray::show() {
  // TODO: Implement StatusNotifierItem for Wayland
  // For now, just log
  std::cout << "[SeaDrop] Tray icon shown (stub)" << std::endl;
}

void SeaDropTray::hide() {
  std::cout << "[SeaDrop] Tray icon hidden" << std::endl;
}

void SeaDropTray::setConnected(bool connected) {
  connected_ = connected;
  std::cout << "[SeaDrop] Connected: " << (connected ? "yes" : "no")
            << std::endl;
}

} // namespace vitusos::seadrop
