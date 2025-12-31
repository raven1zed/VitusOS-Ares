/**
 * OSFDisplayLink.cpp - Frame-Synchronized Updates Implementation
 */

#include "OSFDisplayLink.h"
#include <numeric>

namespace opensef {

// ============================================================================
// OSFDisplayLink Implementation
// ============================================================================

OSFDisplayLink::OSFDisplayLink(Callback callback) : callback_(callback) {}

OSFDisplayLink::~OSFDisplayLink() {}

void OSFDisplayLink::tick(float timestamp) {
  if (paused_)
    return;

  if (firstFrame_) {
    lastTimestamp_ = timestamp;
    lastDeltaTime_ = 1.0f / 60.0f; // Assume 60fps initially
    firstFrame_ = false;
  } else {
    lastDeltaTime_ = timestamp - lastTimestamp_;
    lastTimestamp_ = timestamp;
  }

  // Clamp delta time to prevent huge jumps
  if (lastDeltaTime_ > 0.1f) {
    lastDeltaTime_ = 0.1f;
  }
  if (lastDeltaTime_ < 0.0f) {
    lastDeltaTime_ = 1.0f / 60.0f;
  }

  // Track for FPS calculation
  frameTimes_[frameIndex_] = lastDeltaTime_;
  frameIndex_ = (frameIndex_ + 1) % FPS_SAMPLE_COUNT;

  // Invoke callback
  if (callback_) {
    callback_(timestamp, lastDeltaTime_);
  }
}

float OSFDisplayLink::averageFPS() const {
  float sum = 0.0f;
  int count = 0;

  for (int i = 0; i < FPS_SAMPLE_COUNT; i++) {
    if (frameTimes_[i] > 0.0f) {
      sum += frameTimes_[i];
      count++;
    }
  }

  if (count == 0 || sum == 0.0f) {
    return 60.0f; // Default
  }

  float avgFrameTime = sum / static_cast<float>(count);
  return 1.0f / avgFrameTime;
}

// ============================================================================
// OSFDisplayLinkManager Implementation
// ============================================================================

OSFDisplayLinkManager &OSFDisplayLinkManager::instance() {
  static OSFDisplayLinkManager instance;
  return instance;
}

size_t OSFDisplayLinkManager::addCallback(OSFDisplayLink::Callback callback) {
  Entry entry;
  entry.handle = nextHandle_++;
  entry.callback = callback;
  entries_.push_back(entry);
  return entry.handle;
}

void OSFDisplayLinkManager::removeCallback(size_t handle) {
  for (auto it = entries_.begin(); it != entries_.end(); ++it) {
    if (it->handle == handle) {
      entries_.erase(it);
      return;
    }
  }
}

void OSFDisplayLinkManager::tick(float timestamp) {
  float dt;

  if (firstFrame_) {
    lastTimestamp_ = timestamp;
    dt = 1.0f / 60.0f;
    firstFrame_ = false;
  } else {
    dt = timestamp - lastTimestamp_;
    lastTimestamp_ = timestamp;
  }

  // Clamp
  if (dt > 0.1f)
    dt = 0.1f;
  if (dt < 0.0f)
    dt = 1.0f / 60.0f;

  // Invoke all callbacks
  for (auto &entry : entries_) {
    if (!entry.paused && entry.callback) {
      entry.callback(timestamp, dt);
    }
  }
}

} // namespace opensef
