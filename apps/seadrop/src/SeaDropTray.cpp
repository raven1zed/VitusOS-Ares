/**
 * SeaDropTray.cpp - System tray integration
 *
 * Uses StatusNotifierItem (SNI) protocol for Wayland/KDE/GNOME tray.
 * Falls back to XDG portal notifications.
 */

#include "SeaDrop.h"
#include <iostream>

namespace vitusos::seadrop {

// Tray icon states
static const char *ICON_DISCONNECTED = "seadrop-offline";
static const char *ICON_CONNECTED = "seadrop-online";
static const char *ICON_SYNCING = "seadrop-syncing";

SeaDropTray::SeaDropTray() {
  std::cout << "[SeaDrop] Initializing system tray..." << std::endl;

  // TODO: Initialize StatusNotifierItem via D-Bus
  // org.kde.StatusNotifierWatcher
  // org.freedesktop.StatusNotifierItem
}

SeaDropTray::~SeaDropTray() { hide(); }

void SeaDropTray::show() {
  std::cout << "[SeaDrop] Tray icon shown" << std::endl;

  // TODO: Register with StatusNotifierWatcher
  // Properties:
  //   - Id: "seadrop"
  //   - Category: "Communications"
  //   - Title: "SeaDrop"
  //   - IconName: ICON_DISCONNECTED
  //   - Menu: context menu items
}

void SeaDropTray::hide() {
  std::cout << "[SeaDrop] Tray icon hidden" << std::endl;

  // TODO: Unregister from StatusNotifierWatcher
}

void SeaDropTray::setConnected(bool connected) {
  std::cout << "[SeaDrop] Tray connection status: "
            << (connected ? "connected" : "disconnected") << std::endl;

  // TODO: Update icon via D-Bus
  // NewIcon signal with:
  //   IconName: connected ? ICON_CONNECTED : ICON_DISCONNECTED
  //
  // Space Orange dot for connected, gray for disconnected
}

void SeaDropTray::showNotification(const std::string &title,
                                   const std::string &message) {
  std::cout << "[SeaDrop] Notification: " << title << " - " << message
            << std::endl;

  // TODO: Send via org.freedesktop.Notifications D-Bus interface
  //
  // Method: Notify
  // Arguments:
  //   app_name: "SeaDrop"
  //   replaces_id: 0
  //   app_icon: "seadrop"
  //   summary: title
  //   body: message
  //   actions: ["open", "Open SeaDrop"]
  //   hints: {"urgency": 1}
  //   expire_timeout: 5000
}

// ============================================================================
// Context menu for tray icon
// ============================================================================

void createTrayMenu() {
  // TODO: Create D-Bus menu for tray right-click
  //
  // Menu items:
  // - "Open SeaDrop" -> show main window
  // - "---" (separator)
  // - "Pause Sync" -> toggle sync
  // - "Settings..." -> open settings
  // - "---"
  // - "Quit" -> quit application
}

} // namespace vitusos::seadrop
