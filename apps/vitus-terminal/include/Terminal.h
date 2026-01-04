/**
 * VitusOS Terminal - openSEF Native Terminal Emulator
 *
 * Design: OS1 (Her 2013) + macOS Terminal aesthetic
 */

#pragma once

#include <opensef/OSFWindowDecorations.h>
#include <opensef/OpenSEFBackend.h>
#include <opensef/OpenSEFBase.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace vitusos::terminal {

using namespace opensef;

// ============================================================================
// Terminal Theme
// ============================================================================

struct TerminalTheme {
  std::string background = "#1E1E2E";
  std::string foreground = "#CDD6F4";
  std::string cursor = "#E85D04"; // Space Orange

  // ANSI colors
  std::string black = "#45475A";
  std::string red = "#F38BA8";
  std::string green = "#A6E3A1";
  std::string yellow = "#F9E2AF";
  std::string blue = "#89B4FA";
  std::string magenta = "#CBA6F7";
  std::string cyan = "#94E2D5";
  std::string white = "#BAC2DE";
};

// ============================================================================
// Terminal Configuration
// ============================================================================

struct TerminalConfig {
  std::string fontFamily = "JetBrains Mono";
  int fontSize = 13;
  float lineHeight = 1.2f;
  int scrollbackLines = 10000;
  bool cursorBlink = true;
  float backgroundOpacity = 0.95f;
  std::string shell = "/bin/bash";
  TerminalTheme theme;
};

// ============================================================================
// TerminalBuffer - Screen buffer
// ============================================================================

struct TerminalCell {
  char32_t character = ' ';
  uint8_t fgColor = 7;
  uint8_t bgColor = 0;
  bool bold = false;
};

class TerminalBuffer {
public:
  TerminalBuffer(int cols, int rows);

  void write(char32_t ch);
  void write(const std::string &str);
  void newline();
  void clear();
  void resize(int cols, int rows);

  TerminalCell &at(int col, int row);
  const TerminalCell &at(int col, int row) const;

  int cols() const { return cols_; }
  int rows() const { return rows_; }
  int cursorCol() const { return cursorCol_; }
  int cursorRow() const { return cursorRow_; }

private:
  std::vector<TerminalCell> cells_;
  int cols_, rows_;
  int cursorCol_ = 0, cursorRow_ = 0;
};

// ============================================================================
// TerminalWindow - Main window (pure C++)
// ============================================================================

class TerminalWindow {
public:
  TerminalWindow();
  ~TerminalWindow();

  void show();
  void hide();
  bool isVisible() const { return visible_; }

private:
  bool visible_ = false;
  std::shared_ptr<OSFWaylandSurface> surface_;
  std::unique_ptr<TerminalBuffer> buffer_;
  TerminalConfig config_;

  void draw();
};

// ============================================================================
// TerminalApp - Main application
// ============================================================================

class TerminalApp {
public:
  TerminalApp();
  ~TerminalApp();

  int run(int argc, char **argv);
  static TerminalApp &instance();

  TerminalConfig &config() { return config_; }

private:
  static TerminalApp *instance_;
  TerminalConfig config_;
  std::unique_ptr<TerminalWindow> window_;

  void loadConfig();
  void saveConfig();
};

} // namespace vitusos::terminal
