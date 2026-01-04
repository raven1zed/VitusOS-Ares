/**
 * openSEF Base: Foundation Layer (C++ Version)
 *
 * Open SeagrEnv Framework - Core object system
 * Originally designed for GNUstep, now pure C++ for portability
 */

#pragma once

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace opensef {

// ============================================================================
// OSFObject - Root class for all openSEF objects
// ============================================================================

class OSFObject {
public:
  OSFObject();
  virtual ~OSFObject();

  /** Unique identifier for this object */
  const std::string &objectID() const { return objectID_; }

  /** Create a new openSEF object */
  static std::shared_ptr<OSFObject> create();

protected:
  std::string objectID_;

private:
  static std::atomic<uint64_t> nextID_;
};

// ============================================================================
// OSFString - Enhanced string (thin wrapper for now)
// ============================================================================

using OSFString = std::string;

// ============================================================================
// OSFArray - Enhanced array (thin wrapper for now)
// ============================================================================

template <typename T> using OSFArray = std::vector<T>;

// ============================================================================
// OSFRunLoop - Basic task runner
// ============================================================================

class OSFRunLoop {
public:
  using Task = std::function<void()>;
  friend class OSFApplication;

  static OSFRunLoop &main();

  void run();
  void stop();
  bool isRunning() const { return running_; }

  void postTask(Task task);

private:
  OSFRunLoop() = default;

  std::mutex mutex_;
  std::condition_variable cv_;
  std::deque<Task> tasks_;
  bool running_ = false;
};

// ============================================================================
// OSFNotificationCenter - Publish/subscribe notifications
// ============================================================================

class OSFNotificationCenter {
public:
  using Callback = std::function<void()>;

  struct ObserverToken {
    std::string name;
    std::size_t id = 0;
  };

  static OSFNotificationCenter &defaultCenter();

  ObserverToken addObserver(const std::string &name, Callback callback);
  void removeObserver(const ObserverToken &token);
  void postNotification(const std::string &name);

private:
  OSFNotificationCenter() = default;

  std::mutex mutex_;
  std::unordered_map<std::string, std::vector<std::pair<std::size_t, Callback>>>
      observers_;
  std::size_t nextObserverID_ = 1;
};

// ============================================================================
// OSFBundle - Resource bundle metadata
// ============================================================================

class OSFBundle {
public:
  OSFBundle() = default;
  OSFBundle(std::string identifier, std::string resourcePath);

  static OSFBundle mainBundle();

  const std::string &identifier() const { return identifier_; }
  const std::string &resourcePath() const { return resourcePath_; }

private:
  std::string identifier_;
  std::string resourcePath_;
};

// ============================================================================
// OSFApplication - Application lifecycle and event coordination
// ============================================================================

// Forward declaration
class OSFWindow;
class OSFResponder;

class OSFApplication {
public:
  using Callback = std::function<void()>;

  static OSFApplication &shared();

  void setOnLaunch(Callback callback) { onLaunch_ = std::move(callback); }
  void setOnTerminate(Callback callback) { onTerminate_ = std::move(callback); }

  /**
   * Main entry point. Runs the application event loop.
   * Polls all registered windows for Wayland events.
   */
  void run();

  /**
   * Stop the application event loop.
   */
  void stop();

  OSFRunLoop &runLoop() { return runLoop_; }

  // === Window Management (Phase 3) ===

  /**
   * Register a window with the application.
   * The app will poll this window's display in the event loop.
   */
  void registerWindow(OSFWindow *window);

  /**
   * Unregister a window from the application.
   */
  void unregisterWindow(OSFWindow *window);

  /**
   * Get all registered windows.
   */
  const std::vector<OSFWindow *> &windows() const { return windows_; }

  /**
   * Add a custom file descriptor to poll.
   * Useful for integrating external event loops (e.g. OSFSurface/LayerShell).
   */
  void addExternalEventSource(int fd, std::function<void()> callback);

  // === First Responder (Phase 3) ===

  /**
   * Get the current first responder (receives keyboard events).
   */
  OSFResponder *firstResponder() const { return firstResponder_; }

  /**
   * Make a responder the first responder.
   */
  bool makeFirstResponder(OSFResponder *responder);

private:
  OSFApplication() = default;

  OSFRunLoop runLoop_;
  Callback onLaunch_;
  Callback onTerminate_;
  bool running_ = false;

  // Phase 3 additions
  std::vector<OSFWindow *> windows_;
  OSFResponder *firstResponder_ = nullptr;

  struct ExternalSource {
    int fd;
    std::function<void()> callback;
  };
  std::vector<ExternalSource> externalSources_;
};

} // namespace opensef
