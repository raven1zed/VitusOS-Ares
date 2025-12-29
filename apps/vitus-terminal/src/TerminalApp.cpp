/**
 * TerminalApp.cpp - Terminal application (pure C++)
 */

#include "Terminal.h"
#include <iostream>

namespace vitusos::terminal {

TerminalApp *TerminalApp::instance_ = nullptr;

TerminalApp::TerminalApp() {
  instance_ = this;
  loadConfig();
}

TerminalApp::~TerminalApp() {
  saveConfig();
  instance_ = nullptr;
}

TerminalApp &TerminalApp::instance() { return *instance_; }

int TerminalApp::run(int argc, char **argv) {
  (void)argc;
  (void)argv;

  std::cout << "╔════════════════════════════════════════════╗" << std::endl;
  std::cout << "║     VitusOS Terminal                       ║" << std::endl;
  std::cout << "║     OS1 + macOS Aesthetic                  ║" << std::endl;
  std::cout << "╚════════════════════════════════════════════╝" << std::endl;
  std::cout << std::endl;

  // Connect to Wayland
  auto &backend = OSFBackend::shared();
  if (!backend.connect()) {
    std::cerr << "[Terminal] Failed to connect to display" << std::endl;
    return 1;
  }

  // Create window
  window_ = std::make_unique<TerminalWindow>();
  window_->show();

  std::cout << "[Terminal] Ready" << std::endl;

  // Run event loop
  backend.run();

  backend.disconnect();
  return 0;
}

void TerminalApp::loadConfig() {
  config_ = TerminalConfig{};
  std::cout << "[Terminal] Using default config" << std::endl;
}

void TerminalApp::saveConfig() {
  // TODO: Save to config file
}

// ============================================================================
// TerminalWindow
// ============================================================================

TerminalWindow::TerminalWindow() {
  buffer_ = std::make_unique<TerminalBuffer>(80, 24);
}

TerminalWindow::~TerminalWindow() { hide(); }

void TerminalWindow::show() {
  if (visible_)
    return;

  surface_ = OSFWaylandSurface::create(720, 480, "Terminal");
  if (!surface_) {
    std::cerr << "[Terminal] Failed to create window" << std::endl;
    return;
  }

  visible_ = true;
  draw();

  std::cout << "[Terminal] Window shown (80x24)" << std::endl;
}

void TerminalWindow::hide() {
  if (!visible_)
    return;
  surface_.reset();
  visible_ = false;
}

void TerminalWindow::draw() {
  if (!surface_)
    return;

  // Dark charcoal background
  OSFColor bg = OSFColors::backgroundDark();
  surface_->draw(bg);
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
