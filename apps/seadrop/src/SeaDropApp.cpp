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
  std::cout << "[SeaDrop] Starting SeaDrop v1.0.0 for VitusOS..." << std::endl;
  std::cout << "[SeaDrop] openSEF " << OPENSEF_VERSION_STRING << " + libseadrop"
            << std::endl;

  initBackend();
  initUI();

  // Connect to Wayland via openSEF
  if (!OSFBackend::sharedBackend()->connect()) {
    std::cerr << "[SeaDrop] Failed to connect to Wayland" << std::endl;
    return 1;
  }

  // Start discovery
  backend_->startDiscovery();

  std::cout << "[SeaDrop] Ready. Monitoring clipboard..." << std::endl;

  // Show tray icon, hide window initially
  tray_->show();

  // Run openSEF event loop
  OSFBackend::sharedBackend()->run();

  return 0;
}

void SeaDropApp::initBackend() {
  // Initialize libseadrop
  backend_ = std::make_unique<seadrop::SeaDrop>();

  // Configure callbacks
  backend_->onClipboardChanged([this](const seadrop::ClipboardEntry &entry) {
    onClipboardChanged(entry);
  });

  backend_->onDeviceConnected(
      [this](const seadrop::Device &device) { onDeviceConnected(device); });

  backend_->onDeviceDisconnected(
      [this](const seadrop::Device &device) { onDeviceDisconnected(device); });

  // Initialize with device name
  backend_->initialize("VitusOS-Desktop");
}

void SeaDropApp::initUI() {
  window_ = std::make_unique<SeaDropWindow>();
  tray_ = std::make_unique<SeaDropTray>();

  tray_->onOpenWindow = [this]() { window_->show(); };

  tray_->onQuit = [this]() {
    OSFBackend::sharedBackend()->disconnect();
    // Exit app
  };
}

void SeaDropApp::onClipboardChanged(const seadrop::ClipboardEntry &entry) {
  std::cout << "[SeaDrop] Clipboard: " << entry.preview.substr(0, 50) << "..."
            << std::endl;

  // Add to history
  history_.insert(history_.begin(), entry);
  if (history_.size() > 100) {
    history_.resize(100);
  }

  // Update UI
  window_->updateHistory(history_);

  // Sync to connected devices
  backend_->syncClipboard(entry);
}

void SeaDropApp::onDeviceConnected(const seadrop::Device &device) {
  std::cout << "[SeaDrop] Device connected: " << device.name << std::endl;

  tray_->setConnected(true);
  tray_->showNotification("SeaDrop", device.name + " connected");

  auto devices = backend_->connectedDevices();
  window_->setStatus("Connected", static_cast<int>(devices.size()));
}

void SeaDropApp::onDeviceDisconnected(const seadrop::Device &device) {
  std::cout << "[SeaDrop] Device disconnected: " << device.name << std::endl;

  auto devices = backend_->connectedDevices();
  if (devices.empty()) {
    tray_->setConnected(false);
    window_->setStatus("No devices", 0);
  } else {
    window_->setStatus("Connected", static_cast<int>(devices.size()));
  }
}

} // namespace vitusos::seadrop
