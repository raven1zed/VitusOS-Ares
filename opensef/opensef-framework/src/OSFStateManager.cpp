#include "OSFStateManager.h"
#include <algorithm>
#include <map>
#include <mutex>

namespace OpenSEF {

// OSFWindow implementation
OSFWindow::OSFWindow(const std::string &id, const std::string &title,
                     const std::string &appId)
    : id_(id), title_(title), appId_(appId) {}

// OSFApplication implementation
OSFApplication::OSFApplication(const std::string &id, const std::string &name)
    : id_(id), name_(name) {}

// OSFWorkspace implementation
OSFWorkspace::OSFWorkspace(int id, const std::string &name)
    : id_(id), name_(name) {}

// OSFStateManager implementation
struct OSFStateManager::Impl {
  std::map<std::string, OSFWindow *> windows;
  std::map<std::string, OSFApplication *> applications;
  std::vector<OSFWorkspace *> workspaces;
  OSFWindow *activeWindow = nullptr;
  int currentWorkspace = 0;
  std::mutex mutex;
};

OSFStateManager::OSFStateManager() : impl_(std::make_unique<Impl>()) {
  // Create default workspace
  impl_->workspaces.push_back(new OSFWorkspace(0, "Workspace 1"));
}

OSFStateManager::~OSFStateManager() {
  // Cleanup windows
  for (auto &pair : impl_->windows) {
    delete pair.second;
  }

  // Cleanup applications
  for (auto &pair : impl_->applications) {
    delete pair.second;
  }

  // Cleanup workspaces
  for (auto *workspace : impl_->workspaces) {
    delete workspace;
  }
}

std::vector<OSFWindow *> OSFStateManager::allWindows() {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  std::vector<OSFWindow *> result;
  for (auto &pair : impl_->windows) {
    result.push_back(pair.second);
  }
  return result;
}

OSFWindow *OSFStateManager::activeWindow() {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  return impl_->activeWindow;
}

OSFWindow *OSFStateManager::windowById(const std::string &id) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  auto it = impl_->windows.find(id);
  return (it != impl_->windows.end()) ? it->second : nullptr;
}

std::vector<OSFApplication *> OSFStateManager::runningApps() {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  std::vector<OSFApplication *> result;
  for (auto &pair : impl_->applications) {
    result.push_back(pair.second);
  }
  return result;
}

OSFApplication *OSFStateManager::appById(const std::string &id) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  auto it = impl_->applications.find(id);
  return (it != impl_->applications.end()) ? it->second : nullptr;
}

int OSFStateManager::currentWorkspace() {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  return impl_->currentWorkspace;
}

std::vector<OSFWorkspace *> OSFStateManager::allWorkspaces() {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  return impl_->workspaces;
}

void OSFStateManager::setActiveWindow(OSFWindow *window) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  if (impl_->activeWindow) {
    impl_->activeWindow->setFocused(false);
  }
  impl_->activeWindow = window;
  if (window) {
    window->setFocused(true);
  }
}

void OSFStateManager::addWindow(OSFWindow *window) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  impl_->windows[window->id()] = window;
}

void OSFStateManager::removeWindow(const std::string &id) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  auto it = impl_->windows.find(id);
  if (it != impl_->windows.end()) {
    if (impl_->activeWindow == it->second) {
      impl_->activeWindow = nullptr;
    }
    delete it->second;
    impl_->windows.erase(it);
  }
}

void OSFStateManager::addApplication(OSFApplication *app) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  impl_->applications[app->id()] = app;
}

void OSFStateManager::removeApplication(const std::string &id) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  auto it = impl_->applications.find(id);
  if (it != impl_->applications.end()) {
    delete it->second;
    impl_->applications.erase(it);
  }
}

} // namespace OpenSEF
