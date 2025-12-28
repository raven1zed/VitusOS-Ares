/**
 * SeaDropSettings.cpp - Settings persistence
 */

#include "SeaDropSettings.h"
#include <cstdlib>
#include <fstream>
#include <iostream>


namespace vitusos::seadrop {

std::string getSettingsPath() {
  // XDG config directory
  const char *xdgConfig = std::getenv("XDG_CONFIG_HOME");
  std::string configDir;

  if (xdgConfig) {
    configDir = xdgConfig;
  } else {
    const char *home = std::getenv("HOME");
    if (home) {
      configDir = std::string(home) + "/.config";
    } else {
      configDir = "/tmp";
    }
  }

  return configDir + "/seadrop/settings.conf";
}

SeaDropSettings loadSettings() {
  SeaDropSettings settings;
  std::string path = getSettingsPath();

  std::ifstream file(path);
  if (!file.is_open()) {
    std::cout << "[SeaDrop] No settings file, using defaults" << std::endl;
    return settings;
  }

  std::string line;
  while (std::getline(file, line)) {
    // Simple key=value parser
    size_t pos = line.find('=');
    if (pos == std::string::npos)
      continue;

    std::string key = line.substr(0, pos);
    std::string value = line.substr(pos + 1);

    if (key == "autoSync")
      settings.autoSync = (value == "true");
    else if (key == "syncOnWifi")
      settings.syncOnWifi = (value == "true");
    else if (key == "historySize")
      settings.historySize = std::stoi(value);
    else if (key == "deviceName")
      settings.deviceName = value;
    else if (key == "showNotifications")
      settings.showNotifications = (value == "true");
    else if (key == "startMinimized")
      settings.startMinimized = (value == "true");
    else if (key == "launchAtLogin")
      settings.launchAtLogin = (value == "true");
    else if (key == "darkMode")
      settings.darkMode = (value == "true");
    else if (key == "accentColor")
      settings.accentColor = value;
  }

  std::cout << "[SeaDrop] Settings loaded from " << path << std::endl;
  return settings;
}

void saveSettings(const SeaDropSettings &settings) {
  std::string path = getSettingsPath();

  // Create directory if needed
  std::string dir = path.substr(0, path.rfind('/'));
  std::string mkdirCmd = "mkdir -p " + dir;
  system(mkdirCmd.c_str());

  std::ofstream file(path);
  if (!file.is_open()) {
    std::cerr << "[SeaDrop] Failed to save settings to " << path << std::endl;
    return;
  }

  file << "# SeaDrop Settings\n";
  file << "autoSync=" << (settings.autoSync ? "true" : "false") << "\n";
  file << "syncOnWifi=" << (settings.syncOnWifi ? "true" : "false") << "\n";
  file << "historySize=" << settings.historySize << "\n";
  file << "deviceName=" << settings.deviceName << "\n";
  file << "showNotifications="
       << (settings.showNotifications ? "true" : "false") << "\n";
  file << "startMinimized=" << (settings.startMinimized ? "true" : "false")
       << "\n";
  file << "launchAtLogin=" << (settings.launchAtLogin ? "true" : "false")
       << "\n";
  file << "darkMode=" << (settings.darkMode ? "true" : "false") << "\n";
  file << "accentColor=" << settings.accentColor << "\n";

  std::cout << "[SeaDrop] Settings saved to " << path << std::endl;
}

} // namespace vitusos::seadrop
