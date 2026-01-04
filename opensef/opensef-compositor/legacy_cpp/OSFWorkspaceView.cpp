/**
 * OSFWorkspaceView.cpp - Multitask/Workspace Switcher
 */

#include "OSFWorkspaceView.h"
#include "OSFCompositor.h"
#include "OSFDecorations.h"
#include "OSFView.h"
#include "OSFOutput.h"

#include <algorithm>
#include <cmath>
#include <vector>

extern "C" {
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_surface.h>
}

namespace opensef {

using namespace AresColors;

OSFWorkspaceView::OSFWorkspaceView(OSFCompositor *compositor)
    : compositor_(compositor) {}

void OSFWorkspaceView::show() {
  if (visible_) return;
  visible_ = true;
  createScene();
}

void OSFWorkspaceView::hide() {
  if (!visible_) return;
  visible_ = false;
  destroyScene();
}

void OSFWorkspaceView::tick() {
  // Animate visibility
  float target = visible_ ? 1.0f : 0.0f;
  animProgress_ += (target - animProgress_) * 0.15f; // Smooth animation

  if (animProgress_ < 0.01f)
    animProgress_ = 0.0f;
  if (animProgress_ > 0.99f)
    animProgress_ = 1.0f;
}

void OSFWorkspaceView::createScene() {
  if (sceneTree_) return;

  // Attach directly to the root scene tree for overlay
  sceneTree_ = wlr_scene_tree_create(&compositor_->scene()->tree);

  // Get output geometry to size the overlay
  wlr_box box;
  wlr_output_layout_get_box(compositor_->outputLayout(), nullptr, &box);

  // Create dark overlay (Dark blue-black semi-transparent)
  // Color 0x1A1A2E with alpha ~0.7 (180/255)
  float overlayColor[4] = {
    0.102f, 0.102f, 0.180f, 0.7f
  };
  overlay_ = wlr_scene_rect_create(sceneTree_, box.width, box.height, overlayColor);
  wlr_scene_node_set_position(&overlay_->node, box.x, box.y);

  // Create workspace strip
  // Top strip showing all workspaces
  int stripHeight = 60;
  int stripY = 20;

  // Slight lighter background for strip area: 0x40FFFFFF (white with 25% alpha)
  float stripColor[4] = {1.0f, 1.0f, 1.0f, 0.25f};
  wlr_scene_rect *stripRect = wlr_scene_rect_create(sceneTree_, box.width, stripHeight, stripColor);
  wlr_scene_node_set_position(&stripRect->node, box.x, box.y + stripY);

  // Draw workspace thumbnails (3 workspaces for now)
  int workspaceCount = 3;
  int thumbWidth = 120;
  int thumbHeight = 40;
  int spacing = 20;
  int totalWidth = workspaceCount * thumbWidth + (workspaceCount - 1) * spacing;
  int startX = box.x + (box.width - totalWidth) / 2;
  int startY = box.y + stripY + (stripHeight - thumbHeight) / 2;

  for (int i = 0; i < workspaceCount; i++) {
    int x = startX + i * (thumbWidth + spacing);

    // Workspace thumbnail background
    // (i == 0) ? (0xFF000000 | SpaceOrange) : 0xFF404040;
    // SpaceOrange = 0xE85D04 -> R=0xE8, G=0x5D, B=0x04
    float thumbColor[4];
    if (i == 0) {
        thumbColor[0] = 0xE8 / 255.0f;
        thumbColor[1] = 0x5D / 255.0f;
        thumbColor[2] = 0x04 / 255.0f;
        thumbColor[3] = 1.0f;
    } else {
        thumbColor[0] = 0x40 / 255.0f;
        thumbColor[1] = 0x40 / 255.0f;
        thumbColor[2] = 0x40 / 255.0f;
        thumbColor[3] = 1.0f;
    }

    wlr_scene_rect *wsRect = wlr_scene_rect_create(sceneTree_, thumbWidth, thumbHeight, thumbColor);
    wlr_scene_node_set_position(&wsRect->node, x, startY);
  }

  // Create window thumbnails
  updateThumbnails();
}

void OSFWorkspaceView::destroyScene() {
  if (sceneTree_) {
    wlr_scene_node_destroy(&sceneTree_->node);
    sceneTree_ = nullptr;
    overlay_ = nullptr;
    thumbnailNodes_.clear();
  }
}

void OSFWorkspaceView::updateThumbnails() {
  if (!sceneTree_) return;

  // Clear existing thumbnails
  for (auto *node : thumbnailNodes_) {
    wlr_scene_node_destroy(&node->node);
  }
  thumbnailNodes_.clear();

  const auto &views = compositor_->views();
  if (views.empty()) return;

  // Count mapped windows
  int count = 0;
  for (const auto &view : views) {
      if (view->toplevel()->base->mapped) count++;
  }

  if (count == 0) return;

  wlr_box box;
  wlr_output_layout_get_box(compositor_->outputLayout(), nullptr, &box);

  int cols = static_cast<int>(std::ceil(std::sqrt(count)));
  int rows = static_cast<int>(std::ceil(static_cast<float>(count) / cols));

  int padding = 50;
  int topOffset = 100; // Leave room for strip
  int availWidth = box.width - padding * 2;
  int availHeight = box.height - topOffset - padding;

  // Grid layout logic
  int thumbW = availWidth / cols - padding;
  int thumbH = availHeight / rows - padding;

  // Clamp thumbnail size
  if (thumbW > 600) thumbW = 600;
  if (thumbH > 400) thumbH = 400;
  if (thumbW < 100) thumbW = 100;
  if (thumbH < 100) thumbH = 100;

  int current = 0;
  for (const auto &view : views) {
    if (!view->toplevel()->base->mapped) continue;

    // Get the surface buffer
    // Accessing surface->current.buffer for rendering
    wlr_surface *surface = view->toplevel()->base->surface;
    if (!surface) continue;

    struct wlr_buffer *buffer = surface->current.buffer;
    if (!buffer) continue;

    int row = current / cols;
    int col = current % cols;

    int x = box.x + padding + col * (thumbW + padding);
    int y = box.y + topOffset + row * (thumbH + padding);

    // Create container for this thumbnail
    wlr_scene_tree *thumbTree = wlr_scene_tree_create(sceneTree_);
    thumbnailNodes_.push_back(thumbTree);

    wlr_scene_node_set_position(&thumbTree->node, x, y);

    // Calculate scale to fit thumbW x thumbH
    int bufferWidth = buffer->width;
    int bufferHeight = buffer->height;

    float scaleX = static_cast<float>(thumbW) / bufferWidth;
    float scaleY = static_cast<float>(thumbH) / bufferHeight;
    float scale = std::min(scaleX, scaleY);

    // Create buffer node
    wlr_scene_buffer *bufNode = wlr_scene_buffer_create(thumbTree, buffer);
    if (bufNode) {
       wlr_scene_buffer_set_dest_size(bufNode, bufferWidth * scale, bufferHeight * scale);

       // Center it
       int finalW = static_cast<int>(bufferWidth * scale);
       int finalH = static_cast<int>(bufferHeight * scale);
       int offX = (thumbW - finalW) / 2;
       int offY = (thumbH - finalH) / 2;
       wlr_scene_node_set_position(&bufNode->node, offX, offY);
    }

    current++;
  }
}

void OSFWorkspaceView::render(uint32_t *buffer, int screenW, int screenH) {
  // Legacy software render path - replaced by scene graph usage
  (void)buffer;
  (void)screenW;
  (void)screenH;
}

void OSFWorkspaceView::drawOverlay(uint32_t *buf, int sw, int sh) {
  (void)buf; (void)sw; (void)sh;
}

void OSFWorkspaceView::drawWorkspaceStrip(uint32_t *buf, int sw, int sh) {
  (void)buf; (void)sw; (void)sh;
}

void OSFWorkspaceView::drawWindowThumbnails(uint32_t *buf, int sw, int sh) {
  (void)buf; (void)sw; (void)sh;
}

bool OSFWorkspaceView::handleClick(int x, int y) {
  if (!visible_)
    return false;

  // Modal behavior: any click hides the view
  if (overlay_) {
     // TODO: Implement proper hit testing to select window
     hide();
     return true;
  }

  return false;
}

void OSFWorkspaceView::updateHover(int x, int y) {
  (void)x;
  (void)y;
  // TODO: Track which window/workspace is hovered
}

} // namespace opensef
