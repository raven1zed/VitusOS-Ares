/**
 * OSFAnimationEngine.cpp - Boot-to-Shutdown Animation System
 *
 * Implementation of the unified visual system that dominates the display
 * from power-on to power-off. Replaces Plymouth/systemd vomit with
 * smooth, GPU-accelerated Qt Quick rendering.
 *
 * Note: This uses pure C++ (no Qt) for systemd monitoring.
 * Qt Quick rendering is done in shell, not framework.
 */

#include "opensef/OSFAnimationEngine.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#define LOG(msg) std::cout << "[AnimationEngine] " << msg << std::endl

namespace OpenSEF {

struct AnimationEngine::Implementation {
  bool bootSequenceActive = false;
  bool shutdownSequenceActive = false;
  float currentProgress = 0.0f;
  ProgressCallback progressProvider = nullptr;

  // Pure C++ systemd target check (no Qt dependency)
  bool checkSystemdTarget(const char *target) {
    char command[256];
    snprintf(command, sizeof(command), "systemctl is-active %s 2>/dev/null",
             target);

    FILE *pipe = popen(command, "r");
    if (!pipe)
      return false;

    char buffer[64];
    if (fgets(buffer, sizeof(buffer), pipe)) {
      pclose(pipe);
      // Remove trailing newline
      size_t len = strlen(buffer);
      if (len > 0 && buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';
      return strcmp(buffer, "active") == 0;
    }
    pclose(pipe);
    return false;
  }

  float calculateBootProgress() {
    if (progressProvider) {
      return progressProvider();
    }

    // Track actual systemd targets - REAL progress, not fake
    float progress = 0.0f;

    if (checkSystemdTarget("sysinit.target"))
      progress = 0.2f; // 20% - Basic system init
    if (checkSystemdTarget("basic.target"))
      progress = 0.4f; // 40% - Core services
    if (checkSystemdTarget("network.target"))
      progress = 0.5f; // 50% - Network ready
    if (checkSystemdTarget("multi-user.target"))
      progress = 0.6f; // 60% - User services
    if (checkSystemdTarget("graphical.target"))
      progress = 0.85f; // 85% - Graphics ready

    return progress;
  }
};

AnimationEngine &AnimationEngine::shared() {
  static AnimationEngine instance;
  return instance;
}

AnimationEngine::AnimationEngine() : impl_(std::make_unique<Implementation>()) {
  LOG("Initialized - Boot-to-Shutdown visual system");
}

AnimationEngine::~AnimationEngine() = default;

void AnimationEngine::playBootSequence() {
  LOG("Starting boot sequence");
  impl_->bootSequenceActive = true;
  impl_->currentProgress = 0.0f;

  // Boot splash rendering is done in shell via Qt Quick
  // This just manages state and progress tracking
}

void AnimationEngine::updateBootProgress(float progress) {
  impl_->currentProgress = progress;
  std::cout << "[AnimationEngine] Boot progress: " << (progress * 100) << "%"
            << std::endl;
}

void AnimationEngine::transitionToDesktop() {
  LOG("Transition: Boot → Desktop");
  impl_->bootSequenceActive = false;
}

void AnimationEngine::transitionToOnboarding() {
  LOG("Transition: Boot → Onboarding (first boot)");
  impl_->bootSequenceActive = false;
}

void AnimationEngine::fadeInWindow(void *windowHandle) {
  std::cout << "[AnimationEngine] Fade in window: " << windowHandle
            << std::endl;
}

void AnimationEngine::fadeOutWindow(void *windowHandle) {
  std::cout << "[AnimationEngine] Fade out window: " << windowHandle
            << std::endl;
}

void AnimationEngine::playWindowMinimize(void *windowHandle) {
  std::cout << "[AnimationEngine] Window minimize: " << windowHandle
            << std::endl;
}

void AnimationEngine::playWindowMaximize(void *windowHandle) {
  std::cout << "[AnimationEngine] Window maximize: " << windowHandle
            << std::endl;
}

void AnimationEngine::playShutdownSequence() {
  LOG("Starting shutdown sequence");
  impl_->shutdownSequenceActive = true;
}

void AnimationEngine::setProgressProvider(ProgressCallback callback) {
  impl_->progressProvider = callback;
}

bool AnimationEngine::isBootSequenceActive() const {
  return impl_->bootSequenceActive;
}

bool AnimationEngine::isShutdownSequenceActive() const {
  return impl_->shutdownSequenceActive;
}

float AnimationEngine::getCurrentProgress() const {
  return impl_->currentProgress;
}

float AnimationEngine::querySystemdProgress() {
  return impl_->calculateBootProgress();
}

} // namespace OpenSEF
