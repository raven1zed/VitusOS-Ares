#include <algorithm>
#include <mutex>
#include <opensef/OSFEventBus.h>
#include <queue>
#include <thread>

namespace OpenSEF {

struct OSFEventBus::Impl {
  struct HandlerEntry {
    EventHandler handler;
    void *owner = nullptr;
  };
  std::map<std::string, std::vector<HandlerEntry>> handlers;
  std::mutex mutex;
  std::queue<std::pair<std::string, OSFEvent>> asyncQueue;
};

OSFEventBus::OSFEventBus() : impl_(std::make_unique<Impl>()) {}

OSFEventBus::~OSFEventBus() = default;

OSFEventBus &OSFEventBus::shared() {
  static OSFEventBus instance;
  return instance;
}

void OSFEventBus::subscribe(const std::string &eventType,
                            EventHandler handler) {
  subscribe(eventType, handler, nullptr);
}

void OSFEventBus::subscribe(const std::string &eventType, EventHandler handler,
                            void *owner) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  impl_->handlers[eventType].push_back({handler, owner});
}

void OSFEventBus::unsubscribe(const std::string &eventType,
                              EventHandler *handler) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  auto it = impl_->handlers.find(eventType);
  if (it != impl_->handlers.end()) {
    // Simplified - clear all handlers for this event type
    it->second.clear();
  }
}

void OSFEventBus::unsubscribeAll(void *owner) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  for (auto &pair : impl_->handlers) {
    auto &entries = pair.second;
    entries.erase(std::remove_if(entries.begin(), entries.end(),
                                 [owner](const Impl::HandlerEntry &entry) {
                                   return entry.owner == owner;
                                 }),
                  entries.end());
  }
}

void OSFEventBus::publish(const std::string &eventType, const OSFEvent &event) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  auto it = impl_->handlers.find(eventType);
  if (it != impl_->handlers.end()) {
    for (auto &entry : it->second) {
      entry.handler(event);
    }
  }
}

void OSFEventBus::publishAsync(const std::string &eventType,
                               const OSFEvent &event) {
  // For now, just publish synchronously
  // In production, would use a separate thread
  publish(eventType, event);
}

} // namespace OpenSEF
