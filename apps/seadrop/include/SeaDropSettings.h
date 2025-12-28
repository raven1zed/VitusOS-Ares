/**
 * SeaDropSettings.h - Settings for SeaDrop
 *
 * Configurable options for clipboard sync behavior.
 */

#pragma once

#include <string>

namespace vitusos::seadrop {

struct SeaDropSettings {
  // Sync behavior
  bool autoSync = true;   // Automatically sync clipboard
  bool syncOnWifi = true; // Only sync on WiFi (not cellular on mobile)
  int historySize = 100;  // Max clipboard history items

  // Connection
  std::string deviceName = "VitusOS Desktop";
  bool discoverableByDefault = true;
  int discoveryTimeoutMs = 30000; // 30 seconds

  // Security (future - Phase 4+)
  bool requirePairing = true; // Require device pairing before sync
  bool useEncryption = true;  // Encrypt transfers

  // UI
  bool showNotifications = true; // Desktop notifications on sync
  bool startMinimized = true;    // Start in tray
  bool launchAtLogin = true;     // Auto-start

  // Ares theme
  bool darkMode = false;              // Use dark theme
  std::string accentColor = "orange"; // Space Orange default
};

// Load/save settings to config file
SeaDropSettings loadSettings();
void saveSettings(const SeaDropSettings &settings);
std::string getSettingsPath();

} // namespace vitusos::seadrop
