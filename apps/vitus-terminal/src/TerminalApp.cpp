/**
 * TerminalApp.cpp - Terminal application implementation
 */

#include "Terminal.h"
#include <iostream>

namespace vitusos::terminal {

static TerminalApp *g_instance = nullptr;

TerminalApp::TerminalApp() {
  g_instance = this;
  loadConfig();
}

TerminalApp::~TerminalApp() {
  saveConfig();
  g_instance = nullptr;
}

TerminalApp &TerminalApp::instance() { return *g_instance; }

int TerminalApp::run(int argc, char **argv) {
  std::cout << "[Terminal] VitusOS Terminal starting..." << std::endl;
  std::cout << "[Terminal] openSEF " << OPENSEF_VERSION_STRING << std::endl;
  std::cout << "[Terminal] Design: OS1 + macOS minimal aesthetic" << std::endl;

  // Connect to Wayland
  OSFBackend *backend = [OSFBackend sharedBackend];
  if (![backend connect]) {
    std::cerr << "[Terminal] Failed to connect to display" << std::endl;
    return 1;
  }

  // Apply dark theme by default for terminal
  [OSFTheme currentTheme].mode = OSFThemeModeDark;
  [[OSFTheme currentTheme] apply];

  // Create window
  window_ = std::make_unique<TerminalWindow>();
  window_->show();

  std::cout << "[Terminal] Ready" << std::endl;

  // Run event loop
  [backend run];

  [backend disconnect];
  return 0;
}

void TerminalApp::loadConfig() {
  // TODO: Load from ~/.config/vitus-terminal/config.conf
  config_ = TerminalConfig{};
  std::cout << "[Terminal] Using default config" << std::endl;
}

void TerminalApp::saveConfig() {
  // TODO: Save to config file
}

// ============================================================================
// TerminalWindow
// ============================================================================

TerminalWindow::TerminalWindow() { setupUI(); }

TerminalWindow::~TerminalWindow() {}

void TerminalWindow::setupUI() {
  // OS1-style minimal window
  // - No visible title bar (integrated)
  // - Subtle rounded corners
  // - Glass effect background

  window_ =
      [OSFWindow windowWithTitle:@"Terminal" frame:CGRectMake(0, 0, 720, 480)];

  // Glass panel background (slight transparency)
  OSFGlassPanel *bg =
      [OSFGlassPanel glassPanelWithFrame:CGRectMake(0, 0, 720, 480)];
  bg.blurRadius = TerminalApp::instance().config().blurRadius;
  bg.tintAlpha = TerminalApp::instance().config().backgroundOpacity;
  // Dark charcoal background
  bg.tintColor = [NSColor colorWithRed:0.12 green:0.12 blue:0.18 alpha:1.0];
  [window_ setContentView:bg];

  // Tab bar (macOS-style, minimal)
  setupTabBar();

  // Create initial tab
  newTab();
}

void TerminalWindow::setupTabBar() {
  // macOS-style tab bar
  // - Subtle, integrated with window
  // - Tabs are just text, minimal dividers
  // - New tab button (+) on right

  // TODO: Create OSFTabBar view
}

void TerminalWindow::show() { [window_ show]; }

void TerminalWindow::hide() { [window_ close]; }

void TerminalWindow::setTitle(const std::string &title) {
  window_.title = [NSString stringWithUTF8String:title.c_str()];
}

void TerminalWindow::newTab() {
  auto tab = std::make_unique<TerminalView>();
  // TODO: Set up PTY and shell
  tabs_.push_back(std::move(tab));
  activeTab_ = static_cast<int>(tabs_.size()) - 1;
}

void TerminalWindow::closeTab(int index) {
  if (index >= 0 && index < static_cast<int>(tabs_.size())) {
    tabs_.erase(tabs_.begin() + index);
    if (activeTab_ >= static_cast<int>(tabs_.size())) {
      activeTab_ = static_cast<int>(tabs_.size()) - 1;
    }
  }
}

// ============================================================================
// TerminalView
// ============================================================================

TerminalView::TerminalView() {
  view_ = [OSFGlassPanel glassPanelWithFrame:CGRectMake(0, 28, 720, 452)];
  buffer_ = nullptr;
}

TerminalView::~TerminalView() {}

void TerminalView::setBuffer(TerminalBuffer *buffer) { buffer_ = buffer; }

void TerminalView::setConfig(const TerminalConfig &config) { config_ = config; }

void TerminalView::render() {
  if (!buffer_)
    return;

  // TODO: Render cells using Vulkan text rendering
  for (int row = 0; row < buffer_->rows(); row++) {
    for (int col = 0; col < buffer_->cols(); col++) {
      drawCell(col, row, buffer_->at(col, row));
    }
  }

  drawCursor();
}

void TerminalView::drawCell(int col, int row, const TerminalCell &cell) {
  // TODO: Draw character with font and colors
}

void TerminalView::drawCursor() {
  if (!buffer_)
    return;

  // Space Orange cursor (Ares accent)
  // TODO: Draw cursor block/underline/bar
}

void TerminalView::onKeyDown(const std::string &key, unsigned int modifiers) {
  // TODO: Handle key input, send to PTY
}

void TerminalView::onKeyUp(const std::string &key, unsigned int modifiers) {
  // TODO: Handle key release
}

void TerminalView::onTextInput(const std::string &text) {
  if (onInput) {
    onInput(text);
  }
}

// ============================================================================
// TerminalBuffer
// ============================================================================

TerminalBuffer::TerminalBuffer(int cols, int rows) : cols_(cols), rows_(rows) {
  cells_.resize(cols * rows);
}

void TerminalBuffer::write(char32_t ch) {
  if (cursorCol_ >= cols_) {
    newline();
  }
  cells_[cursorRow_ * cols_ + cursorCol_].character = ch;
  cursorCol_++;
}

void TerminalBuffer::write(const std::string &str) {
  for (char c : str) {
    if (c == '\n') {
      newline();
    } else {
      write(static_cast<char32_t>(c));
    }
  }
}

void TerminalBuffer::newline() {
  cursorCol_ = 0;
  cursorRow_++;
  if (cursorRow_ >= rows_) {
    // Scroll up, add to scrollback
    // TODO: Implement scrollback
    cursorRow_ = rows_ - 1;
  }
}

void TerminalBuffer::clear() {
  for (auto &cell : cells_) {
    cell = TerminalCell{};
  }
  cursorCol_ = 0;
  cursorRow_ = 0;
}

void TerminalBuffer::resize(int cols, int rows) {
  cols_ = cols;
  rows_ = rows;
  cells_.resize(cols * rows);
}

TerminalCell &TerminalBuffer::at(int col, int row) {
  return cells_[row * cols_ + col];
}

const TerminalCell &TerminalBuffer::at(int col, int row) const {
  return cells_[row * cols_ + col];
}

} // namespace vitusos::terminal
