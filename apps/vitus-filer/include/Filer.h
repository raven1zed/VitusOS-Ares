/**
 * VitusOS Filer - openSEF Native File Manager
 *
 * Design: OS1 (Her 2013) + macOS Finder aesthetic
 * - Column view default (like Finder)
 * - Minimal toolbar
 * - Warm, soft colors
 * - Smooth animations
 * - Glass sidebar
 */

#pragma once

#include <ctime>
#include <functional>
#include <memory>
#include <opensef/opensef.h>
#include <string>
#include <vector>


namespace vitusos::filer {

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

  uint64_t size = 0; // bytes
  std::time_t modified = 0;
  std::time_t created = 0;

  std::string icon; // Icon name or path

  // For display
  std::string displaySize() const;
  std::string displayDate() const;
};

// ============================================================================
// FilerView - View modes
// ============================================================================

enum class ViewMode {
  Column, // macOS Finder column view (default)
  Icon,   // Icon grid
  List,   // Detailed list
  Gallery // Image preview (for photos)
};

// ============================================================================
// SidebarItem - Sidebar locations
// ============================================================================

struct SidebarSection {
  std::string title;
  std::vector<FileItem> items;
};

// ============================================================================
// FilerSidebar - Glass sidebar with locations
// ============================================================================

class FilerSidebar {
public:
  FilerSidebar();
  ~FilerSidebar();

  OSFView *view() { return view_; }

  void setWidth(float width);
  void addSection(const SidebarSection &section);

  std::function<void(const std::string &path)> onLocationSelected;

private:
  OSFGlassPanel *view_;
  std::vector<SidebarSection> sections_;

  void setupDefaultLocations();
  void render();
};

// ============================================================================
// FilerColumnView - macOS-style column browser
// ============================================================================

class FilerColumnView {
public:
  FilerColumnView();
  ~FilerColumnView();

  OSFView *view() { return view_; }

  void navigateTo(const std::string &path);
  void navigateBack();
  void navigateForward();
  void navigateUp();

  std::string currentPath() const { return currentPath_; }

  std::function<void(const FileItem &item)> onItemSelected;
  std::function<void(const FileItem &item)> onItemOpened;

private:
  OSFScrollView *view_;
  std::vector<std::string> columns_; // Paths for each column
  std::string currentPath_;
  std::vector<std::string> history_;
  int historyIndex_ = -1;

  void loadColumn(int index, const std::string &path);
  void renderColumns();
  std::vector<FileItem> listDirectory(const std::string &path);
};

// ============================================================================
// FilerToolbar - Minimal toolbar
// ============================================================================

class FilerToolbar {
public:
  FilerToolbar();
  ~FilerToolbar();

  OSFView *view() { return view_; }

  void setPath(const std::string &path);
  void setViewMode(ViewMode mode);

  std::function<void()> onBack;
  std::function<void()> onForward;
  std::function<void(ViewMode)> onViewModeChanged;
  std::function<void(const std::string &)> onSearch;

private:
  OSFView *view_;
  OSFButton *backButton_;
  OSFButton *forwardButton_;
  OSFTextField *pathField_;
  OSFTextField *searchField_;

  void setupUI();
};

// ============================================================================
// FilerPreview - Quick Look style preview
// ============================================================================

class FilerPreview {
public:
  FilerPreview();
  ~FilerPreview();

  OSFView *view() { return view_; }

  void setItem(const FileItem &item);
  void clear();

private:
  OSFGlassPanel *view_;
  FileItem currentItem_;

  void renderTextPreview(const std::string &path);
  void renderImagePreview(const std::string &path);
  void renderVideoPreview(const std::string &path);
  void renderGenericPreview(const FileItem &item);
};

// ============================================================================
// FilerWindow - Main window
// ============================================================================

class FilerWindow {
public:
  FilerWindow();
  ~FilerWindow();

  void show();
  void hide();

  void navigateTo(const std::string &path);
  void setViewMode(ViewMode mode);

private:
  OSFWindow *window_;
  std::unique_ptr<FilerSidebar> sidebar_;
  std::unique_ptr<FilerToolbar> toolbar_;
  std::unique_ptr<FilerColumnView> columnView_;
  std::unique_ptr<FilerPreview> preview_;

  ViewMode viewMode_ = ViewMode::Column;
  bool showPreview_ = true;
  float sidebarWidth_ = 200.0f;
  float previewWidth_ = 260.0f;

  void setupUI();
  void layout();
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
  std::unique_ptr<FilerWindow> window_;
  std::string initialPath_;

  void parseArgs(int argc, char **argv);
};

} // namespace vitusos::filer
