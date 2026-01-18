#pragma once

#include <functional>
#include <memory>
#include <string>

namespace OpenSEF {

/**
 * AnimationEngine - Boot-to-Shutdown Unified Visual System
 *
 * Provides consistent, GPU-accelerated animations across the entire
 * system lifecycle. This is NOT Plymouth - this is a Qt Quick renderer
 * that dominates the user's display from power-on to power-off.
 *
 * Scope:
 *  - Boot splash (VitusOS | ARES logo + progress)
 *  - Onboarding wizard animations
 *  - Desktop transitions
 *  - Shutdown sequence
 *
 * Architecture:
 *  - Renders via Qt Quick (GPU-accelerated)
 *  - Monitors systemd targets for REAL progress
 *  - Hides all systemd logs/vomit
 *  - Seamless transitions (same rendering context)
 */
class AnimationEngine {
public:
  static AnimationEngine &shared();

  // Boot Sequence
  void playBootSequence();
  void updateBootProgress(float progress); // 0.0 to 1.0
  void transitionToDesktop();
  void transitionToOnboarding(); // First boot

  // Desktop Transitions & Window Animations
  void fadeInWindow(void *windowHandle);
  void fadeOutWindow(void *windowHandle);
  void playWindowMinimize(void *windowHandle);
  void playWindowMaximize(void *windowHandle);

  // Tiling Animations (Hyprland-style smooth snapping)
  struct TilingFrame {
    float x, y, width, height;
  };
  void playWindowTileAnimation(void *windowHandle, TilingFrame targetFrame);

  // Shutdown Sequence
  void playShutdownSequence();

  // Progress Tracking Integration
  using ProgressCallback = std::function<float()>;
  void setProgressProvider(ProgressCallback callback);

  // State
  bool isBootSequenceActive() const;
  bool isShutdownSequenceActive() const;
  float getCurrentProgress() const;
  float querySystemdProgress(); // Query real systemd state

  AnimationEngine(const AnimationEngine &) = delete;
  AnimationEngine &operator=(const AnimationEngine &) = delete;

  ~AnimationEngine(); // Public destructor for unique_ptr

private:
  friend class OSFDesktop; // Allow construction

  AnimationEngine(); // Private constructor - only OSFDesktop can create

  struct Implementation;
  std::unique_ptr<Implementation> impl_;
};

} // namespace OpenSEF
