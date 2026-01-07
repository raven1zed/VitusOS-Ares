/**
 * OSFShortcutManager.h - Global Keybinding Management
 *
 * Registry for system-wide shortcuts.
 */

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>


namespace opensef {

struct OSFShortcut {
  uint32_t modifiers;
  uint32_t keysym;
  std::string identifier;
  std::function<void()> action;
};

class OSFShortcutManager {
public:
  static OSFShortcutManager &shared();

  // Register a static action (e.g. from compositor)
  void registerAction(const std::string &identifier,
                      std::function<void()> action);

  // Register a shortcut bound to an action
  void bindShortcut(const std::string &identifier, uint32_t modifiers,
                    uint32_t keysym);

  // Handle key event (returns true if handled)
  bool handleKey(uint32_t modifiers, uint32_t keysym);

  // Config loading
  bool loadConfig(const std::string &path);

private:
  OSFShortcutManager() = default;

  std::map<std::string, std::function<void()>> actions_;
  std::vector<OSFShortcut> shortcuts_;
};

} // namespace opensef
