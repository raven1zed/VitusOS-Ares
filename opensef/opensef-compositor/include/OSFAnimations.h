/**
 * OSFAnimations.h - Compositor-Level Window Animations
 *
 * Implements smooth window animations per the Ares design spec:
 * - Minimize: 300ms ease-in-out, scale + fade toward dock
 * - Maximize: 200ms ease-out, scale from current to full
 * - Close: 200ms ease-in, scale down + fade
 */

#pragma once

#include <chrono>
#include <cstdint>
#include <functional>
#include <utility>
#include <vector>

namespace opensef {

// Animation timing functions (from Design Reference)
namespace AresAnimations {
// Duration in milliseconds
constexpr int DurationInstant = 100;  // Button press, hover
constexpr int DurationQuick = 200;    // Menu open, tooltip, close
constexpr int DurationStandard = 300; // Window minimize, drawer slide
constexpr int DurationSlow = 500;     // Workspace switch
} // namespace AresAnimations

// Easing functions
namespace Easing {
// ease-out: cubic-bezier(0.0, 0.0, 0.2, 1) - Decelerating
inline float easeOut(float t) {
  return 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t);
}

// ease-in-out: cubic-bezier(0.4, 0.0, 0.2, 1) - Smooth both ends
inline float easeInOut(float t) {
  return t < 0.5f ? 4.0f * t * t * t
                  : 1.0f - (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) *
                               (-2.0f * t + 2.0f) / 2.0f;
}

// ease-in: Used for close animation
inline float easeIn(float t) { return t * t * t; }

// Linear
inline float linear(float t) { return t; }
} // namespace Easing

// Animation types
enum class AnimationType {
  NONE,
  MINIMIZE,
  MAXIMIZE,
  RESTORE,
  CLOSE,
  FOCUS,    // Brief pulse when gaining focus
  WORKSPACE // Sliding to another workspace
};

// Animation state for a view
struct ViewAnimation {
  AnimationType type = AnimationType::NONE;
  bool active = false;

  // Timing
  using Clock = std::chrono::steady_clock;
  Clock::time_point startTime;
  int durationMs = 0;

  // Transform values (current)
  float scale = 1.0f;
  float alpha = 1.0f;
  float x = 0.0f, y = 0.0f; // Position offset

  // Target values
  float targetScale = 1.0f;
  float targetAlpha = 1.0f;
  float targetX = 0.0f, targetY = 0.0f;

  // Starting values (for interpolation)
  float startScale = 1.0f;
  float startAlpha = 1.0f;
  float startX = 0.0f, startY = 0.0f;

  // Easing function
  std::function<float(float)> easing = Easing::easeOut;
};

// Forward declarations
class OSFView;
class OSFCompositor;

/**
 * OSFAnimationManager - Manages all compositor animations
 */
class OSFAnimationManager {
public:
  OSFAnimationManager(OSFCompositor *compositor);

  // Start animations
  void startMinimize(OSFView *view, float dockX, float dockY);
  void startMaximize(OSFView *view);
  void startRestore(OSFView *view);
  void startClose(OSFView *view);
  void startFocus(OSFView *view);

  // Update all active animations (call every frame)
  void tick();

  // Get animation state for a view
  ViewAnimation *getAnimation(OSFView *view);

  // Check if any animations are active
  bool hasActiveAnimations() const;

private:
  OSFCompositor *compositor_;
  std::vector<std::pair<OSFView *, ViewAnimation>> animations_;

  void updateAnimation(ViewAnimation &anim);
  void applyAnimation(OSFView *view, const ViewAnimation &anim);
};

} // namespace opensef
