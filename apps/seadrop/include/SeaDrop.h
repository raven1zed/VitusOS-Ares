/**
 * SeaDrop for VitusOS - openSEF Native Frontend
 *
 * Uses libseadrop backend from SeagrEnv (submodule)
 * Built with openSEF framework for native VitusOS integration.
 *
 * Architecture:
 * ┌─────────────────────────────────────────────┐
 * │  openSEF Frontend (VitusOS native)          │
 * │  └── SeaDropWindow, SeaDropTray             │
 * ├─────────────────────────────────────────────┤
 * │  libseadrop (C++17 core - from SeagrEnv)    │
 * │  └── Discovery, Connection, Transfer,       │
 * │      Clipboard, Distance, Security          │
 * └─────────────────────────────────────────────┘
 */

#pragma once

#include <opensef/OpenSEFAppKit.h>
#include <opensef/OpenSEFBackend.h>
#include <opensef/OpenSEFBase.h>
#include <opensef/OpenSEFUI.h>


#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace vitusos::seadrop {

using namespace opensef;

// ============================================================================
// ClipboardEntry - For UI display
// ============================================================================

struct ClipboardEntry {
  std::string id;
  std::string preview; // First ~100 chars
  std::string type;    // "text", "image", "file"
  std::string sourceDevice;
  uint64_t timestamp = 0;
  bool synced = false;
};

// ============================================================================
// SeaDropWindow - Main window (pure C++)
// ============================================================================

class SeaDropWindow {
public:
  SeaDropWindow();
  ~SeaDropWindow();

  void show();
  void hide();
  bool isVisible() const { return visible_; }

  void updateHistory(const std::vector<ClipboardEntry> &entries);
  void setStatus(const std::string &status, int deviceCount);

private:
  bool visible_ = false;
  std::shared_ptr<OSFWaylandSurface> surface_;
  std::vector<ClipboardEntry> history_;
  std::string statusText_;
  int deviceCount_ = 0;

  void draw();
};

// ============================================================================
// SeaDropTray - System tray (stub for now)
// ============================================================================

class SeaDropTray {
public:
  SeaDropTray();
  ~SeaDropTray();

  void show();
  void hide();
  void setConnected(bool connected);

  std::function<void()> onOpenWindow;
  std::function<void()> onQuit;

private:
  bool connected_ = false;
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

  SeaDropWindow &window() { return *window_; }

private:
  static SeaDropApp *instance_;

  std::unique_ptr<SeaDropWindow> window_;
  std::unique_ptr<SeaDropTray> tray_;
  std::vector<ClipboardEntry> history_;

  bool initBackend();
  void initUI();
  void onClipboardChanged(const ClipboardEntry &entry);
};

} // namespace vitusos::seadrop
