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

  OSFEventBus();
  ~OSFEventBus();

  // Subscribe to events
  void subscribe(const std::string &eventType, EventHandler handler);
  void unsubscribe(const std::string &eventType, EventHandler *handler);

  // Publish events
  void publish(const std::string &eventType, const OSFEvent &event);
  void publishAsync(const std::string &eventType, const OSFEvent &event);

  // Standard event types
  static constexpr const char *WINDOW_CREATED = "window.created";
  static constexpr const char *WINDOW_DESTROYED = "window.destroyed";
  static constexpr const char *WINDOW_FOCUSED = "window.focused";
  static constexpr const char *WINDOW_MINIMIZED = "window.minimized";
  static constexpr const char *WINDOW_MAXIMIZED = "window.maximized";

  static constexpr const char *MENU_CLICKED = "menu.clicked";
  static constexpr const char *APP_LAUNCHED = "app.launched";
  static constexpr const char *APP_TERMINATED = "app.terminated";

  static constexpr const char *THEME_CHANGED = "theme.changed";
  static constexpr const char *WORKSPACE_CHANGED = "workspace.changed";

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace OpenSEF
