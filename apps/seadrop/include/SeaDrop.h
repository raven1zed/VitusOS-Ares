/**
 * SeaDrop for VitusOS - openSEF Native Frontend
 *
 * Uses libseadrop backend from github.com/raven1zed/SeagrEnv (submodule)
 * Built with openSEF framework for native VitusOS integration.
 *
 * Architecture:
 * ┌─────────────────────────────────────────────┐
 * │  openSEF Frontend (VitusOS native)         │
 * │  └── SeaDropWindow, SeaDropTray            │
 * ├─────────────────────────────────────────────┤
 * │  libseadrop (C++17 core - from SeagrEnv)   │
 * │  └── Discovery, Connection, Transfer,      │
 * │      Clipboard, Distance, Security         │
 * └─────────────────────────────────────────────┘
 *
 * SeaDrop is:
 * - Open source (GPL-3.0)
 * - Cross-platform core (libseadrop works on Linux + Android)
 * - VitusOS native frontend (openSEF)
 * - Seamless Android <-> VitusOS sync
 */

#pragma once

// openSEF framework
#include <opensef/opensef.h>

// libseadrop (if available via submodule)
#if HAVE_LIBSEADROP
#include <seadrop/clipboard.h>
#include <seadrop/connection.h>
#include <seadrop/device.h>
#include <seadrop/discovery.h>
#include <seadrop/distance.h>
#include <seadrop/seadrop.h>
#include <seadrop/security.h>
#include <seadrop/transfer.h>

#endif

#include <functional>
#include <memory>
#include <string>
#include <vector>


namespace vitusos::seadrop {

// ============================================================================
// ClipboardEntry - For UI display (works with or without libseadrop)
// ============================================================================

struct ClipboardEntry {
  std::string id;
  std::string preview; // First 100 chars or filename
  std::string type;    // "text", "image", "file"
  std::string sourceDevice;
  uint64_t timestamp;
  bool synced;
};

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

  void updateHistory(const std::vector<ClipboardEntry> &entries);
  void setStatus(const std::string &status, int deviceCount);
  void setSearchQuery(const std::string &query);

private:
  OSFWindow *window_;
  OSFGlassPanel *backgroundPanel_;
  OSFTableView *historyTableView_;
  OSFTextField *searchField_;
  OSFLabel *statusLabel_;
  OSFButton *syncButton_;

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

#if HAVE_LIBSEADROP
  // Access libseadrop when available
  ::seadrop::SeaDrop &backend() { return *backend_; }
#endif

private:
#if HAVE_LIBSEADROP
  std::unique_ptr<::seadrop::SeaDrop> backend_;
#endif

  std::unique_ptr<SeaDropWindow> window_;
  std::unique_ptr<SeaDropTray> tray_;
  std::vector<ClipboardEntry> history_;

  void initBackend();
  void initUI();
  void onClipboardChanged(const ClipboardEntry &entry);
};

} // namespace vitusos::seadrop
