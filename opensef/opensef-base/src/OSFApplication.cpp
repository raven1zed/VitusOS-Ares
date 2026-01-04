/**
 * OSFApplication.cpp - Application lifecycle and event coordination
 *
 * Phase 3: Unified event loop that polls all registered windows.
 */

#include <opensef/OSFResponder.h>
#include <opensef/OSFWindow.h>
#include <opensef/OpenSEFBase.h>


#include <algorithm>
#include <poll.h>

namespace opensef {

OSFApplication &OSFApplication::shared() {
  static OSFApplication app;
  return app;
}

void OSFApplication::run() {
  if (onLaunch_) {
    onLaunch_();
  }

  // Phase 3: Unified event loop
  // Instead of each window running its own loop, we poll all windows here
  runLoop_.run();

  if (onTerminate_) {
    onTerminate_();
  }
}

void OSFApplication::stop() { runLoop_.stop(); }

// === Window Management (Phase 3) ===

void OSFApplication::registerWindow(OSFWindow *window) {
  if (window &&
      std::find(windows_.begin(), windows_.end(), window) == windows_.end()) {
    windows_.push_back(window);
  }
}

void OSFApplication::unregisterWindow(OSFWindow *window) {
  auto it = std::find(windows_.begin(), windows_.end(), window);
  if (it != windows_.end()) {
    windows_.erase(it);
  }
}

// === First Responder (Phase 3) ===

bool OSFApplication::makeFirstResponder(OSFResponder *responder) {
  if (responder && !responder->acceptsFirstResponder()) {
    return false;
  }

  // Resign current first responder
  if (firstResponder_) {
    firstResponder_->resignFirstResponder();
  }

  // Set new first responder
  firstResponder_ = responder;

  if (firstResponder_) {
    firstResponder_->becomeFirstResponder();
  }

  return true;
}

} // namespace opensef
