#include <opensef/OSFDesktop.h>
#include <opensef/OSFEventBus.h>
#include <opensef/OSFResourceCache.h>
#include <opensef/OSFServiceRegistry.h>
#include <opensef/OSFStateManager.h>
#include <opensef/OSFThemeManager.h>
#include <opensef/OSFWindowManager.h>


namespace OpenSEF {

// Static instance
OSFDesktop *OSFDesktop::instance_ = nullptr;

OSFDesktop::OSFDesktop() : initialized_(false) {
  // Create all core services
  eventBus_ = std::make_unique<OSFEventBus>();
  stateManager_ = std::make_unique<OSFStateManager>();
  windowManager_ = std::make_unique<OSFWindowManager>();
  serviceRegistry_ = std::make_unique<OSFServiceRegistry>();
  resourceCache_ = std::make_unique<OSFResourceCache>();
  themeManager_ = std::make_unique<OSFThemeManager>();
}

OSFDesktop::~OSFDesktop() { terminate(); }

OSFDesktop *OSFDesktop::shared() {
  if (!instance_) {
    instance_ = new OSFDesktop();
  }
  return instance_;
}

OSFEventBus *OSFDesktop::eventBus() { return eventBus_.get(); }

OSFStateManager *OSFDesktop::stateManager() { return stateManager_.get(); }

OSFWindowManager *OSFDesktop::windowManager() { return windowManager_.get(); }

OSFServiceRegistry *OSFDesktop::serviceRegistry() {
  return serviceRegistry_.get();
}

OSFResourceCache *OSFDesktop::resourceCache() { return resourceCache_.get(); }

OSFThemeManager *OSFDesktop::themeManager() { return themeManager_.get(); }

void OSFDesktop::initialize() {
  if (initialized_) {
    return;
  }

  // Initialize all services
  // (Currently they auto-initialize in constructors)

  initialized_ = true;
}

void OSFDesktop::run() {
  // Main event loop would go here
  // For now, this is handled by individual components
}

void OSFDesktop::terminate() {
  if (!initialized_) {
    return;
  }

  // Cleanup all services
  themeManager_.reset();
  resourceCache_.reset();
  serviceRegistry_.reset();
  windowManager_.reset();
  stateManager_.reset();
  eventBus_.reset();

  initialized_ = false;
}

} // namespace OpenSEF
