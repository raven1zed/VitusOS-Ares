/**
 * OSFAnimations.cpp - Compositor-Level Window Animations
 */

#include "OSFAnimations.h"
#include "OSFCompositor.h"
#include "OSFView.h"

#include <algorithm>
#include <iostream>

namespace opensef {

OSFAnimationManager::OSFAnimationManager(OSFCompositor *compositor)
    : compositor_(compositor) {}

void OSFAnimationManager::startMinimize(OSFView *view, float dockX,
                                        float dockY) {
  ViewAnimation anim;
  anim.type = AnimationType::MINIMIZE;
  anim.active = true;
  anim.startTime = ViewAnimation::Clock::now();
  anim.durationMs = AresAnimations::DurationStandard; // 300ms

  // Start from current state
  anim.startScale = 1.0f;
  anim.startAlpha = 1.0f;
  anim.startX = view->x();
  anim.startY = view->y();

  // Target: scaled down, faded, moved toward dock
  anim.targetScale = 0.1f; // Scale down to 10%
  anim.targetAlpha = 0.0f; // Fade out completely
  anim.targetX = dockX;
  anim.targetY = dockY;

  // Smooth transition
  anim.easing = Easing::easeInOut;

  // Current = start
  anim.scale = anim.startScale;
  anim.alpha = anim.startAlpha;
  anim.x = anim.startX;
  anim.y = anim.startY;

  animations_.push_back({view, anim});
  std::cout << "[openSEF] Starting minimize animation" << std::endl;
}

void OSFAnimationManager::startMaximize(OSFView *view) {
  ViewAnimation anim;
  anim.type = AnimationType::MAXIMIZE;
  anim.active = true;
  anim.startTime = ViewAnimation::Clock::now();
  anim.durationMs = AresAnimations::DurationQuick; // 200ms

  // Start from current state
  anim.startScale = 1.0f;
  anim.startAlpha = 1.0f;
  anim.startX = view->x();
  anim.startY = view->y();

  // Target: full scale (maximized handled by compositor)
  anim.targetScale = 1.0f;
  anim.targetAlpha = 1.0f;
  anim.targetX = 0;  // Maximized windows start at 0,0
  anim.targetY = 32; // Below menu bar (if present)

  anim.easing = Easing::easeOut;

  anim.scale = anim.startScale;
  anim.alpha = anim.startAlpha;
  anim.x = anim.startX;
  anim.y = anim.startY;

  animations_.push_back({view, anim});
  std::cout << "[openSEF] Starting maximize animation" << std::endl;
}

void OSFAnimationManager::startRestore(OSFView *view) {
  ViewAnimation anim;
  anim.type = AnimationType::RESTORE;
  anim.active = true;
  anim.startTime = ViewAnimation::Clock::now();
  anim.durationMs = AresAnimations::DurationStandard;

  // Start from minimized state
  anim.startScale = 0.1f;
  anim.startAlpha = 0.0f;

  // Target: normal
  anim.targetScale = 1.0f;
  anim.targetAlpha = 1.0f;

  anim.easing = Easing::easeOut;

  anim.scale = anim.startScale;
  anim.alpha = anim.startAlpha;

  animations_.push_back({view, anim});
  std::cout << "[openSEF] Starting restore animation" << std::endl;
}

void OSFAnimationManager::startClose(OSFView *view) {
  ViewAnimation anim;
  anim.type = AnimationType::CLOSE;
  anim.active = true;
  anim.startTime = ViewAnimation::Clock::now();
  anim.durationMs = AresAnimations::DurationQuick; // 200ms

  anim.startScale = 1.0f;
  anim.startAlpha = 1.0f;

  // Scale down slightly and fade
  anim.targetScale = 0.95f;
  anim.targetAlpha = 0.0f;

  anim.easing = Easing::easeIn;

  anim.scale = anim.startScale;
  anim.alpha = anim.startAlpha;

  animations_.push_back({view, anim});
  std::cout << "[openSEF] Starting close animation" << std::endl;
}

void OSFAnimationManager::startFocus(OSFView *view) {
  ViewAnimation anim;
  anim.type = AnimationType::FOCUS;
  anim.active = true;
  anim.startTime = ViewAnimation::Clock::now();
  anim.durationMs = AresAnimations::DurationInstant; // 100ms

  // Brief scale pulse
  anim.startScale = 1.0f;
  anim.targetScale = 1.02f; // Slight grow then back

  anim.easing = Easing::easeOut;
  anim.scale = anim.startScale;
  anim.alpha = 1.0f;

  animations_.push_back({view, anim});
}

void OSFAnimationManager::tick() {
  auto now = ViewAnimation::Clock::now();

  for (auto it = animations_.begin(); it != animations_.end();) {
    auto &[view, anim] = *it;

    if (!anim.active) {
      it = animations_.erase(it);
      continue;
    }

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       now - anim.startTime)
                       .count();

    float progress =
        static_cast<float>(elapsed) / static_cast<float>(anim.durationMs);
    progress = std::clamp(progress, 0.0f, 1.0f);

    // Apply easing
    float t = anim.easing(progress);

    // Interpolate values
    anim.scale = anim.startScale + (anim.targetScale - anim.startScale) * t;
    anim.alpha = anim.startAlpha + (anim.targetAlpha - anim.startAlpha) * t;
    anim.x = anim.startX + (anim.targetX - anim.startX) * t;
    anim.y = anim.startY + (anim.targetY - anim.startY) * t;

    // Apply to view
    applyAnimation(view, anim);

    // Check if complete
    if (progress >= 1.0f) {
      anim.active = false;

      if (anim.type == AnimationType::FOCUS) {
        // Reset scale after focus pulse
        anim.scale = 1.0f;
        applyAnimation(view, anim);
      }
    }

    ++it;
  }
}

ViewAnimation *OSFAnimationManager::getAnimation(OSFView *view) {
  for (auto &[v, anim] : animations_) {
    if (v == view) {
      return &anim;
    }
  }
  return nullptr;
}

bool OSFAnimationManager::hasActiveAnimations() const {
  return std::any_of(animations_.begin(), animations_.end(),
                     [](const auto &pair) { return pair.second.active; });
}

void OSFAnimationManager::updateAnimation(ViewAnimation &anim) {
  (void)anim;
  // Called by tick() - see implementation above
}

void OSFAnimationManager::applyAnimation(OSFView *view,
                                         const ViewAnimation &anim) {
  // Apply the animation transforms to the view's scene node
  // This would set position, scale, and alpha on the wlr_scene_node

  if (view && view->sceneTree()) {
    // Position
    wlr_scene_node_set_position(&view->sceneTree()->node,
                                static_cast<int>(anim.x),
                                static_cast<int>(anim.y));

    // TODO: Scale and alpha require wlr_scene transformations
    // For now, visibility is controlled by enabled state
    if (anim.alpha <= 0.01f && anim.type == AnimationType::MINIMIZE) {
      wlr_scene_node_set_enabled(&view->sceneTree()->node, false);
    } else if (anim.type == AnimationType::RESTORE && anim.alpha > 0.5f) {
      wlr_scene_node_set_enabled(&view->sceneTree()->node, true);
    }
  }
}

} // namespace opensef
