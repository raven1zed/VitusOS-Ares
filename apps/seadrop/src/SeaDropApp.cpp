/**
 * SeaDropApp.cpp - Main application (openSEF + libseadrop)
 */

#include "SeaDrop.h"
#include <iostream>

namespace vitusos::seadrop {

static SeaDropApp *g_instance = nullptr;

SeaDropApp::SeaDropApp() { g_instance = this; }

SeaDropApp::~SeaDropApp() { g_instance = nullptr; }

SeaDropApp &SeaDropApp::instance() { return *g_instance; }

int SeaDropApp::run(int argc, char **argv) {
  std::cout << "[SeaDrop] Starting SeaDrop for VitusOS..." << std::endl;
  std::cout << "[SeaDrop] openSEF " << OPENSEF_VERSION_STRING << std::endl;

#if HAVE_LIBSEADROP
  std::cout << "[SeaDrop] libseadrop: Available (from SeagrEnv)" << std::endl;
#else
  std::cout << "[SeaDrop] libseadrop: Not found (UI-only mode)" << std::endl;
  std::cout
      << "[SeaDrop] To enable sync: git submodule update --init --recursive"
      << std::endl;
#endif

  // Connect to Wayland via openSEF
  OSFBackend *osfBackend = [OSFBackend sharedBackend];
  if (![osfBackend connect]) {
    std::cerr << "[SeaDrop] Failed to connect to Wayland" << std::endl;
    return 1;
  }

  initBackend();
  initUI();

  std::cout << "[SeaDrop] Ready" << std::endl;

  // Show tray, hide window initially
  tray_->show();

  // Run event loop
  [osfBackend run];

  [osfBackend disconnect];
  return 0;
}

void SeaDropApp::initBackend() {
#if HAVE_LIBSEADROP
  backend_ = std::make_unique<::seadrop::SeaDrop>();

  // Configure callbacks
  backend_->onClipboardChanged([this](const ::seadrop::ClipboardEntry &entry) {
    ClipboardEntry uiEntry;
    uiEntry.id = entry.id;
    uiEntry.preview = entry.content.substr(0, 100);
    uiEntry.type = entry.type;
    uiEntry.sourceDevice = entry.sourceDevice;
    uiEntry.timestamp = entry.timestamp;
    uiEntry.synced = entry.synced;
    onClipboardChanged(uiEntry);
  });

  backend_->initialize("VitusOS-Desktop");
  backend_->startDiscovery();
#else
  std::cout << "[SeaDrop] Running in UI-only mode (no sync)" << std::endl;
#endif
}

void SeaDropApp::initUI() {
  window_ = std::make_unique<SeaDropWindow>();
  tray_ = std::make_unique<SeaDropTray>();

  tray_->onOpenWindow = [this]() { window_->show(); };

  tray_->onQuit = [this]() { [[OSFBackend sharedBackend] disconnect]; };

  // Set initial status
#if HAVE_LIBSEADROP
  window_->setStatus("Discovering devices...", 0);
#else
  window_->setStatus("UI-only mode", 0);
#endif
}

void SeaDropApp::onClipboardChanged(const ClipboardEntry &entry) {
  std::cout << "[SeaDrop] Clipboard: " << entry.preview << std::endl;

  // Add to history
  history_.insert(history_.begin(), entry);
  if (history_.size() > 100) {
    history_.resize(100);
  }

  // Update UI
  window_->updateHistory(history_);

#if HAVE_LIBSEADROP
  // Sync to other devices
  // backend_->syncClipboard(...);
#endif
}

} // namespace vitusos::seadrop
