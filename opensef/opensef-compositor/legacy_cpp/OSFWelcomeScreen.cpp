/**
 * OSFWelcomeScreen.cpp - Onboarding Welcome Screen Implementation
 */

#include "OSFWelcomeScreen.h"
#include "OSFAresTheme.h"
#include "OSFCompositor.h"
#include "OSFDesktopLayers.h"


#include <iostream>

namespace opensef {

OSFWelcomeScreen::OSFWelcomeScreen(OSFCompositor *compositor,
                                   OSFDesktopLayers *layers)
    : compositor_(compositor), layers_(layers) {
  std::cout << "[openSEF] WelcomeScreen initialized" << std::endl;
}

OSFWelcomeScreen::~OSFWelcomeScreen() { destroyNodes(); }

void OSFWelcomeScreen::show() {
  if (visible_)
    return;

  visible_ = true;
  createNodes();

  std::cout << "[openSEF] WelcomeScreen: Hello, Welcome" << std::endl;
}

void OSFWelcomeScreen::hide() {
  if (!visible_)
    return;

  visible_ = false;
  destroyNodes();

  std::cout << "[openSEF] WelcomeScreen hidden" << std::endl;
}

void OSFWelcomeScreen::createNodes() {
  // Get screen dimensions - placeholder for now
  screenWidth_ = 1920;
  screenHeight_ = 1080;

  // Window dimensions (centered, ~80% of screen)
  int windowW = static_cast<int>(screenWidth_ * 0.6);
  int windowH = static_cast<int>(screenHeight_ * 0.7);
  int windowX = (screenWidth_ - windowW) / 2;
  int windowY = (screenHeight_ - windowH) / 2;

  // === Create frosted glass window background ===
  float windowColor[4];
  AresTheme::hexToRGBA(AresTheme::WindowBg, windowColor, 0.9f); // 90% opacity

  windowBg_ =
      layers_->createRect(DesktopLayer::Windows, windowW, windowH, windowColor);
  if (windowBg_) {
    wlr_scene_node_set_position(&windowBg_->node, windowX, windowY);
  }

  // === Traffic light buttons (top-left of window) ===
  int buttonY = windowY + AresDimensions::TrafficButtonMargin;
  int buttonX = windowX + AresDimensions::TrafficButtonMargin;
  int buttonSize = AresDimensions::TrafficButtonSize;
  int buttonSpacing = AresDimensions::TrafficButtonSpacing;

  // Close button (red)
  float closeColor[4];
  AresTheme::hexToRGBA(AresTheme::TrafficRed, closeColor);
  closeButton_ = layers_->createRect(DesktopLayer::Windows, buttonSize,
                                     buttonSize, closeColor);
  if (closeButton_) {
    wlr_scene_node_set_position(&closeButton_->node, buttonX, buttonY);
    closeButtonBounds_ = {buttonX, buttonY, buttonSize, buttonSize};
  }
  buttonX += buttonSize + buttonSpacing;

  // Minimize button (yellow)
  float minimizeColor[4];
  AresTheme::hexToRGBA(AresTheme::TrafficYellow, minimizeColor);
  minimizeButton_ = layers_->createRect(DesktopLayer::Windows, buttonSize,
                                        buttonSize, minimizeColor);
  if (minimizeButton_) {
    wlr_scene_node_set_position(&minimizeButton_->node, buttonX, buttonY);
  }
  buttonX += buttonSize + buttonSpacing;

  // Fullscreen button (blue/green)
  float fullscreenColor[4];
  AresTheme::hexToRGBA(AresTheme::TrafficBlue, fullscreenColor);
  fullscreenButton_ = layers_->createRect(DesktopLayer::Windows, buttonSize,
                                          buttonSize, fullscreenColor);
  if (fullscreenButton_) {
    wlr_scene_node_set_position(&fullscreenButton_->node, buttonX, buttonY);
  }

  // === "Let's Start" button (bottom center of window) ===
  int startBtnW = 160;
  int startBtnH = 44;
  int startBtnX = windowX + (windowW - startBtnW) / 2;
  int startBtnY = windowY + windowH - startBtnH - 60;

  float startColor[4];
  AresTheme::hexToRGBA(AresTheme::SpaceOrange, startColor);
  startButton_ = layers_->createRect(DesktopLayer::Windows, startBtnW,
                                     startBtnH, startColor);
  if (startButton_) {
    wlr_scene_node_set_position(&startButton_->node, startBtnX, startBtnY);
    startButtonBounds_ = {startBtnX, startBtnY, startBtnW, startBtnH};
  }

  // Note: Text rendering for "vitusOS | ARES", "Hello,Welcome", and "Let's
  // Start" requires font infrastructure - placeholder for now
}

void OSFWelcomeScreen::destroyNodes() {
  if (windowBg_) {
    wlr_scene_node_destroy(&windowBg_->node);
    windowBg_ = nullptr;
  }
  if (closeButton_) {
    wlr_scene_node_destroy(&closeButton_->node);
    closeButton_ = nullptr;
  }
  if (minimizeButton_) {
    wlr_scene_node_destroy(&minimizeButton_->node);
    minimizeButton_ = nullptr;
  }
  if (fullscreenButton_) {
    wlr_scene_node_destroy(&fullscreenButton_->node);
    fullscreenButton_ = nullptr;
  }
  if (startButton_) {
    wlr_scene_node_destroy(&startButton_->node);
    startButton_ = nullptr;
  }
}

bool OSFWelcomeScreen::handleClick(int x, int y) {
  if (!visible_)
    return false;

  // Check "Let's Start" button
  if (x >= startButtonBounds_.x &&
      x < startButtonBounds_.x + startButtonBounds_.w &&
      y >= startButtonBounds_.y &&
      y < startButtonBounds_.y + startButtonBounds_.h) {
    std::cout << "[openSEF] WelcomeScreen: Let's Start clicked" << std::endl;
    if (onStartClicked) {
      onStartClicked();
    }
    hide();
    return true;
  }

  // Check close button
  if (x >= closeButtonBounds_.x &&
      x < closeButtonBounds_.x + closeButtonBounds_.w &&
      y >= closeButtonBounds_.y &&
      y < closeButtonBounds_.y + closeButtonBounds_.h) {
    std::cout << "[openSEF] WelcomeScreen: Close clicked" << std::endl;
    hide();
    return true;
  }

  return false;
}

} // namespace opensef
