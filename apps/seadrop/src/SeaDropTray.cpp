/**
 * SeaDropTray.cpp - System tray integration
 */

#include "SeaDrop.h"

namespace vitusos::seadrop {

SeaDropTray::SeaDropTray() {
  // TODO: Create tray icon via Wayland tray protocol
  // or D-Bus StatusNotifierItem
}

SeaDropTray::~SeaDropTray() {}

void SeaDropTray::show() {
  // TODO: Show tray icon
}

void SeaDropTray::hide() {
  // TODO: Hide tray icon
}

void SeaDropTray::setConnected(bool connected) {
  // TODO: Change tray icon based on connection status
  // Green dot = connected
  // Gray dot = disconnected
}

void SeaDropTray::showNotification(const std::string &title,
                                   const std::string &message) {
  // TODO: Send desktop notification via D-Bus
  // org.freedesktop.Notifications
}

} // namespace vitusos::seadrop
