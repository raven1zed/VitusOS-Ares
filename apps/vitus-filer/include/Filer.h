/**
 * VitusOS Filer - openSEF Native File Manager
 *
 * Design: OS1 (Her 2013) + macOS Finder aesthetic
 */

#pragma once

#include <opensef/OSFWindowDecorations.h>
#include <opensef/OpenSEFBackend.h>
#include <opensef/OpenSEFBase.h>

#include <ctime>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace vitusos::filer {

using namespace opensef;

// ============================================================================
// FileItem - Represents a file or directory
// ============================================================================

struct FileItem {
  std::string name;
  std::string path;
  std::string extension;

  bool isDirectory = false;
  bool isHidden = false;
  bool isSymlink = false;

  uint64_t size = 0;
  std::time_t modified = 0;

  std::string displaySize() const;
  std::string displayDate() const;
};

// ============================================================================
// View modes
// ============================================================================

enum class ViewMode { Column, Icon, List };

// ============================================================================
// FilerWindow - Main window (pure C++)
// ============================================================================

class FilerWindow {
public:
  FilerWindow();
  ~FilerWindow();

  void show();
  void hide();
  bool isVisible() const { return visible_; }

  void navigateTo(const std::string &path);

private:
  bool visible_ = false;
  std::shared_ptr<OSFWaylandSurface> surface_;
  std::string currentPath_;
  std::vector<FileItem> items_;

  void draw();
  std::vector<FileItem> listDirectory(const std::string &path);
};

// ============================================================================
// FilerApp - Main application
// ============================================================================

class FilerApp {
public:
  FilerApp();
  ~FilerApp();

  int run(int argc, char **argv);
  static FilerApp &instance();

private:
  static FilerApp *instance_;
  std::unique_ptr<FilerWindow> window_;
  std::string initialPath_;

  void parseArgs(int argc, char **argv);
};

} // namespace vitusos::filer
