/**
 * OSFTransaction.h - Animation Transaction System
 *
 * Based on Core Animation CATransaction pattern from R&D research.
 * Allows batching multiple property changes into coordinated animations.
 *
 * Usage:
 *   OSFTransaction::begin();
 *   OSFTransaction::setAnimationDuration(0.3f);
 *   layer->setOpacity(1.0f);   // Animates to 1.0 over 0.3s
 *   layer->setPosition(x, y);  // Animates to position over 0.3s
 *   OSFTransaction::commit();
 */

#pragma once

#include <cstdint>
#include <functional>
#include <vector>

namespace opensef {

/**
 * OSFTransaction - Animation batching and configuration
 *
 * Thread-local transaction stack allows nested transactions.
 * Each transaction can configure animation duration and disable actions.
 */
class OSFTransaction {
public:
  // === Static Transaction Control ===

  /**
   * Begin a new transaction. Must be paired with commit().
   */
  static void begin();

  /**
   * Commit the current transaction and apply all property changes.
   */
  static void commit();

  /**
   * Flush any pending implicit transactions.
   */
  static void flush();

  // === Animation Configuration ===

  /**
   * Set animation duration for current transaction (default: 0.25s)
   */
  static void setAnimationDuration(float seconds);

  /**
   * Get current animation duration.
   */
  static float animationDuration();

  /**
   * Disable implicit animations for current transaction.
   * Property changes will be instant.
   */
  static void setDisableActions(bool disable);

  /**
   * Check if animations are currently disabled.
   */
  static bool animationsDisabled();

  // === Completion Handling ===

  /**
   * Set a completion callback for current transaction.
   * Called when all animations in the transaction complete.
   */
  static void setCompletionBlock(std::function<void()> block);

  // === Timing Function ===

  enum class TimingFunction { Linear, EaseIn, EaseOut, EaseInOut, Spring };

  static void setTimingFunction(TimingFunction function);
  static TimingFunction timingFunction();

private:
  OSFTransaction();
  ~OSFTransaction();

  // Thread-local transaction stack
  static thread_local std::vector<OSFTransaction *> stack_;

  // Transaction state
  float duration_ = 0.25f;
  bool disableActions_ = false;
  TimingFunction timingFunction_ = TimingFunction::EaseInOut;
  std::function<void()> completionBlock_;

  // Get current transaction (creates implicit one if needed)
  static OSFTransaction *current();
};

} // namespace opensef
