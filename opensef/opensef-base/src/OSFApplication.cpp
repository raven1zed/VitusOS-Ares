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

    // Poll all descriptors with 16ms timeout (~60fps)
    int result = poll(fds.data(), fds.size(), 16);

    if (result > 0) {
      // Handle Windows
      for (size_t i = 0; i < activeWindows.size(); ++i) {
        if (fds[i].revents & POLLIN) {
          if (!activeWindows[i]->processEvents()) {
            // Window closed or error
          }
        }
      }

      // Handle External Sources
      for (size_t i = 0; i < externalSources_.size(); ++i) {
        size_t fdIndex = externalStart + i;
        if (fdIndex < fds.size() && (fds[fdIndex].revents & POLLIN)) {
          externalSources_[i].callback();
        }
      }
    } else if (result == 0) {
      // Timeout - animation tick for windows
      for (OSFWindow *window : activeWindows) {
        window->processEvents();
      }
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
 *
 * Phase 3: REAL unified event loop that polls all registered windows.
 * This is the single entry point for openSEF applications.
 */

#include <opensef/OSFResponder.h>
#include <opensef/OSFWindow.h>
#include <opensef/OpenSEFBase.h>

#include <algorithm>
#include <chrono>
<<<<<<< HEAD
#include <iostream>
#include <poll.h>
#include <vector>


=======
#include <poll.h>
#include <vector>

>>>>>>> dd28457a865c4fd5064c4934eaa5b6bd2bc63253
namespace opensef {

   OSFApplication &OSFApplication::shared() {
     static OSFApplication app;
     return app;
   }

   void OSFApplication::run() {
     if (onLaunch_) {
       onLaunch_();
     }

<<<<<<< HEAD
     == == == =
                  // Phase 3: REAL Unified Event Loop
                  // Poll all registered windows for Wayland events

>>>>>>> dd28457a865c4fd5064c4934eaa5b6bd2bc63253
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
<<<<<<< HEAD
       == == == =
                    // Note where external sources start in fds array
>>>>>>> dd28457a865c4fd5064c4934eaa5b6bd2bc63253
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

       // Poll all descriptors with 16ms timeout (~60fps)
       int result = poll(fds.data(), fds.size(), 16);

       if (result > 0) {
         // Handle Windows
         for (size_t i = 0; i < activeWindows.size(); ++i) {
           if (fds[i].revents & POLLIN) {
             if (!activeWindows[i]->processEvents()) {
               // Window closed or error
             }
           }
         }

         // Handle External Sources
         for (size_t i = 0; i < externalSources_.size(); ++i) {
           size_t fdIndex = externalStart + i;
           if (fdIndex < fds.size() && (fds[fdIndex].revents & POLLIN)) {
             externalSources_[i].callback();
           }
         }
       } else if (result == 0) {
<<<<<<< HEAD
         // Timeout - animation tick for windows
         for (OSFWindow *window : activeWindows) {
           window->processEvents();
         }
       }
       == == == =
                    // Timeout - render windows (animation loop)
           for (OSFWindow *window : activeWindows) {
         window->processEvents();
       }
       // Trigger external sources 'tick' if needed?
       // For now, assume they rely on their own internal timers via
       // dispatchTimers or we can manually invoke them.
       for (const auto &source : externalSources_) {
         // If we want to simulate a "frame tick" for them, we might need a
         // separate callback. For now, let's assume they set up a timer FD if
         // they need precise timing, OR we just rely on display updates. BUT
         // OSFSurface has dispatchTimers() which checks internal timers. We
         // should probably call the callback on timeout too if it's meant to
         // drive the loop. Let's stick to FD readiness for now.
       }
     }

     // Process runloop tasks
>>>>>>> dd28457a865c4fd5064c4934eaa5b6bd2bc63253
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

<<<<<<< HEAD
 == == == =
              // === Window Management ===

>>>>>>> dd28457a865c4fd5064c4934eaa5b6bd2bc63253
     void OSFApplication::registerWindow(OSFWindow * window) {
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
