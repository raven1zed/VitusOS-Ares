#pragma once

#include <memory>
#include <string>

namespace OpenSEF {

// Forward declarations
class OSFEventBus;
class OSFStateManager;
class OSFWindowManager;
class OSFServiceRegistry;
class OSFResourceCache;
class OSFThemeManager;
class AnimationEngine;

/**
 * OSFDesktop - Central Desktop Environment API
 *
 * Singleton that provides unified access to all desktop services.
 * This is the single entry point for all desktop functionality.
 *
 * Usage:
 *   auto* desktop = OSFDesktop::shared();
 *   desktop->eventBus()->publish("event", data);
 *   auto windows = desktop->windowManager()->allWindows();
 */
class OSFDesktop {
public:
  // Singleton access
  static OSFDesktop *shared();

  // Core services
  OSFEventBus *eventBus();
  OSFStateManager *stateManager();
  OSFWindowManager *windowManager();
  OSFServiceRegistry *serviceRegistry();
  OSFResourceCache *resourceCache();
  OSFThemeManager *themeManager();

  // Boot-to-Shutdown Visual System
  AnimationEngine *animationEngine();

  // Lifecycle
  void initialize();
  void run();
  void terminate();

  // Prevent copying
  OSFDesktop(const OSFDesktop &) = delete;
  OSFDesktop &operator=(const OSFDesktop &) = delete;

private:
  OSFDesktop();
  ~OSFDesktop();

  static OSFDesktop *instance_;

  std::unique_ptr<OSFEventBus> eventBus_;
  std::unique_ptr<OSFStateManager> stateManager_;
  std::unique_ptr<OSFWindowManager> windowManager_;
  std::unique_ptr<OSFServiceRegistry> serviceRegistry_;
  std::unique_ptr<OSFResourceCache> resourceCache_;
  std::unique_ptr<OSFThemeManager> themeManager_;
  std::unique_ptr<AnimationEngine> animationEngine_;

  bool initialized_;
};

} // namespace OpenSEF
