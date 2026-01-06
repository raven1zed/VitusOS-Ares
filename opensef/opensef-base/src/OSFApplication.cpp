/**
 * OSFApplication.cpp - Application lifecycle and event coordination
 *
 * Phase 3: REAL unified event loop that polls all registered windows.
 * This is the single entry point for openSEF applications.
 */

#include <opensef/OSFResponder.h>
#include <opensef/OSFWindow.h>
#include <opensef/OpenSEFBase.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <poll.h>
#include <vector>

namespace opensef {

OSFApplication &OSFApplication::shared() {
  static OSFApplication app;
  return app;
}

void OSFApplication::run() {
  if (onLaunch_) {
    onLaunch_();
  }

  running_ = true;

  while (running_) {
    // If no windows/sources are registered, just wait
    if (windows_.empty() && externalSources_.empty()) {
      // Process any pending runloop tasks
      runLoop_.postTask([]() {}); // Wake up runloop
      std::this_thread::sleep_for(std::chrono::milliseconds(16));
      continue;
    }

    // Build poll file descriptor array
    std::vector<struct pollfd> fds;
    std::vector<OSFWindow *> activeWindows;

    // 1. Registered Windows
    for (OSFWindow *window : windows_) {
      int fd = window->displayFd();
      if (fd >= 0) {
        struct pollfd pfd;
        pfd.fd = fd;
        pfd.events = POLLIN;
        pfd.revents = 0;
        fds.push_back(pfd);
        activeWindows.push_back(window);
      }
    }

    // 2. External Sources
    size_t externalStart = fds.size();
    for (const auto &source : externalSources_) {
      struct pollfd pfd;
      pfd.fd = source.fd;
      pfd.events = POLLIN;
      pfd.revents = 0;
      fds.push_back(pfd);
    }

    if (fds.empty()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(16));
      continue;
    }

    // Determine poll timeout.
    // If any window has a pending redraw OR is waiting for a frame callback,
    // we should poll with a very short timeout (e.g., 1ms) to keep the loop
    // tight. Otherwise, we can wait longer (e.g., 100ms) to save CPU.
    bool busy = false;
    bool pending = false; // Renamed from 'busy' to 'pending' for clarity
    for (OSFWindow *window : activeWindows) {
      if (window->needsRedraw()) { // Check if any window needs a redraw
        pending = true;
        break;
      }
    }
    // Poll all descriptors
    // If waiting for frame (framePending), use fail-safe timeout (e.g. 16ms)
    // to prevent freezing if callback is lost.
    int timeout = pending ? 16 : 100;

    int result = poll(fds.data(), fds.size(), timeout);

    if (result > 0) {
      // Handle Windows Input
      for (size_t i = 0; i < activeWindows.size(); ++i) {
        if (fds[i].revents & (POLLIN | POLLHUP | POLLERR)) {
          if (!activeWindows[i]->processEvents()) {
            // If processEvents returns false, the window might be
            // invalid/closed. Remove it from activeWindows. The fds vector will
            // be rebuilt next iteration.
            activeWindows.erase(activeWindows.begin() + i);
            i--; // Adjust index as element was removed
          }
        }
      }

      // Handle External Sources
      // Use an iterator to allow removal if a source becomes invalid
      for (auto it = externalSources_.begin(); it != externalSources_.end();) {
        size_t fdIndex =
            externalStart + std::distance(externalSources_.begin(), it);
        if (fdIndex < fds.size() && (fds[fdIndex].revents & POLLIN)) {
          it->callback();
          ++it; // Move to next if callback handled
        } else {
          ++it; // Move to next if not ready or error
        }
      }
    } else if (result == 0) {
      // Timeout (100ms or 16ms)
      // If we were waiting for a frame and timed out (16ms),
      // Force a redraw check to keep animation moving.
      if (pending) {
        // Force updates if stuck?
        // Actually, OSFWindow::update() will skip if framePending && strict.
        // But we relaxed strictness.
      }
    }

    // ALWAYS perform update/animation tick for all windows
    // This drives the "Game Loop"
    for (OSFWindow *window : activeWindows) {
      // Force 'needsRedraw' if we want continuous animation (e.g. progress
      // bars) For now, let's just call update(). If OSFWindow thinks it's done,
      // it returns. But for Phase 3 (which might be animating), we might need
      // to Poke it.
      window->update();
    }
  }

  if (onTerminate_) {
    onTerminate_();
  }
}

void OSFApplication::stop() {
  running_ = false;
  runLoop_.stop();
}

// === Window Management ===

void OSFApplication::addExternalEventSource(int fd,
                                            std::function<void()> callback) {
  if (fd < 0 || !callback)
    return;
  externalSources_.push_back({fd, callback});
}

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

// === First Responder ===

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
