#pragma once

#include <any>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace OpenSEF {

/**
 * OSFEvent - Event data container
 */
class OSFEvent {
public:
  OSFEvent() = default;
  explicit OSFEvent(const std::map<std::string, std::any> &data)
      : data_(data) {}

  template <typename T> T get(const std::string &key) const {
    auto it = data_.find(key);
    if (it != data_.end()) {
      return std::any_cast<T>(it->second);
    }
    return T{};
  }

  // Convenience helper for strings
  std::string getString(const std::string &key) const {
    return get<std::string>(key);
  }

  void set(const std::string &key, const std::any &value) {
    data_[key] = value;
  }

private:
  std::map<std::string, std::any> data_;
};

/**
 * OSFEventBus - Unified Event System
 *
 * All components communicate via events, not direct calls.
 * This enables loose coupling and easy extensibility.
 *
 * Usage:
 *   // Subscribe
 *   desktop->eventBus()->subscribe("window.created", [](const OSFEvent& e) {
 *     auto* window = e.get<void*>("window");
 *     updateUI(window);
 *   });
 *
 *   // Publish
 *   OSFEvent event;
 *   event.set("window", window);
 *   desktop->eventBus()->publish("window.created", event);
 */
class OSFEventBus {
public:
  using EventHandler = std::function<void(const OSFEvent &)>;

  // Singleton access
  static OSFEventBus &shared();

  OSFEventBus();
  ~OSFEventBus();

  // Subscribe to events with owner for cleanup
  void subscribe(const std::string &eventType, EventHandler handler);
  void subscribe(const std::string &eventType, EventHandler handler,
                 void *owner);
  void unsubscribe(const std::string &eventType, EventHandler *handler);
  void unsubscribeAll(void *owner);

  // Publish events
  void publish(const std::string &eventType, const OSFEvent &event);
  void publishAsync(const std::string &eventType, const OSFEvent &event);

  // Standard event types - Windows
  static constexpr const char *WINDOW_CREATED = "window.created";
  static constexpr const char *WINDOW_DESTROYED = "window.destroyed";
  static constexpr const char *WINDOW_FOCUSED = "window.focused";
  static constexpr const char *WINDOW_MINIMIZED = "window.minimized";
  static constexpr const char *WINDOW_MAXIMIZED = "window.maximized";
  static constexpr const char *WINDOW_CLOSE_REQUEST = "window.close_request";
  static constexpr const char *WINDOW_MINIMIZE_REQUEST =
      "window.minimize_request";
  static constexpr const char *WINDOW_GEOMETRY_CHANGED =
      "window.geometry_changed";

  // Application events
  static constexpr const char *MENU_CLICKED = "menu.clicked";
  static constexpr const char *APP_LAUNCHED = "application.launched";
  static constexpr const char *APP_TERMINATED = "application.closed";
  static constexpr const char *APP_QUIT_REQUEST = "application.quit_request";
  static constexpr const char *APP_HIDE_REQUEST = "application.hide_request";
  static constexpr const char *APP_ACTIVATED = "application.activated";

  // Service events (for always-running services like Filer)
  static constexpr const char *SERVICE_STARTED = "service.started";
  static constexpr const char *SERVICE_STOPPED = "service.stopped";
  static constexpr const char *SERVICE_ACTIVATED = "service.activated";

  // Shortcut events
  static constexpr const char *SHORTCUT_ACTIVATED = "shortcut.activated";

  static constexpr const char *THEME_CHANGED = "theme.changed";
  static constexpr const char *WORKSPACE_CHANGED = "workspace.changed";

  // Multitask & Pathfinder
  static constexpr const char *MULTITASK_TOGGLE = "multitask.toggle";
  static constexpr const char *SPOTLIGHT_TOGGLE = "spotlight.toggle";
  static constexpr const char *PATHFINDER_TOGGLE = "pathfinder.toggle";

  // Clipboard events
  static constexpr const char *CLIPBOARD_COPY = "clipboard.copy";
  static constexpr const char *CLIPBOARD_PASTE = "clipboard.paste";
  static constexpr const char *CLIPBOARD_CUT = "clipboard.cut";
  static constexpr const char *CLIPBOARD_CHANGED = "clipboard.changed";

  // Edit events
  static constexpr const char *EDIT_UNDO = "edit.undo";
  static constexpr const char *EDIT_REDO = "edit.redo";
  static constexpr const char *EDIT_SELECT_ALL = "edit.select_all";

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace OpenSEF
