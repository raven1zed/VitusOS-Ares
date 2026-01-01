/**
 * TerminalApp.cpp - Terminal application (pure C++)
 */

#include "Terminal.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <sstream>

namespace vitusos::terminal {

namespace fs = std::filesystem;

static fs::path getConfigPath() {
  const char* home = std::getenv("HOME");
  if (!home) return {};
  fs::path configDir = fs::path(home) / ".config" / "vitus-terminal";
  if (!fs::exists(configDir)) {
    fs::create_directories(configDir);
  }
  return configDir / "config.ini";
}

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
  fs::path path = getConfigPath();

  if (path.empty() || !fs::exists(path)) {
    std::cout << "[Terminal] Using default config" << std::endl;
    return;
  }

  std::ifstream file(path);
  if (!file.is_open()) {
    std::cout << "[Terminal] Using default config (failed to open file)" << std::endl;
    return;
  }

  std::string line;
  std::string currentSection;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;
    if (line[0] == '[') {
      size_t end = line.find(']');
      if (end != std::string::npos) {
        currentSection = line.substr(1, end - 1);
      }
      continue;
    }

    size_t delimiterPos = line.find('=');
    if (delimiterPos == std::string::npos) continue;

    std::string key = line.substr(0, delimiterPos);
    std::string value = line.substr(delimiterPos + 1);

    // Trim whitespace if needed (simple trim)
    // For now assuming clean writes

    if (currentSection == "Theme") {
      if (key == "background") config_.theme.background = value;
      else if (key == "foreground") config_.theme.foreground = value;
      else if (key == "cursor") config_.theme.cursor = value;
      else if (key == "black") config_.theme.black = value;
      else if (key == "red") config_.theme.red = value;
      else if (key == "green") config_.theme.green = value;
      else if (key == "yellow") config_.theme.yellow = value;
      else if (key == "blue") config_.theme.blue = value;
      else if (key == "magenta") config_.theme.magenta = value;
      else if (key == "cyan") config_.theme.cyan = value;
      else if (key == "white") config_.theme.white = value;
    } else {
      try {
        if (key == "fontFamily") config_.fontFamily = value;
        else if (key == "fontSize") config_.fontSize = std::stoi(value);
        else if (key == "lineHeight") config_.lineHeight = std::stof(value);
        else if (key == "scrollbackLines") config_.scrollbackLines = std::stoi(value);
        else if (key == "cursorBlink") config_.cursorBlink = (value == "true");
        else if (key == "backgroundOpacity") config_.backgroundOpacity = std::stof(value);
        else if (key == "shell") config_.shell = value;
      } catch (...) {
        // Ignore conversion errors
      }
    }
  }
  std::cout << "[Terminal] Config loaded from " << path << std::endl;
}

void TerminalApp::saveConfig() {
  fs::path path = getConfigPath();
  if (path.empty()) return;

  std::ofstream file(path);
  if (!file.is_open()) {
    std::cerr << "[Terminal] Failed to save config to " << path << std::endl;
    return;
  }

  file << "fontFamily=" << config_.fontFamily << "\n";
  file << "fontSize=" << config_.fontSize << "\n";
  file << "lineHeight=" << config_.lineHeight << "\n";
  file << "scrollbackLines=" << config_.scrollbackLines << "\n";
  file << "cursorBlink=" << (config_.cursorBlink ? "true" : "false") << "\n";
  file << "backgroundOpacity=" << config_.backgroundOpacity << "\n";
  file << "shell=" << config_.shell << "\n";

  file << "\n[Theme]\n";
  file << "background=" << config_.theme.background << "\n";
  file << "foreground=" << config_.theme.foreground << "\n";
  file << "cursor=" << config_.theme.cursor << "\n";
  file << "black=" << config_.theme.black << "\n";
  file << "red=" << config_.theme.red << "\n";
  file << "green=" << config_.theme.green << "\n";
  file << "yellow=" << config_.theme.yellow << "\n";
  file << "blue=" << config_.theme.blue << "\n";
  file << "magenta=" << config_.theme.magenta << "\n";
  file << "cyan=" << config_.theme.cyan << "\n";
  file << "white=" << config_.theme.white << "\n";

  std::cout << "[Terminal] Config saved to " << path << std::endl;
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
