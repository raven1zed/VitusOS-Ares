/**
 * OSFAnimation.h - Core Animation Framework
 *
 * Provides smooth animations for all openSEF UI elements.
 * Part of opensef-base.
 */

#pragma once

#include <chrono>
#include <cmath>
#include <functional>
#include <memory>
#include <opensef/OSFGeometry.h>
#include <string>
#include <unordered_map>
#include <vector>


namespace opensef {

// =============================================================================
// Animation Timing (Ares Design Spec)
// =============================================================================

namespace AnimationTiming {
constexpr double Instant = 0.1;  // 100ms - Button press, hover
constexpr double Quick = 0.2;    // 200ms - Menu open, tooltip
constexpr double Standard = 0.3; // 300ms - Window minimize, drawer
constexpr double Slow = 0.5;     // 500ms - Workspace switch
} // namespace AnimationTiming

// =============================================================================
// Easing Functions
// =============================================================================

namespace Easing {

// ease-out: Decelerating (most common)
inline double easeOut(double t) {
  return 1.0 - (1.0 - t) * (1.0 - t) * (1.0 - t);
}

// ease-in-out: Smooth both ends
inline double easeInOut(double t) {
  return t < 0.5 ? 4.0 * t * t * t
                 : 1.0 - (-2.0 * t + 2.0) * (-2.0 * t + 2.0) *
                             (-2.0 * t + 2.0) / 2.0;
}

// ease-in: Accelerating
inline double easeIn(double t) { return t * t * t; }

// Linear
inline double linear(double t) { return t; }

// Spring (for bouncy effects)
inline double spring(double t) {
  constexpr double damping = 0.7;
  constexpr double frequency = 3.5;
  return 1.0 -
         std::exp(-damping * t * 10) * std::cos(frequency * t * 3.14159 * 2);
}

} // namespace Easing

using EasingFunction = std::function<double(double)>;

// =============================================================================
// OSFAnimation - Base Animation Class
// =============================================================================

class OSFAnimation {
public:
  using CompletionCallback = std::function<void()>;
  using Clock = std::chrono::steady_clock;

  OSFAnimation();
  virtual ~OSFAnimation() = default;

  // Configuration
  void setDuration(double seconds);
  void setEasing(EasingFunction easing);
  void setDelay(double seconds);
  void setRepeatCount(int count); // -1 for infinite
  void setAutoReverse(bool reverse);

  // Lifecycle
  void start();
  void pause();
  void resume();
  void stop();
  bool isRunning() const { return running_; }
  bool isComplete() const { return complete_; }

  // Callbacks
  void onComplete(CompletionCallback cb) { completionCallback_ = cb; }
  void onUpdate(std::function<void(double)> cb) { updateCallback_ = cb; }

  // Called every frame by the animation manager
  virtual void tick();

  // Get current progress (0.0 to 1.0, with easing applied)
  double progress() const { return progress_; }
  double rawProgress() const { return rawProgress_; }

protected:
  virtual void applyProgress(double progress) = 0;

  double duration_ = AnimationTiming::Standard;
  double delay_ = 0.0;
  EasingFunction easing_ = Easing::easeOut;
  int repeatCount_ = 0;
  bool autoReverse_ = false;

  bool running_ = false;
  bool complete_ = false;
  bool paused_ = false;

  Clock::time_point startTime_;
  double progress_ = 0.0;
  double rawProgress_ = 0.0;
  int currentRepeat_ = 0;
  bool reversing_ = false;

  CompletionCallback completionCallback_;
  std::function<void(double)> updateCallback_;

  friend class OSFTransaction;
};

// =============================================================================
// OSFPropertyAnimation - Animates a single numeric value
// =============================================================================

class OSFPropertyAnimation : public OSFAnimation {
public:
  OSFPropertyAnimation(double *target, double from, double to);

  static std::shared_ptr<OSFPropertyAnimation> create(double *target,
                                                      double from, double to);

protected:
  void applyProgress(double progress) override;

private:
  double *target_;
  double from_;
  double to_;
};

// =============================================================================
// OSFKeyframeAnimation - Animates through keyframes
// =============================================================================

struct Keyframe {
  double time; // 0.0 to 1.0
  double value;
  EasingFunction easing = Easing::linear;
};

class OSFKeyframeAnimation : public OSFAnimation {
public:
  OSFKeyframeAnimation(double *target);

  void addKeyframe(double time, double value,
                   EasingFunction easing = Easing::linear);

  static std::shared_ptr<OSFKeyframeAnimation> create(double *target);

protected:
  void applyProgress(double progress) override;

private:
  double *target_;
  std::vector<Keyframe> keyframes_;
};

// =============================================================================
// OSFTransaction - Batch Animation Updates (like CATransaction)
// =============================================================================

class OSFTransaction {
public:
  // Begin a new transaction block
  static void begin();

  // Commit pending changes and start animations
  static void commit();

  // Set animation duration for this transaction
  static void setAnimationDuration(double seconds);

  // Set easing for this transaction
  static void setAnimationEasing(EasingFunction easing);

  // Disable animations for this transaction
  static void setDisableActions(bool disable);

  // Completion handler for all animations in this transaction
  static void setCompletionBlock(std::function<void()> block);

private:
  struct TransactionState {
    double duration = AnimationTiming::Standard;
    EasingFunction easing = Easing::easeOut;
    bool disableActions = false;
    std::function<void()> completionBlock;
    std::vector<std::shared_ptr<OSFAnimation>> animations;
  };

  static std::vector<TransactionState> &transactionStack();
  static TransactionState &current();

  friend class OSFAnimationManager;
};

// =============================================================================
// OSFAnimationManager - Global Animation Coordinator
// =============================================================================

class OSFAnimationManager {
public:
  static OSFAnimationManager &shared();

  // Add animation to be managed
  void addAnimation(std::shared_ptr<OSFAnimation> animation);

  // Remove completed animations
  void removeCompletedAnimations();

  // Called every frame to update all animations
  void tick();

  // Check if any animations are running
  bool hasActiveAnimations() const;

private:
  OSFAnimationManager() = default;
  std::vector<std::shared_ptr<OSFAnimation>> animations_;
};

} // namespace opensef
