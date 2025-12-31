/**
 * OSFDesktopLayers.cpp - Desktop Scene Layer Implementation
 *
 * Creates and manages wlr_scene_tree layers for proper z-ordering.
 */

#include "OSFDesktopLayers.h"
#include "OSFCompositor.h"

#include <iostream>

namespace opensef {

OSFDesktopLayers::OSFDesktopLayers(OSFCompositor *compositor, wlr_scene *scene)
    : compositor_(compositor), scene_(scene) {

  // Create layer trees in order (bottom to top)
  for (int i = 0; i < static_cast<int>(DesktopLayer::Count); i++) {
    layers_[i] = wlr_scene_tree_create(&scene->tree);
  }

  std::cout << "[openSEF] Created " << static_cast<int>(DesktopLayer::Count)
            << " desktop layers" << std::endl;
}

wlr_scene_tree *OSFDesktopLayers::layer(DesktopLayer layer) {
  int idx = static_cast<int>(layer);
  if (idx >= 0 && idx < static_cast<int>(DesktopLayer::Count)) {
    return layers_[idx];
  }
  return nullptr;
}

wlr_scene_rect *OSFDesktopLayers::createRect(DesktopLayer layer, int width,
                                             int height, float color[4]) {
  wlr_scene_tree *tree = this->layer(layer);
  if (!tree)
    return nullptr;

  return wlr_scene_rect_create(tree, width, height, color);
}

wlr_scene_buffer *OSFDesktopLayers::createBuffer(DesktopLayer layer) {
  wlr_scene_tree *tree = this->layer(layer);
  if (!tree)
    return nullptr;

  // Create an empty buffer node - caller will set the buffer
  return wlr_scene_buffer_create(tree, nullptr);
}

} // namespace opensef
