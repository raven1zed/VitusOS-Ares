#pragma once

#include "OSFStateManager.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace OpenSEF {

/**
 * OSFWindowManager - Window Lifecycle Management
 *
 * Tracks and manages all windows in the system.
 * Provides high-level window operations.
 *
 * Usage:
 *   auto* wm = desktop->windowManager();
 *   wm->focusWindow(windowId);
 *   wm->minimizeWindow(windowId);
 *   auto windows = wm->allWindows();
 */
class OSFWindowManager {
public:
  OSFWindowManager();
  ~OSFWindowManager();

  // Window queries
  std::vector<OSFWindow *> allWindows();
  OSFWindow *activeWindow();
  OSFWindow *windowById(const std::string &id);

  // Window actions
  void focusWindow(const std::string &id);
  void minimizeWindow(const std::string &id);
  void maximizeWindow(const std::string &id);
  void closeWindow(const std::string &id);
  void moveWindow(const std::string &id, int x, int y);
  void resizeWindow(const std::string &id, int width, int height);

  // Window registration (called by compositor)
  void registerWindow(OSFWindow *window);
  void unregisterWindow(const std::string &id);
  void updateWindowTitle(const std::string &id, const std::string &title);

  // Callbacks
  using WindowCallback = std::function<void(OSFWindow *)>;
  using WindowIdCallback = std::function<void(const std::string &)>;

  void onWindowCreated(WindowCallback callback);
  void onWindowDestroyed(WindowIdCallback callback);
  void onWindowFocused(WindowCallback callback);

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace OpenSEF
