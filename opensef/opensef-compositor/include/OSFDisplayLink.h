/**
 * OSFDisplayLink.h - Frame-Synchronized Updates
 *
 * Based on Core Animation CADisplayLink pattern from R&D research.
 * Provides callbacks synchronized to the display refresh rate.
 *
 * Usage:
 *   auto displayLink = OSFDisplayLink([](float ts, float dt) {
 *     // Update animations, physics, etc.
 *     animationManager->update(dt);
 *   });
 *
 *   // In frame callback:
 *   displayLink.tick(timestamp);
 */

#pragma once

#include <functional>
#include <vector>

namespace opensef {

/**
 * OSFDisplayLink - Frame-synchronized update callback
 */
class OSFDisplayLink {
public:
  using Callback = std::function<void(float timestamp, float deltaTime)>;

  OSFDisplayLink(Callback callback);
  ~OSFDisplayLink();

  /**
   * Called each frame with the current timestamp.
   * Calculates delta time and invokes the callback.
   */
  void tick(float timestamp);

  /**
   * Pause/resume the display link.
   */
  void setPaused(bool paused) { paused_ = paused; }
  bool isPaused() const { return paused_; }

  /**
   * Get frame statistics.
   */
  float lastDeltaTime() const { return lastDeltaTime_; }
  float averageFPS() const;

private:
  Callback callback_;
  bool paused_ = false;
  float lastTimestamp_ = 0.0f;
  float lastDeltaTime_ = 0.0f;
  bool firstFrame_ = true;

  // FPS tracking
  static constexpr int FPS_SAMPLE_COUNT = 60;
  float frameTimes_[FPS_SAMPLE_COUNT] = {0};
  int frameIndex_ = 0;
};

/**
 * OSFDisplayLinkManager - Manages multiple display links
 */
class OSFDisplayLinkManager {
public:
  static OSFDisplayLinkManager &instance();

  /**
   * Register a display link callback.
   * Returns a handle for removal.
   */
  size_t addCallback(OSFDisplayLink::Callback callback);

  /**
   * Remove a callback by handle.
   */
  void removeCallback(size_t handle);

  /**
   * Tick all registered display links.
   * Call this from the compositor frame callback.
   */
  void tick(float timestamp);

private:
  OSFDisplayLinkManager() = default;

  struct Entry {
    size_t handle;
    OSFDisplayLink::Callback callback;
    bool paused = false;
  };

  std::vector<Entry> entries_;
  size_t nextHandle_ = 1;
  float lastTimestamp_ = 0.0f;
  bool firstFrame_ = true;
};

} // namespace opensef
