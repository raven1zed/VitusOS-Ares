#include "OSFWindowManager.h"
#include "OSFDesktop.h"
#include "OSFEventBus.h"
#include <algorithm>

namespace OpenSEF {

struct OSFWindowManager::Impl {
  std::vector<WindowCallback> createdCallbacks;
  std::vector<WindowIdCallback> destroyedCallbacks;
  std::vector<WindowCallback> focusedCallbacks;
};

OSFWindowManager::OSFWindowManager() : impl_(std::make_unique<Impl>()) {}

OSFWindowManager::~OSFWindowManager() = default;

std::vector<OSFWindow *> OSFWindowManager::allWindows() {
  return OSFDesktop::shared()->stateManager()->allWindows();
}

OSFWindow *OSFWindowManager::activeWindow() {
  return OSFDesktop::shared()->stateManager()->activeWindow();
}

OSFWindow *OSFWindowManager::windowById(const std::string &id) {
  return OSFDesktop::shared()->stateManager()->windowById(id);
}

void OSFWindowManager::focusWindow(const std::string &id) {
  auto *window = windowById(id);
  if (window) {
    OSFDesktop::shared()->stateManager()->setActiveWindow(window);

    // Publish event
    OSFEvent event;
    event.set("window_id", id);
    OSFDesktop::shared()->eventBus()->publish(OSFEventBus::WINDOW_FOCUSED,
                                              event);

    // Call callbacks
    for (auto &callback : impl_->focusedCallbacks) {
      callback(window);
    }
  }
}

void OSFWindowManager::minimizeWindow(const std::string &id) {
  auto *window = windowById(id);
  if (window) {
    window->setMinimized(true);

    OSFEvent event;
    event.set("window_id", id);
    OSFDesktop::shared()->eventBus()->publish(OSFEventBus::WINDOW_MINIMIZED,
                                              event);
  }
}

void OSFWindowManager::maximizeWindow(const std::string &id) {
  auto *window = windowById(id);
  if (window) {
    window->setMaximized(!window->isMaximized());

    OSFEvent event;
    event.set("window_id", id);
    OSFDesktop::shared()->eventBus()->publish(OSFEventBus::WINDOW_MAXIMIZED,
                                              event);
  }
}

void OSFWindowManager::closeWindow(const std::string &id) {
  // Would send close request to compositor
  // For now, just remove from state
  unregisterWindow(id);
}

void OSFWindowManager::moveWindow(const std::string &id, int x, int y) {
  // Would send move request to compositor
  (void)id;
  (void)x;
  (void)y;
}

void OSFWindowManager::resizeWindow(const std::string &id, int width,
                                    int height) {
  // Would send resize request to compositor
  (void)id;
  (void)width;
  (void)height;
}

void OSFWindowManager::registerWindow(OSFWindow *window) {
  OSFDesktop::shared()->stateManager()->addWindow(window);

  // Publish event
  OSFEvent event;
  event.set("window_id", window->id());
  event.set("title", window->title());
  event.set("app_id", window->appId());
  OSFDesktop::shared()->eventBus()->publish(OSFEventBus::WINDOW_CREATED, event);

  // Call callbacks
  for (auto &callback : impl_->createdCallbacks) {
    callback(window);
  }
}

void OSFWindowManager::unregisterWindow(const std::string &id) {
  OSFDesktop::shared()->stateManager()->removeWindow(id);

  // Publish event
  OSFEvent event;
  event.set("window_id", id);
  OSFDesktop::shared()->eventBus()->publish(OSFEventBus::WINDOW_DESTROYED,
                                            event);

  // Call callbacks
  for (auto &callback : impl_->destroyedCallbacks) {
    callback(id);
  }
}

void OSFWindowManager::updateWindowTitle(const std::string &id,
                                         const std::string &title) {
  auto *window = windowById(id);
  if (window) {
    window->setTitle(title);
  }
}

void OSFWindowManager::onWindowCreated(WindowCallback callback) {
  impl_->createdCallbacks.push_back(callback);
}

void OSFWindowManager::onWindowDestroyed(WindowIdCallback callback) {
  impl_->destroyedCallbacks.push_back(callback);
}

void OSFWindowManager::onWindowFocused(WindowCallback callback) {
  impl_->focusedCallbacks.push_back(callback);
}

} // namespace OpenSEF
