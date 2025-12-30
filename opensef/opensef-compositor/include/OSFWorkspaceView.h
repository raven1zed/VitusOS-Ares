/**
 * OSFWorkspaceView.h - Multitask/Workspace Switcher
 *
 * Shows when clicking the orange multitask button:
 * - All open windows as thumbnails
 * - Multiple workspaces/desktops
 * - Click to switch focus or workspace
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace opensef {

// Forward declarations
class OSFCompositor;
class OSFView;

// Workspace containing views
struct Workspace {
  int id;
  std::string name;
  std::vector<OSFView *> views;
};

/**
 * OSFWorkspaceView - Mission Control-like workspace switcher
 */
class OSFWorkspaceView {
public:
  OSFWorkspaceView(OSFCompositor *compositor);

  // Activate/deactivate the overview
  void show();
  void hide();
  bool isVisible() const { return visible_; }

  // Render the workspace view (overlay on top of everything)
  void render(uint32_t *buffer, int screenW, int screenH);

  // Handle input
  bool handleClick(int x, int y);
  void updateHover(int x, int y);

  // Animation progress (0.0 = hidden, 1.0 = fully visible)
  float animationProgress() const { return animProgress_; }
  void tick(); // Update animation

private:
  OSFCompositor *compositor_;
  bool visible_ = false;
  float animProgress_ = 0.0f;

  int hoveredViewIndex_ = -1;
  int hoveredWorkspaceIndex_ = -1;

  // Drawing
  void drawOverlay(uint32_t *buf, int sw, int sh);
  void drawWindowThumbnails(uint32_t *buf, int sw, int sh);
  void drawWorkspaceStrip(uint32_t *buf, int sw, int sh);
};

} // namespace opensef
