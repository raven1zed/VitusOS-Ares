/**
 * FilerApp.cpp - File manager implementation
 */

#include "Filer.h"
#include <algorithm>
#include <filesystem>
#include <iostream>


namespace fs = std::filesystem;

namespace vitusos::filer {

static FilerApp *g_instance = nullptr;

// ============================================================================
// FileItem helpers
// ============================================================================

std::string FileItem::displaySize() const {
  if (isDirectory)
    return "--";

  if (size < 1024) {
    return std::to_string(size) + " B";
  } else if (size < 1024 * 1024) {
    return std::to_string(size / 1024) + " KB";
  } else if (size < 1024 * 1024 * 1024) {
    return std::to_string(size / (1024 * 1024)) + " MB";
  } else {
    return std::to_string(size / (1024 * 1024 * 1024)) + " GB";
  }
}

std::string FileItem::displayDate() const {
  char buffer[64];
  std::strftime(buffer, sizeof(buffer), "%b %d, %Y", std::localtime(&modified));
  return buffer;
}

// ============================================================================
// FilerApp
// ============================================================================

FilerApp::FilerApp() { g_instance = this; }

FilerApp::~FilerApp() { g_instance = nullptr; }

FilerApp &FilerApp::instance() { return *g_instance; }

int FilerApp::run(int argc, char **argv) {
  std::cout << "[Filer] VitusOS Filer starting..." << std::endl;
  std::cout << "[Filer] openSEF " << OPENSEF_VERSION_STRING << std::endl;
  std::cout << "[Filer] Design: macOS Finder + OS1 aesthetic" << std::endl;

  parseArgs(argc, argv);

  // Connect to Wayland
  OSFBackend *backend = [OSFBackend sharedBackend];
  if (![backend connect]) {
    std::cerr << "[Filer] Failed to connect to display" << std::endl;
    return 1;
  }

  // Apply theme (light mode for file manager)
  [OSFTheme currentTheme].mode = OSFThemeModeLight;
  [[OSFTheme currentTheme] apply];

  // Create window
  window_ = std::make_unique<FilerWindow>();

  if (!initialPath_.empty()) {
    window_->navigateTo(initialPath_);
  } else {
    // Default to home directory
    const char *home = std::getenv("HOME");
    window_->navigateTo(home ? home : "/");
  }

  window_->show();

  std::cout << "[Filer] Ready" << std::endl;

  [backend run];
  [backend disconnect];
  return 0;
}

void FilerApp::parseArgs(int argc, char **argv) {
  if (argc > 1) {
    initialPath_ = argv[1];
  }
}

// ============================================================================
// FilerWindow
// ============================================================================

FilerWindow::FilerWindow() { setupUI(); }

FilerWindow::~FilerWindow() {}

void FilerWindow::setupUI() {
  // OS1-inspired minimal, warm window
  window_ =
      [OSFWindow windowWithTitle:@"Filer" frame:CGRectMake(0, 0, 1024, 640)];

  // Glass background (warm cream tint)
  OSFGlassPanel *bg =
      [OSFGlassPanel glassPanelWithFrame:CGRectMake(0, 0, 1024, 640)];
  bg.blurRadius = [OSFStyle blurRadiusLight];
  bg.tintColor = [OSFColors background];
  bg.tintAlpha = 0.95;
  [window_ setContentView:bg];

  // Create components
  sidebar_ = std::make_unique<FilerSidebar>();
  toolbar_ = std::make_unique<FilerToolbar>();
  columnView_ = std::make_unique<FilerColumnView>();
  preview_ = std::make_unique<FilerPreview>();

  // Wire up callbacks
  sidebar_->onLocationSelected = [this](const std::string &path) {
    navigateTo(path);
  };

  columnView_->onItemSelected = [this](const FileItem &item) {
    preview_->setItem(item);
  };

  columnView_->onItemOpened = [this](const FileItem &item) {
    if (item.isDirectory) {
      navigateTo(item.path);
    } else {
      // TODO: Open file with default app
      std::cout << "[Filer] Opening: " << item.path << std::endl;
    }
  };

  toolbar_->onBack = [this]() { columnView_->navigateBack(); };
  toolbar_->onForward = [this]() { columnView_->navigateForward(); };
  toolbar_->onViewModeChanged = [this](ViewMode mode) { setViewMode(mode); };

  // Add subviews
  [bg addSubview:sidebar_->view()];
  [bg addSubview:toolbar_->view()];
  [bg addSubview:columnView_->view()];
  if (showPreview_) {
    [bg addSubview:preview_->view()];
  }

  layout();
}

void FilerWindow::layout() {
  float windowWidth = 1024;
  float windowHeight = 640;
  float toolbarHeight = 52;

  // Sidebar (left, full height)
  sidebar_->view().frame = CGRectMake(0, 0, sidebarWidth_, windowHeight);

  // Toolbar (top, minus sidebar)
  toolbar_->view().frame =
      CGRectMake(sidebarWidth_, 0, windowWidth - sidebarWidth_, toolbarHeight);

  // Content area
  float contentX = sidebarWidth_;
  float contentY = toolbarHeight;
  float contentWidth = windowWidth - sidebarWidth_;
  float contentHeight = windowHeight - toolbarHeight;

  if (showPreview_) {
    contentWidth -= previewWidth_;
    preview_->view().frame = CGRectMake(windowWidth - previewWidth_, contentY,
                                        previewWidth_, contentHeight);
  }

  columnView_->view().frame =
      CGRectMake(contentX, contentY, contentWidth, contentHeight);
}

void FilerWindow::show() { [window_ show]; }

void FilerWindow::hide() { [window_ close]; }

void FilerWindow::navigateTo(const std::string &path) {
  columnView_->navigateTo(path);
  toolbar_->setPath(path);
  window_.title = [NSString
      stringWithUTF8String:fs::path(path).filename().string().c_str()];
}

void FilerWindow::setViewMode(ViewMode mode) {
  viewMode_ = mode;
  // TODO: Switch between column/icon/list views
}

// ============================================================================
// FilerSidebar
// ============================================================================

FilerSidebar::FilerSidebar() {
  view_ = [OSFGlassPanel glassPanelWithFrame:CGRectZero];
  view_.blurRadius = [OSFStyle blurRadiusMedium];
  view_.tintColor = [OSFColors surface];
  view_.tintAlpha = 0.8;

  setupDefaultLocations();
}

FilerSidebar::~FilerSidebar() {}

void FilerSidebar::setupDefaultLocations() {
  const char *home = std::getenv("HOME");
  std::string homePath = home ? home : "/home";

  // Favorites
  SidebarSection favorites;
  favorites.title = "Favorites";
  favorites.items = {
      {"Home", homePath, "", true, false, false, 0, 0, 0, "folder-home"},
      {"Desktop", homePath + "/Desktop", "", true, false, false, 0, 0, 0,
       "folder-desktop"},
      {"Documents", homePath + "/Documents", "", true, false, false, 0, 0, 0,
       "folder-documents"},
      {"Downloads", homePath + "/Downloads", "", true, false, false, 0, 0, 0,
       "folder-downloads"},
      {"Pictures", homePath + "/Pictures", "", true, false, false, 0, 0, 0,
       "folder-pictures"},
      {"Music", homePath + "/Music", "", true, false, false, 0, 0, 0,
       "folder-music"},
      {"Videos", homePath + "/Videos", "", true, false, false, 0, 0, 0,
       "folder-videos"},
  };
  sections_.push_back(favorites);

  // Locations
  SidebarSection locations;
  locations.title = "Locations";
  locations.items = {
      {"Computer", "/", "", true, false, false, 0, 0, 0, "computer"},
  };
  sections_.push_back(locations);
}

void FilerSidebar::setWidth(float width) {
  CGRect frame = view_.frame;
  frame.size.width = width;
  view_.frame = frame;
}

void FilerSidebar::addSection(const SidebarSection &section) {
  sections_.push_back(section);
  render();
}

void FilerSidebar::render() {
  // TODO: Render sidebar sections
}

// ============================================================================
// FilerToolbar
// ============================================================================

FilerToolbar::FilerToolbar() { setupUI(); }

FilerToolbar::~FilerToolbar() {}

void FilerToolbar::setupUI() {
  view_ = [[OSFView alloc] init];

  // Back button
  backButton_ = [OSFButton buttonWithLabel:@"◀"
                                    action:^{
                                      if (onBack)
                                        onBack();
                                    }];
  backButton_.frame = CGRectMake(8, 10, 32, 32);
  [view_ addSubview:backButton_];

  // Forward button
  forwardButton_ = [OSFButton buttonWithLabel:@"▶"
                                       action:^{
                                         if (onForward)
                                           onForward();
                                       }];
  forwardButton_.frame = CGRectMake(44, 10, 32, 32);
  [view_ addSubview:forwardButton_];

  // Path display (breadcrumb style)
  pathField_ = [[OSFTextField alloc] init];
  pathField_.frame = CGRectMake(84, 10, 400, 32);
  [view_ addSubview:pathField_];

  // Search (right side)
  searchField_ = [[OSFTextField alloc] init];
  searchField_.placeholder = @"Search";
  searchField_.frame = CGRectMake(600, 10, 200, 32);
  [view_ addSubview:searchField_];
}

void FilerToolbar::setPath(const std::string &path) {
  pathField_.text = [NSString stringWithUTF8String:path.c_str()];
}

void FilerToolbar::setViewMode(ViewMode mode) {
  // TODO: Update view mode buttons
}

// ============================================================================
// FilerColumnView
// ============================================================================

FilerColumnView::FilerColumnView() { view_ = [[OSFScrollView alloc] init]; }

FilerColumnView::~FilerColumnView() {}

void FilerColumnView::navigateTo(const std::string &path) {
  currentPath_ = path;
  columns_.clear();
  columns_.push_back(path);

  // Add to history
  if (historyIndex_ < static_cast<int>(history_.size()) - 1) {
    history_.resize(historyIndex_ + 1);
  }
  history_.push_back(path);
  historyIndex_ = static_cast<int>(history_.size()) - 1;

  renderColumns();
}

void FilerColumnView::navigateBack() {
  if (historyIndex_ > 0) {
    historyIndex_--;
    currentPath_ = history_[historyIndex_];
    columns_.clear();
    columns_.push_back(currentPath_);
    renderColumns();
  }
}

void FilerColumnView::navigateForward() {
  if (historyIndex_ < static_cast<int>(history_.size()) - 1) {
    historyIndex_++;
    currentPath_ = history_[historyIndex_];
    columns_.clear();
    columns_.push_back(currentPath_);
    renderColumns();
  }
}

void FilerColumnView::navigateUp() {
  fs::path p(currentPath_);
  if (p.has_parent_path() && p.parent_path() != p) {
    navigateTo(p.parent_path().string());
  }
}

std::vector<FileItem> FilerColumnView::listDirectory(const std::string &path) {
  std::vector<FileItem> items;

  try {
    for (const auto &entry : fs::directory_iterator(path)) {
      FileItem item;
      item.name = entry.path().filename().string();
      item.path = entry.path().string();
      item.isDirectory = entry.is_directory();
      item.isHidden = item.name[0] == '.';
      item.isSymlink = entry.is_symlink();

      if (!entry.is_directory()) {
        item.size = entry.file_size();
        item.extension = entry.path().extension().string();
      }

      items.push_back(item);
    }
  } catch (const std::exception &e) {
    std::cerr << "[Filer] Error listing " << path << ": " << e.what()
              << std::endl;
  }

  // Sort: directories first, then alphabetical
  std::sort(items.begin(), items.end(),
            [](const FileItem &a, const FileItem &b) {
              if (a.isDirectory != b.isDirectory)
                return a.isDirectory > b.isDirectory;
              return a.name < b.name;
            });

  return items;
}

void FilerColumnView::loadColumn(int index, const std::string &path) {
  if (index >= static_cast<int>(columns_.size())) {
    columns_.push_back(path);
  } else {
    columns_[index] = path;
    columns_.resize(index + 1);
  }
  renderColumns();
}

void FilerColumnView::renderColumns() {
  // TODO: Render column view with items
  std::cout << "[Filer] Rendering " << columns_.size() << " columns"
            << std::endl;
}

// ============================================================================
// FilerPreview
// ============================================================================

FilerPreview::FilerPreview() {
  view_ = [OSFGlassPanel glassPanelWithFrame:CGRectZero];
  view_.blurRadius = [OSFStyle blurRadiusLight];
  view_.tintColor = [OSFColors surface];
  view_.tintAlpha = 0.9;
}

FilerPreview::~FilerPreview() {}

void FilerPreview::setItem(const FileItem &item) {
  currentItem_ = item;

  if (item.isDirectory) {
    renderGenericPreview(item);
  } else if (item.extension == ".txt" || item.extension == ".md" ||
             item.extension == ".cpp" || item.extension == ".h") {
    renderTextPreview(item.path);
  } else if (item.extension == ".png" || item.extension == ".jpg" ||
             item.extension == ".jpeg" || item.extension == ".gif") {
    renderImagePreview(item.path);
  } else {
    renderGenericPreview(item);
  }
}

void FilerPreview::clear() { currentItem_ = {}; }

void FilerPreview::renderTextPreview(const std::string &path) {
  // TODO: Display text preview
}

void FilerPreview::renderImagePreview(const std::string &path) {
  // TODO: Display image preview
}

void FilerPreview::renderVideoPreview(const std::string &path) {
  // TODO: Display video thumbnail
}

void FilerPreview::renderGenericPreview(const FileItem &item) {
  // TODO: Display icon, name, size, date
}

} // namespace vitusos::filer
