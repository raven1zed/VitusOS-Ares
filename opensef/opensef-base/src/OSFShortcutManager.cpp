/**
 * OSFShortcutManager.cpp - Global Keybinding Implementation
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <opensef/OSFShortcutManager.h>

namespace opensef {

OSFShortcutManager &OSFShortcutManager::shared() {
  static OSFShortcutManager instance;
  return instance;
}

void OSFShortcutManager::registerAction(const std::string &identifier,
                                        std::function<void()> action) {
  actions_[identifier] = action;
}

void OSFShortcutManager::bindShortcut(const std::string &identifier,
                                      uint32_t modifiers, uint32_t keysym) {
  auto it = actions_.find(identifier);
  if (it != actions_.end()) {
    shortcuts_.push_back({modifiers, keysym, identifier, it->second});
  }
}

bool OSFShortcutManager::handleKey(uint32_t modifiers, uint32_t keysym) {
  for (const auto &shortcut : shortcuts_) {
    if (shortcut.modifiers == modifiers && shortcut.keysym == keysym) {
      if (shortcut.action) {
        shortcut.action();
        return true;
      }
    }
  }
  return false;
}

bool OSFShortcutManager::loadConfig(const std::string &path) {
  std::cout << "[openSEF] Loading shortcuts from: " << path << std::endl;

  std::ifstream f(path);
  if (!f.is_open()) {
    return false;
  }

  std::string line;
  while (std::getline(f, line)) {
    // Skip comments and whitespace
    line.erase(0, line.find_first_not_of(" \t"));
    if (line.empty() || line[0] == '#')
      continue;

    // Format: bind: <keys> = <action>
    if (line.rfind("bind:", 0) == 0) {
      size_t eq = line.find('=');
      if (eq != std::string::npos) {
        std::string keys = line.substr(5, eq - 5);
        std::string action = line.substr(eq + 1);

        // Trim
        keys.erase(keys.find_last_not_of(" \t") + 1);
        action.erase(0, action.find_first_not_of(" \t"));
        action.erase(action.find_last_not_of(" \t") + 1);

        // Simple hardcoded mapping for demonstration
        uint32_t mods = 0;
        if (keys.find("Super") != std::string::npos)
          mods |= 0x40; // WLR_MODIFIER_LOGO

        std::cout << "[ShortcutManager] Mapped: " << keys << " -> " << action
                  << std::endl;
      }
    }
  }

  return true;
}

} // namespace opensef
