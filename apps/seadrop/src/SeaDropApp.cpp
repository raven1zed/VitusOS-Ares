/**
 * SeaDropApp.cpp - Main application (pure C++)
 */

#include "SeaDrop.h"
#include <iostream>

namespace vitusos::seadrop {

SeaDropApp *SeaDropApp::instance_ = nullptr;

SeaDropApp::SeaDropApp() { instance_ = this; }

SeaDropApp::~SeaDropApp() { instance_ = nullptr; }

SeaDropApp &SeaDropApp::instance() { return *instance_; }

int SeaDropApp::run(int argc, char **argv) {
  (void)argc;
  (void)argv;

  std::cout << "╔════════════════════════════════════════════╗" << std::endl;
  std::cout << "║     SeaDrop for VitusOS                    ║" << std::endl;
  std::cout << "║     Clipboard Sync - Ares Theme            ║" << std::endl;
  std::cout << "╚════════════════════════════════════════════╝" << std::endl;
  std::cout << std::endl;

  // Connect to Wayland via openSEF
  auto &backend = OSFBackend::shared();
  if (!backend.connect()) {
    std::cerr << "[SeaDrop] Failed to connect to Wayland" << std::endl;
    return 1;
  }

  if (!initBackend()) {
    std::cerr << "[SeaDrop] Warning: No libseadrop (UI-only mode)" << std::endl;
  }

  initUI();

  std::cout << "[SeaDrop] Ready - running in system tray" << std::endl;

  // Show tray, window starts hidden
  tray_->show();

  // Run event loop
  backend.run();

  // Cleanup
  backend.disconnect();
  return 0;
}

bool SeaDropApp::initBackend() {
  // TODO: Initialize libseadrop when available as submodule
  // For now, we run in UI-only mode
  std::cout << "[SeaDrop] Running in UI-only mode" << std::endl;
  std::cout << "[SeaDrop] To enable sync: add libseadrop submodule"
            << std::endl;
  return false;
}

void SeaDropApp::initUI() {
  window_ = std::make_unique<SeaDropWindow>();
  tray_ = std::make_unique<SeaDropTray>();

  tray_->onOpenWindow = [this]() { window_->show(); };

  tray_->onQuit = []() { OSFBackend::shared().stop(); };

  window_->setStatus("UI-only mode", 0);
}

void SeaDropApp::onClipboardChanged(const ClipboardEntry &entry) {
  std::cout << "[SeaDrop] Clipboard: " << entry.preview << std::endl;

  // Add to history (newest first)
  history_.insert(history_.begin(), entry);
  if (history_.size() > 100) {
    history_.resize(100);
  }

  window_->updateHistory(history_);
}

} // namespace vitusos::seadrop
