/**
 * OSFShutdownScreen.cpp - Shutdown/Restart Screen Implementation
 */

#include "OSFShutdownScreen.h"
#include "OSFAresTheme.h"
#include "OSFCompositor.h"
#include "OSFDesktopLayers.h"

#include <cstdlib>
#include <iostream>
#include <wayland-server-core.h>

namespace opensef {

OSFShutdownScreen::OSFShutdownScreen(OSFCompositor *compositor,
                                     OSFDesktopLayers *layers)
    : compositor_(compositor), layers_(layers) {
  std::cout << "[openSEF] ShutdownScreen initialized" << std::endl;
}

OSFShutdownScreen::~OSFShutdownScreen() {
  destroyNodes();
  if (timerSource_) {
    wl_event_source_remove(timerSource_);
    timerSource_ = nullptr;
  }
}

void OSFShutdownScreen::showGoodbye() {
  if (visible_)
    return;

  currentAction_ = PowerAction::Shutdown;
  visible_ = true;
  createNodes("goodbye");

  std::cout << "[openSEF] ShutdownScreen: goodbye" << std::endl;

  // Trigger shutdown after a brief delay (2 seconds)
  wl_event_loop *loop = wl_display_get_event_loop(compositor_->display());
  timerSource_ = wl_event_loop_add_timer(loop, handleTimer, this);
  wl_event_source_timer_update(timerSource_, 2000);
}

void OSFShutdownScreen::showRestart() {
  if (visible_)
    return;

  currentAction_ = PowerAction::Restart;
  visible_ = true;
  createNodes("restart");

  std::cout << "[openSEF] ShutdownScreen: restart" << std::endl;

  // Trigger restart after a brief delay (2 seconds)
  wl_event_loop *loop = wl_display_get_event_loop(compositor_->display());
  timerSource_ = wl_event_loop_add_timer(loop, handleTimer, this);
  wl_event_source_timer_update(timerSource_, 2000);
}

void OSFShutdownScreen::hide() {
  if (!visible_)
    return;

  if (timerSource_) {
    wl_event_source_remove(timerSource_);
    timerSource_ = nullptr;
  }

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
    std::cout << "[openSEF] Executing system shutdown..." << std::endl;
    std::system("systemctl poweroff");
    break;
  case PowerAction::Restart:
    std::cout << "[openSEF] Executing system restart..." << std::endl;
    std::system("systemctl reboot");
    break;
  default:
    break;
  }
}

int OSFShutdownScreen::handleTimer(void *data) {
  auto *screen = static_cast<OSFShutdownScreen *>(data);

  if (screen->timerSource_) {
    wl_event_source_remove(screen->timerSource_);
    screen->timerSource_ = nullptr;
  }

  screen->executeAction();
  return 0;
}

} // namespace opensef
