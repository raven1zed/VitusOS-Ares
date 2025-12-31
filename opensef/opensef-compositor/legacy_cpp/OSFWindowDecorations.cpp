/**
 * OSFWindowDecorations.cpp - Traffic Light Window Decorations Implementation
 */

#include "OSFWindowDecorations.h"
#include "OSFAresTheme.h"
#include "OSFCompositor.h"

#include <iostream>

namespace opensef {

OSFWindowDecorations::OSFWindowDecorations(OSFCompositor *compositor,
                                           wlr_scene_tree *parent)
    : compositor_(compositor) {
  if (parent) {
    tree_ = wlr_scene_tree_create(parent);
    createNodes();
  }
}

OSFWindowDecorations::~OSFWindowDecorations() {
  if (tree_) {
    wlr_scene_node_destroy(&tree_->node);
    tree_ = nullptr;
  }
}

void OSFWindowDecorations::createNodes() {
  if (!tree_)
    return;

  // === Title bar background (frosted glass effect) ===
  float titleBgColor[4];
  AresTheme::hexToRGBA(AresTheme::WindowBg, titleBgColor, 0.95f);

  titleBarBg_ =
      wlr_scene_rect_create(tree_, width_, TitleBarHeight, titleBgColor);
  if (titleBarBg_) {
    wlr_scene_node_set_position(&titleBarBg_->node, 0, 0);
  }

  // === Traffic light buttons ===
  int buttonY = ButtonMarginTop;
  int buttonX = ButtonMarginLeft;

  // Close button (red)
  float closeColor[4];
  AresTheme::hexToRGBA(AresTheme::TrafficRed, closeColor);
  closeButton_ =
      wlr_scene_rect_create(tree_, ButtonSize, ButtonSize, closeColor);
  if (closeButton_) {
    wlr_scene_node_set_position(&closeButton_->node, buttonX, buttonY);
    closeBounds_ = {buttonX, buttonY, ButtonSize};
  }
  buttonX += ButtonSize + ButtonSpacing;

  // Minimize button (yellow)
  float minimizeColor[4];
  AresTheme::hexToRGBA(AresTheme::TrafficYellow, minimizeColor);
  minimizeButton_ =
      wlr_scene_rect_create(tree_, ButtonSize, ButtonSize, minimizeColor);
  if (minimizeButton_) {
    wlr_scene_node_set_position(&minimizeButton_->node, buttonX, buttonY);
    minimizeBounds_ = {buttonX, buttonY, ButtonSize};
  }
  buttonX += ButtonSize + ButtonSpacing;

  // Fullscreen button (green/blue)
  float fullscreenColor[4];
  AresTheme::hexToRGBA(AresTheme::TrafficBlue, fullscreenColor);
  fullscreenButton_ =
      wlr_scene_rect_create(tree_, ButtonSize, ButtonSize, fullscreenColor);
  if (fullscreenButton_) {
    wlr_scene_node_set_position(&fullscreenButton_->node, buttonX, buttonY);
    fullscreenBounds_ = {buttonX, buttonY, ButtonSize};
  }
}

void OSFWindowDecorations::setWidth(int width) {
  width_ = width;
  if (titleBarBg_) {
    wlr_scene_rect_set_size(titleBarBg_, width, TitleBarHeight);
  }
}

void OSFWindowDecorations::setActive(bool active) {
  active_ = active;
  updateColors();
}

void OSFWindowDecorations::updateColors() {
  if (!active_) {
    // Inactive: gray out the buttons
    float grayColor[4] = {0.6f, 0.6f, 0.6f, 1.0f};
    if (closeButton_)
      wlr_scene_rect_set_color(closeButton_, grayColor);
    if (minimizeButton_)
      wlr_scene_rect_set_color(minimizeButton_, grayColor);
    if (fullscreenButton_)
      wlr_scene_rect_set_color(fullscreenButton_, grayColor);
  } else {
    // Active: restore original colors
    float closeColor[4], minimizeColor[4], fullscreenColor[4];
    AresTheme::hexToRGBA(AresTheme::TrafficRed, closeColor);
    AresTheme::hexToRGBA(AresTheme::TrafficYellow, minimizeColor);
    AresTheme::hexToRGBA(AresTheme::TrafficBlue, fullscreenColor);

    if (closeButton_)
      wlr_scene_rect_set_color(closeButton_, closeColor);
    if (minimizeButton_)
      wlr_scene_rect_set_color(minimizeButton_, minimizeColor);
    if (fullscreenButton_)
      wlr_scene_rect_set_color(fullscreenButton_, fullscreenColor);
  }
}

bool OSFWindowDecorations::hitTestTitleBar(int x, int y) const {
  return x >= 0 && x < width_ && y >= 0 && y < TitleBarHeight;
}

WindowButton OSFWindowDecorations::hitTestButton(int x, int y) const {
  // Check close button
  if (x >= closeBounds_.x && x < closeBounds_.x + closeBounds_.size &&
      y >= closeBounds_.y && y < closeBounds_.y + closeBounds_.size) {
    return WindowButton::Close;
  }

  // Check minimize button
  if (x >= minimizeBounds_.x && x < minimizeBounds_.x + minimizeBounds_.size &&
      y >= minimizeBounds_.y && y < minimizeBounds_.y + minimizeBounds_.size) {
    return WindowButton::Minimize;
  }

  // Check fullscreen button
  if (x >= fullscreenBounds_.x &&
      x < fullscreenBounds_.x + fullscreenBounds_.size &&
      y >= fullscreenBounds_.y &&
      y < fullscreenBounds_.y + fullscreenBounds_.size) {
    return WindowButton::Fullscreen;
  }

  // No button hit - use Close as default (will be filtered by caller)
  return WindowButton::Close;
}

} // namespace opensef
