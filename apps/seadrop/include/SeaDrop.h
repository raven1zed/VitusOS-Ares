/**
 * SeaDrop for VitusOS - openSEF Native Frontend
 *
 * Uses libseadrop backend from github.com/raven1zed/SeagrEnv
 * Frontend built with openSEF instead of Qt 6 for native VitusOS integration.
 *
 * Architecture:
 * ┌─────────────────────────────────────────────┐
 * │  openSEF Frontend (VitusOS native)         │
 * │  └── SeaDropWindow, SeaDropTray            │
 * ├─────────────────────────────────────────────┤
 * │  libseadrop (C++17 core)                   │
 * │  └── Discovery, Connection, Transfer,      │
 * │      Clipboard, Distance, Security         │
 * └─────────────────────────────────────────────┘
 */

#pragma once

// libseadrop headers (from SeagrEnv)
#include <seadrop/clipboard.h>
#include <seadrop/connection.h>
#include <seadrop/device.h>
#include <seadrop/discovery.h>
#include <seadrop/distance.h>
#include <seadrop/seadrop.h>
#include <seadrop/security.h>
#include <seadrop/transfer.h>


// openSEF framework
#include <opensef/opensef.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>


namespace vitusos::seadrop {

// ============================================================================
// SeaDropWindow - Main window (openSEF native)
// ============================================================================

class SeaDropWindow {
public:
  SeaDropWindow();
  ~SeaDropWindow();

  void show();
  void hide();
  bool isVisible() const;

  // Update UI with clipboard history
  void updateHistory(const std::vector<seadrop::ClipboardEntry> &entries);

  // Update connection status
  void setStatus(const std::string &status, int deviceCount);

  // Filter history by search query
  void setSearchQuery(const std::string &query);

private:
  // openSEF widgets (Objective-C under the hood)
  OSFWindow *window_;
  OSFTableView *historyTableView_;
  OSFTextField *searchField_;
  OSFLabel *statusLabel_;
  OSFButton *syncButton_;
  OSFGlassPanel *backgroundPanel_;

  void setupUI();
  void setupMenu();
  void onHistoryItemSelected(int index);
  void onSearchChanged(const std::string &query);
  void onSyncClicked();
};

// ============================================================================
// SeaDropTray - System tray integration
// ============================================================================

class SeaDropTray {
public:
  SeaDropTray();
  ~SeaDropTray();

  void show();
  void hide();

  void setConnected(bool connected);
  void showNotification(const std::string &title, const std::string &message);

  std::function<void()> onOpenWindow;
  std::function<void()> onQuit;

private:
  void *trayIcon_; // Platform-specific
};

// ============================================================================
// SeaDropApp - Main application
// ============================================================================

class SeaDropApp {
public:
  SeaDropApp();
  ~SeaDropApp();

  int run(int argc, char **argv);

  static SeaDropApp &instance();

  // Access libseadrop components
  seadrop::SeaDrop &backend() { return *backend_; }

private:
  std::unique_ptr<seadrop::SeaDrop> backend_; // libseadrop core
  std::unique_ptr<SeaDropWindow> window_;
  std::unique_ptr<SeaDropTray> tray_;

  std::vector<seadrop::ClipboardEntry> history_;

  void initBackend();
  void initUI();
  void onClipboardChanged(const seadrop::ClipboardEntry &entry);
  void onDeviceConnected(const seadrop::Device &device);
  void onDeviceDisconnected(const seadrop::Device &device);
};

} // namespace vitusos::seadrop
