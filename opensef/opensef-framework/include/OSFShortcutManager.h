/**
 * OSFShortcutManager.h - Universal Shortcut System
 *
 * System-wide keyboard shortcut management for openSEF
 */

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace OpenSEF {

/**
 * Modifier keys for shortcuts
 */
enum class OSFModifier : uint32_t {
  None = 0,
  Ctrl = 1 << 0,
  Alt = 1 << 1,
  Shift = 1 << 2,
  Super = 1 << 3, // Cmd/Win key
  Mod1 = Alt,
  Mod4 = Super
};

inline OSFModifier operator|(OSFModifier a, OSFModifier b) {
  return static_cast<OSFModifier>(static_cast<uint32_t>(a) |
                                  static_cast<uint32_t>(b));
}

inline bool operator&(OSFModifier a, OSFModifier b) {
  return (static_cast<uint32_t>(a) & static_cast<uint32_t>(b)) != 0;
}

/**
 * Shortcut definition
 */
struct OSFShortcut {
  std::string id;
  std::string description;
  uint32_t keycode; // XKB keycode
  OSFModifier modifiers;
  std::function<void()> callback;
  bool enabled = true;
};

/**
 * OSFShortcutManager - Central shortcut registry
 *
 * Handles system-wide keyboard shortcuts like:
 * - Cmd+Tab: Show Multitask view
 * - Cmd+Q: Quit application
 * - Cmd+W: Close window
 * - Cmd+Space: Spotlight/Search
 * - Cmd+C/V/X: Clipboard operations
 */
class OSFShortcutManager {
public:
  static OSFShortcutManager &shared();

  // Registration
  void registerShortcut(const std::string &id, uint32_t keycode,
                        OSFModifier modifiers, std::function<void()> callback,
                        const std::string &description = "");

  void unregisterShortcut(const std::string &id);

  // Enable/disable
  void setEnabled(const std::string &id, bool enabled);
  bool isEnabled(const std::string &id) const;

  // Execute shortcut by ID
  bool executeShortcut(const std::string &id);

  // Check if key matches any shortcut and execute
  bool handleKeyPress(uint32_t keycode, OSFModifier modifiers);

  // Get all registered shortcuts
  std::vector<OSFShortcut> allShortcuts() const;

  // System shortcuts (called during init)
  void registerSystemShortcuts();

private:
  OSFShortcutManager() = default;
  OSFShortcutManager(const OSFShortcutManager &) = delete;
  OSFShortcutManager &operator=(const OSFShortcutManager &) = delete;

  std::map<std::string, OSFShortcut> m_shortcuts;

  // Key lookup for fast matching
  std::multimap<uint64_t, std::string> m_keyLookup;

  uint64_t makeKey(uint32_t keycode, OSFModifier modifiers) const;
};

} // namespace OpenSEF
