/**
 * OSFShortcutManager.cpp - Universal Shortcut System Implementation
 */

#include <opensef/OSFShortcutManager.h>
#include <opensef/OSFEventBus.h>
#include <opensef/OSFKeys.h> // Use internal keys instead of xkbcommon
#include <cstdint>

namespace OpenSEF {

OSFShortcutManager &OSFShortcutManager::shared() {
  static OSFShortcutManager instance;
  return instance;
}

void OSFShortcutManager::registerShortcut(const std::string &id,
                                          uint32_t keycode,
                                          OSFModifier modifiers,
                                          std::function<void()> callback,
                                          const std::string &description) {
  OSFShortcut shortcut;
  shortcut.id = id;
  shortcut.keycode = keycode;
  shortcut.modifiers = modifiers;
  shortcut.callback = callback;
  shortcut.description = description;
  shortcut.enabled = true;

  m_shortcuts[id] = shortcut;
  m_keyLookup.insert({makeKey(keycode, modifiers), id});
}

void OSFShortcutManager::unregisterShortcut(const std::string &id) {
  auto it = m_shortcuts.find(id);
  if (it != m_shortcuts.end()) {
    // Remove from key lookup
    auto key = makeKey(it->second.keycode, it->second.modifiers);
    auto range = m_keyLookup.equal_range(key);
    for (auto lookupIt = range.first; lookupIt != range.second;) {
      if (lookupIt->second == id) {
        lookupIt = m_keyLookup.erase(lookupIt);
      } else {
        ++lookupIt;
      }
    }
    m_shortcuts.erase(it);
  }
}

void OSFShortcutManager::setEnabled(const std::string &id, bool enabled) {
  auto it = m_shortcuts.find(id);
  if (it != m_shortcuts.end()) {
    it->second.enabled = enabled;
  }
}

bool OSFShortcutManager::isEnabled(const std::string &id) const {
  auto it = m_shortcuts.find(id);
  return it != m_shortcuts.end() && it->second.enabled;
}

bool OSFShortcutManager::executeShortcut(const std::string &id) {
  auto it = m_shortcuts.find(id);
  if (it != m_shortcuts.end() && it->second.enabled && it->second.callback) {
    it->second.callback();
    return true;
  }
  return false;
}

bool OSFShortcutManager::handleKeyPress(uint32_t keycode,
                                        OSFModifier modifiers) {
  auto key = makeKey(keycode, modifiers);
  auto range = m_keyLookup.equal_range(key);

  for (auto it = range.first; it != range.second; ++it) {
    if (executeShortcut(it->second)) {
      return true;
    }
  }
  return false;
}

std::vector<OSFShortcut> OSFShortcutManager::allShortcuts() const {
  std::vector<OSFShortcut> result;
  for (const auto &pair : m_shortcuts) {
    result.push_back(pair.second);
  }
  return result;
}

uint64_t OSFShortcutManager::makeKey(uint32_t keycode,
                                     OSFModifier modifiers) const {
  return (static_cast<uint64_t>(modifiers) << 32) | keycode;
}

void OSFShortcutManager::registerSystemShortcuts() {
  // Multitask View: Super+Tab (or Cmd+Tab)
  registerShortcut(
      "multitask", Key::Tab, OSFModifier::Super,
      []() {
        OSFEventBus::shared().publish(OSFEventBus::MULTITASK_TOGGLE,
                                      OSFEvent());
      },
      "Show Multitask View");

  // Quit Application: Super+Q
  registerShortcut(
      "quit_app", Key::Q, OSFModifier::Super,
      []() {
        OSFEventBus::shared().publish(OSFEventBus::APP_QUIT_REQUEST,
                                      OSFEvent());
      },
      "Quit Application");

  // Close Window: Super+W
  registerShortcut(
      "close_window", Key::W, OSFModifier::Super,
      []() {
        OSFEventBus::shared().publish(OSFEventBus::WINDOW_CLOSE_REQUEST,
                                      OSFEvent());
      },
      "Close Window");

  // Minimize Window: Super+M
  registerShortcut(
      "minimize_window", Key::M, OSFModifier::Super,
      []() {
        OSFEventBus::shared().publish(OSFEventBus::WINDOW_MINIMIZE_REQUEST,
                                      OSFEvent());
      },
      "Minimize Window");

  // Hide Application: Super+H
  registerShortcut(
      "hide_app", Key::H, OSFModifier::Super,
      []() {
        OSFEventBus::shared().publish(OSFEventBus::APP_HIDE_REQUEST,
                                      OSFEvent());
      },
      "Hide Application");

  // Spotlight/Search: Super+Space
  registerShortcut(
      "spotlight", Key::Space, OSFModifier::Super,
      []() {
        OSFEventBus::shared().publish(OSFEventBus::SPOTLIGHT_TOGGLE,
                                      OSFEvent());
      },
      "Open Spotlight Search");

  // Copy: Super+C
  registerShortcut(
      "copy", Key::C, OSFModifier::Super,
      []() {
        OSFEventBus::shared().publish(OSFEventBus::CLIPBOARD_COPY, OSFEvent());
      },
      "Copy");

  // Paste: Super+V
  registerShortcut(
      "paste", Key::V, OSFModifier::Super,
      []() {
        OSFEventBus::shared().publish(OSFEventBus::CLIPBOARD_PASTE, OSFEvent());
      },
      "Paste");

  // Cut: Super+X
  registerShortcut(
      "cut", Key::X, OSFModifier::Super,
      []() {
        OSFEventBus::shared().publish(OSFEventBus::CLIPBOARD_CUT, OSFEvent());
      },
      "Cut");

  // Undo: Super+Z
  registerShortcut(
      "undo", Key::Z, OSFModifier::Super,
      []() {
        OSFEventBus::shared().publish(OSFEventBus::EDIT_UNDO, OSFEvent());
      },
      "Undo");

  // Redo: Super+Shift+Z
  registerShortcut(
      "redo", Key::Z, OSFModifier::Super | OSFModifier::Shift,
      []() {
        OSFEventBus::shared().publish(OSFEventBus::EDIT_REDO, OSFEvent());
      },
      "Redo");

  // Select All: Super+A
  registerShortcut(
      "select_all", Key::A, OSFModifier::Super,
      []() {
        OSFEventBus::shared().publish(OSFEventBus::EDIT_SELECT_ALL, OSFEvent());
      },
      "Select All");
}

} // namespace OpenSEF
