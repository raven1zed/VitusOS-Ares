/**
 * SeaDropWindow.cpp - Main window (pure C++)
 */

#include "SeaDrop.h"
#include <iostream>

namespace vitusos::seadrop {

SeaDropWindow::SeaDropWindow() {
  // Window created on-demand when show() is called
}

SeaDropWindow::~SeaDropWindow() { hide(); }

void SeaDropWindow::show() {
  if (visible_)
    return;

  // Create Wayland surface
  surface_ = OSFWaylandSurface::create(500, 400, "SeaDrop");
  if (!surface_) {
    std::cerr << "[SeaDrop] Failed to create window surface" << std::endl;
    return;
  }

  visible_ = true;
  draw();

  std::cout << "[SeaDrop] Window shown" << std::endl;
}

void SeaDropWindow::hide() {
  if (!visible_)
    return;

  surface_.reset();
  visible_ = false;

  std::cout << "[SeaDrop] Window hidden" << std::endl;
}

void SeaDropWindow::updateHistory(const std::vector<ClipboardEntry> &entries) {
  history_ = entries;
  if (visible_) {
    draw();
  }
}

void SeaDropWindow::setStatus(const std::string &status, int deviceCount) {
  statusText_ = status;
  deviceCount_ = deviceCount;
  if (visible_) {
    draw();
  }
}

void SeaDropWindow::draw() {
  if (!surface_)
    return;

  // For now, draw background with Ares colors
  // TODO: Add text rendering, list view, etc.

  // Dark surface background
  OSFColor bgColor = OSFColors::backgroundDark();
  surface_->draw(bgColor);
}

} // namespace vitusos::seadrop
