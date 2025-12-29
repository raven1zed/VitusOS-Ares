/**
 * InstallerApp.cpp - VitusOS Installer (pure C++)
 */

#include "Installer.h"
#include <iostream>

namespace vitusos::installer {

InstallerApp *InstallerApp::instance_ = nullptr;

InstallerApp::InstallerApp() { instance_ = this; }

InstallerApp::~InstallerApp() { instance_ = nullptr; }

InstallerApp &InstallerApp::instance() { return *instance_; }

int InstallerApp::run(int argc, char **argv) {
  (void)argc;
  (void)argv;

  std::cout << "╔════════════════════════════════════════════╗" << std::endl;
  std::cout << "║     VitusOS Installer                      ║" << std::endl;
  std::cout << "║     macOS-Style Installation Wizard        ║" << std::endl;
  std::cout << "╚════════════════════════════════════════════╝" << std::endl;
  std::cout << std::endl;

  // Connect to Wayland
  auto &backend = OSFBackend::shared();
  if (!backend.connect()) {
    std::cerr << "[Installer] Failed to connect to display" << std::endl;
    return 1;
  }

  // Create and show window
  window_ = std::make_unique<InstallerWindow>();
  window_->show();

  std::cout << "[Installer] Ready" << std::endl;

  // Run event loop
  backend.run();

  backend.disconnect();
  return 0;
}

// ============================================================================
// InstallerWindow
// ============================================================================

InstallerWindow::InstallerWindow() = default;

InstallerWindow::~InstallerWindow() { hide(); }

void InstallerWindow::show() {
  if (visible_)
    return;

  surface_ = OSFWaylandSurface::create(800, 600, "VitusOS Installer");
  if (!surface_) {
    std::cerr << "[Installer] Failed to create window" << std::endl;
    return;
  }

  visible_ = true;
  draw();

  std::cout << "[Installer] Window shown (page 1/" << kTotalPages << ")"
            << std::endl;
}

void InstallerWindow::hide() {
  if (!visible_)
    return;
  surface_.reset();
  visible_ = false;
}

void InstallerWindow::nextPage() {
  if (currentPage_ < kTotalPages - 1) {
    currentPage_++;
    draw();
    std::cout << "[Installer] Page " << (currentPage_ + 1) << "/" << kTotalPages
              << std::endl;
  }
}

void InstallerWindow::prevPage() {
  if (currentPage_ > 0) {
    currentPage_--;
    draw();
    std::cout << "[Installer] Page " << (currentPage_ + 1) << "/" << kTotalPages
              << std::endl;
  }
}

void InstallerWindow::draw() {
  if (!surface_)
    return;

  // Warm surface background (light mode)
  OSFColor bg = OSFColors::surface();
  surface_->draw(bg);
}

} // namespace vitusos::installer
