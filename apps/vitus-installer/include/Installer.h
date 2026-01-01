/**
 * VitusOS Installer - Modern Minimalist GUI
 *
 * macOS-style installation experience, pure C++
 */

#pragma once

#include <opensef/OSFWindowDecorations.h>
#include <opensef/OpenSEFBackend.h>
#include <opensef/OpenSEFBase.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace vitusos::installer {

using namespace opensef;

// ============================================================================
// Installation Configuration
// ============================================================================

struct InstallConfig {
  std::string language = "en_US";
  std::string region = "US";
  std::string targetDisk;
  std::string fullName;
  std::string username;
  std::string password;
  bool darkMode = false;
  std::string wallpaper = "default";
};

// ============================================================================
// InstallerWindow - Main window (pure C++)
// ============================================================================

class InstallerWindow {
public:
  InstallerWindow();
  ~InstallerWindow();

  void show();
  void hide();
  bool isVisible() const { return visible_; }

  void nextPage();
  void prevPage();

private:
  bool visible_ = false;
  std::shared_ptr<OSFWaylandSurface> surface_;
  int currentPage_ = 0;
  static constexpr int kTotalPages = 5;
  InstallConfig config_;

  void draw();
};

// ============================================================================
// InstallerApp - Main application
// ============================================================================

class InstallerApp {
public:
  InstallerApp();
  ~InstallerApp();

  int run(int argc, char **argv);
  static InstallerApp &instance();

private:
  static InstallerApp *instance_;
  std::unique_ptr<InstallerWindow> window_;
};

} // namespace vitusos::installer
