/**
 * OSFTransaction.cpp - Animation Transaction Implementation
 */

#include "OSFTransaction.h"
#include <iostream>

namespace opensef {

// Thread-local transaction stack
thread_local std::vector<OSFTransaction *> OSFTransaction::stack_;

OSFTransaction::OSFTransaction() {
  // Default values set in header
}

OSFTransaction::~OSFTransaction() {
  // Completion block handled in commit()
}

void OSFTransaction::begin() { stack_.push_back(new OSFTransaction()); }

void OSFTransaction::commit() {
  if (stack_.empty()) {
    std::cerr << "[openSEF] Warning: commit() called without begin()"
              << std::endl;
    return;
  }

  OSFTransaction *t = stack_.back();
  stack_.pop_back();

  // Execute completion block if set
  if (t->completionBlock_) {
    // In a real implementation, this would be scheduled after animations
    // complete For now, execute immediately
    t->completionBlock_();
  }

  delete t;
}

void OSFTransaction::flush() {
  while (!stack_.empty()) {
    commit();
  }
}

OSFTransaction *OSFTransaction::current() {
  // Create implicit transaction if none exists
  if (stack_.empty()) {
    begin();
  }
  return stack_.back();
}

void OSFTransaction::setAnimationDuration(float seconds) {
  current()->duration_ = seconds;
}

float OSFTransaction::animationDuration() {
  if (stack_.empty()) {
    return 0.25f; // Default duration
  }
  return stack_.back()->duration_;
}

void OSFTransaction::setDisableActions(bool disable) {
  current()->disableActions_ = disable;
}

bool OSFTransaction::animationsDisabled() {
  if (stack_.empty()) {
    return false;
  }
  return stack_.back()->disableActions_;
}

void OSFTransaction::setCompletionBlock(std::function<void()> block) {
  current()->completionBlock_ = block;
}

void OSFTransaction::setTimingFunction(TimingFunction function) {
  current()->timingFunction_ = function;
}

OSFTransaction::TimingFunction OSFTransaction::timingFunction() {
  if (stack_.empty()) {
    return TimingFunction::EaseInOut;
  }
  return stack_.back()->timingFunction_;
}

} // namespace opensef
