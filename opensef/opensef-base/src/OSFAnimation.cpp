/**
 * OSFAnimation.cpp - Core Animation Framework Implementation
 */

#include <algorithm>
#include <opensef/OSFAnimation.h>


namespace opensef {

// =============================================================================
// OSFAnimation
// =============================================================================

OSFAnimation::OSFAnimation() = default;

void OSFAnimation::setDuration(double seconds) { duration_ = seconds; }

void OSFAnimation::setEasing(EasingFunction easing) { easing_ = easing; }

void OSFAnimation::setDelay(double seconds) { delay_ = seconds; }

void OSFAnimation::setRepeatCount(int count) { repeatCount_ = count; }

void OSFAnimation::setAutoReverse(bool reverse) { autoReverse_ = reverse; }

void OSFAnimation::start() {
  running_ = true;
  complete_ = false;
  paused_ = false;
  progress_ = 0.0;
  rawProgress_ = 0.0;
  currentRepeat_ = 0;
  reversing_ = false;
  startTime_ = Clock::now();
}

void OSFAnimation::pause() {
  if (running_ && !paused_) {
    paused_ = true;
  }
}

void OSFAnimation::resume() {
  if (paused_) {
    paused_ = false;
    // Adjust start time to account for paused duration
    startTime_ = Clock::now() - std::chrono::milliseconds(static_cast<int>(
                                    rawProgress_ * duration_ * 1000));
  }
}

void OSFAnimation::stop() {
  running_ = false;
  complete_ = true;
}

void OSFAnimation::tick() {
  if (!running_ || paused_)
    return;

  auto now = Clock::now();
  auto elapsed = std::chrono::duration<double>(now - startTime_).count();

  // Account for delay
  if (elapsed < delay_) {
    return;
  }
  elapsed -= delay_;

  // Calculate raw progress
  rawProgress_ = std::clamp(elapsed / duration_, 0.0, 1.0);

  // Handle auto-reverse
  double effectiveProgress = rawProgress_;
  if (reversing_) {
    effectiveProgress = 1.0 - rawProgress_;
  }

  // Apply easing
  progress_ = easing_(effectiveProgress);

  // Apply to subclass
  applyProgress(progress_);

  // Notify update callback
  if (updateCallback_) {
    updateCallback_(progress_);
  }

  // Check completion
  if (rawProgress_ >= 1.0) {
    if (autoReverse_ && !reversing_) {
      // Start reverse
      reversing_ = true;
      rawProgress_ = 0.0;
      startTime_ = Clock::now();
    } else if (repeatCount_ != 0 &&
               (repeatCount_ < 0 || currentRepeat_ < repeatCount_)) {
      // Repeat
      currentRepeat_++;
      reversing_ = false;
      rawProgress_ = 0.0;
      startTime_ = Clock::now();
    } else {
      // Complete
      running_ = false;
      complete_ = true;
      if (completionCallback_) {
        completionCallback_();
      }
    }
  }
}

// =============================================================================
// OSFPropertyAnimation
// =============================================================================

OSFPropertyAnimation::OSFPropertyAnimation(double *target, double from,
                                           double to)
    : target_(target), from_(from), to_(to) {}

std::shared_ptr<OSFPropertyAnimation>
OSFPropertyAnimation::create(double *target, double from, double to) {
  return std::make_shared<OSFPropertyAnimation>(target, from, to);
}

void OSFPropertyAnimation::applyProgress(double progress) {
  if (target_) {
    *target_ = from_ + (to_ - from_) * progress;
  }
}

// =============================================================================
// OSFKeyframeAnimation
// =============================================================================

OSFKeyframeAnimation::OSFKeyframeAnimation(double *target) : target_(target) {}

void OSFKeyframeAnimation::addKeyframe(double time, double value,
                                       EasingFunction easing) {
  keyframes_.push_back({time, value, easing});
  // Sort by time
  std::sort(
      keyframes_.begin(), keyframes_.end(),
      [](const Keyframe &a, const Keyframe &b) { return a.time < b.time; });
}

std::shared_ptr<OSFKeyframeAnimation>
OSFKeyframeAnimation::create(double *target) {
  return std::make_shared<OSFKeyframeAnimation>(target);
}

void OSFKeyframeAnimation::applyProgress(double progress) {
  if (!target_ || keyframes_.empty())
    return;

  // Find surrounding keyframes
  const Keyframe *prev = nullptr;
  const Keyframe *next = nullptr;

  for (size_t i = 0; i < keyframes_.size(); i++) {
    if (keyframes_[i].time <= progress) {
      prev = &keyframes_[i];
    }
    if (keyframes_[i].time >= progress && !next) {
      next = &keyframes_[i];
      break;
    }
  }

  if (!prev && next) {
    *target_ = next->value;
  } else if (prev && !next) {
    *target_ = prev->value;
  } else if (prev && next && prev != next) {
    // Interpolate between keyframes
    double range = next->time - prev->time;
    double localProgress = (progress - prev->time) / range;
    double easedProgress = next->easing(localProgress);
    *target_ = prev->value + (next->value - prev->value) * easedProgress;
  } else if (prev) {
    *target_ = prev->value;
  }
}

// =============================================================================
// OSFTransaction
// =============================================================================

std::vector<OSFTransaction::TransactionState> &
OSFTransaction::transactionStack() {
  static std::vector<TransactionState> stack;
  return stack;
}

OSFTransaction::TransactionState &OSFTransaction::current() {
  auto &stack = transactionStack();
  if (stack.empty()) {
    // Create implicit transaction
    stack.push_back({});
  }
  return stack.back();
}

void OSFTransaction::begin() { transactionStack().push_back({}); }

void OSFTransaction::commit() {
  auto &stack = transactionStack();
  if (stack.empty())
    return;

  auto state = std::move(stack.back());
  stack.pop_back();

  // Start all animations
  for (auto &anim : state.animations) {
    anim->start();
    OSFAnimationManager::shared().addAnimation(anim);
  }

  // Handle completion block
  if (state.completionBlock && !state.animations.empty()) {
    // Attach to last animation
    auto lastAnim = state.animations.back();
    auto originalCallback = lastAnim->completionCallback_;
    lastAnim->onComplete([originalCallback, cb = state.completionBlock]() {
      if (originalCallback)
        originalCallback();
      cb();
    });
  }
}

void OSFTransaction::setAnimationDuration(double seconds) {
  current().duration = seconds;
}

void OSFTransaction::setAnimationEasing(EasingFunction easing) {
  current().easing = easing;
}

void OSFTransaction::setDisableActions(bool disable) {
  current().disableActions = disable;
}

void OSFTransaction::setCompletionBlock(std::function<void()> block) {
  current().completionBlock = block;
}

// =============================================================================
// OSFAnimationManager
// =============================================================================

OSFAnimationManager &OSFAnimationManager::shared() {
  static OSFAnimationManager instance;
  return instance;
}

void OSFAnimationManager::addAnimation(
    std::shared_ptr<OSFAnimation> animation) {
  animations_.push_back(animation);
}

void OSFAnimationManager::removeCompletedAnimations() {
  animations_.erase(
      std::remove_if(animations_.begin(), animations_.end(),
                     [](const auto &anim) { return anim->isComplete(); }),
      animations_.end());
}

void OSFAnimationManager::tick() {
  if (animations_.empty())
    return;

  for (auto &anim : animations_) {
    anim->tick();
  }
  removeCompletedAnimations();
}

bool OSFAnimationManager::hasActiveAnimations() const {
  return std::any_of(animations_.begin(), animations_.end(),
                     [](const auto &anim) { return anim->isRunning(); });
}

} // namespace opensef
