/**
 * FilerApp.cpp - File manager (pure C++)
 */

#include "Filer.h"
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <iostream>


namespace fs = std::filesystem;

namespace vitusos::filer {

FilerApp *FilerApp::instance_ = nullptr;

// FileItem helpers
std::string FileItem::displaySize() const {
  if (isDirectory)
    return "--";

  if (size < 1024) {
    return std::to_string(size) + " B";
  } else if (size < 1024 * 1024) {
    return std::to_string(size / 1024) + " KB";
  } else if (size < 1024ULL * 1024 * 1024) {
    return std::to_string(size / (1024 * 1024)) + " MB";
  } else {
    return std::to_string(size / (1024ULL * 1024 * 1024)) + " GB";
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

FilerApp::FilerApp() { instance_ = this; }

FilerApp::~FilerApp() { instance_ = nullptr; }

FilerApp &FilerApp::instance() { return *instance_; }

int FilerApp::run(int argc, char **argv) {
  std::cout << "╔════════════════════════════════════════════╗" << std::endl;
  std::cout << "║     VitusOS Filer                          ║" << std::endl;
  std::cout << "║     macOS Finder + OS1 Aesthetic           ║" << std::endl;
  std::cout << "╚════════════════════════════════════════════╝" << std::endl;
  std::cout << std::endl;

  parseArgs(argc, argv);

  // Connect to Wayland
  auto &backend = OSFBackend::shared();
  if (!backend.connect()) {
    std::cerr << "[Filer] Failed to connect to display" << std::endl;
    return 1;
  }

  // Create window
  window_ = std::make_unique<FilerWindow>();

  if (!initialPath_.empty()) {
    window_->navigateTo(initialPath_);
  } else {
    const char *home = std::getenv("HOME");
    window_->navigateTo(home ? home : "/");
  }

  window_->show();

  std::cout << "[Filer] Ready" << std::endl;

  backend.run();
  backend.disconnect();
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

FilerWindow::FilerWindow() = default;

FilerWindow::~FilerWindow() { hide(); }

void FilerWindow::show() {
  if (visible_)
    return;

  surface_ = OSFWaylandSurface::create(1024, 640, "Filer");
  if (!surface_) {
    std::cerr << "[Filer] Failed to create window" << std::endl;
    return;
  }

  visible_ = true;
  draw();

  std::cout << "[Filer] Window shown" << std::endl;
}

void FilerWindow::hide() {
  if (!visible_)
    return;
  surface_.reset();
  visible_ = false;
}

void FilerWindow::navigateTo(const std::string &path) {
  currentPath_ = path;
  items_ = listDirectory(path);

  std::cout << "[Filer] Navigated to: " << path << std::endl;
  std::cout << "[Filer] Found " << items_.size() << " items" << std::endl;

  if (visible_) {
    draw();
  }
}

void FilerWindow::draw() {
  if (!surface_)
    return;

  // Warm cream background (light mode)
  OSFColor bg = OSFColors::background();
  surface_->draw(bg);
}

std::vector<FileItem> FilerWindow::listDirectory(const std::string &path) {
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

} // namespace vitusos::filer
