/**
 * OSFShutdownScreen.cpp - Shutdown/Restart Screen Implementation
 */

#include "OSFShutdownScreen.h"
#include "OSFAresTheme.h"
#include "OSFCompositor.h"
#include "OSFDesktopLayers.h"


#include <cstdlib>
#include <iostream>

namespace opensef {

OSFShutdownScreen::OSFShutdownScreen(OSFCompositor *compositor,
                                     OSFDesktopLayers *layers)
    : compositor_(compositor), layers_(layers) {
  std::cout << "[openSEF] ShutdownScreen initialized" << std::endl;
}

OSFShutdownScreen::~OSFShutdownScreen() { destroyNodes(); }

void OSFShutdownScreen::showGoodbye() {
  if (visible_)
    return;

  currentAction_ = PowerAction::Shutdown;
  visible_ = true;
  createNodes("goodbye");

  std::cout << "[openSEF] ShutdownScreen: goodbye" << std::endl;

  // In real implementation, trigger shutdown after a brief delay
  // For now, just show the screen
}

void OSFShutdownScreen::showRestart() {
  if (visible_)
    return;

  currentAction_ = PowerAction::Restart;
  visible_ = true;
  createNodes("restart");

  std::cout << "[openSEF] ShutdownScreen: restart" << std::endl;

  // In real implementation, trigger restart after a brief delay
}

void OSFShutdownScreen::hide() {
  if (!visible_)
    return;

  visible_ = false;
  currentAction_ = PowerAction::None;
  destroyNodes();

  std::cout << "[openSEF] ShutdownScreen hidden" << std::endl;
}

void OSFShutdownScreen::createNodes(const char *text) {
  // Create full-screen black background in the Lock layer (above everything)
  float bgColor[4];
  AresTheme::hexToRGBA(AresTheme::PureBlack, bgColor);

  // Get screen dimensions from compositor
  // For now, use placeholder dimensions - should get from output
  int width = 1920; // TODO: Get from actual output
  int height = 1080;

  background_ = layers_->createRect(DesktopLayer::Lock, width, height, bgColor);
  if (background_) {
    wlr_scene_node_set_position(&background_->node, 0, 0);
  }

  // Note: Text rendering ("goodbye" / "restart") requires font infrastructure
  // This would be rendered at screen center in white, lowercase
  // Text: AresTheme::PureWhite (0xFFFFFF)

  (void)text; // Will be used when text rendering is implemented
}

void OSFShutdownScreen::destroyNodes() {
  if (background_) {
    wlr_scene_node_destroy(&background_->node);
    background_ = nullptr;
  }
}

void OSFShutdownScreen::executeAction() {
  switch (currentAction_) {
  case PowerAction::Shutdown:
    // TODO: Execute system shutdown
    // system("poweroff");
    break;
  case PowerAction::Restart:
    system("reboot");
    break;
  default:
    break;
  }
}

} // namespace opensef
