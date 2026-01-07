#include "OSFEventBus.h"
#include <algorithm>
#include <mutex>
#include <queue>
#include <thread>

namespace OpenSEF {

struct OSFEventBus::Impl {
  std::map<std::string, std::vector<EventHandler>> handlers;
  std::mutex mutex;
  std::queue<std::pair<std::string, OSFEvent>> asyncQueue;
};

OSFEventBus::OSFEventBus() : impl_(std::make_unique<Impl>()) {}

OSFEventBus::~OSFEventBus() = default;

void OSFEventBus::subscribe(const std::string &eventType,
                            EventHandler handler) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  impl_->handlers[eventType].push_back(handler);
}

void OSFEventBus::unsubscribe(const std::string &eventType,
                              EventHandler *handler) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  auto it = impl_->handlers.find(eventType);
  if (it != impl_->handlers.end()) {
    // Remove handler (simplified - would need better comparison in production)
    it->second.clear();
  }
}

void OSFEventBus::publish(const std::string &eventType, const OSFEvent &event) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  auto it = impl_->handlers.find(eventType);
  if (it != impl_->handlers.end()) {
    for (auto &handler : it->second) {
      handler(event);
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
