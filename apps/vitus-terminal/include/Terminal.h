/**
 * VitusOS Terminal - openSEF Native Terminal Emulator
 *
 * Design: OS1 (Her 2013) + macOS Terminal aesthetic
 * - Minimal chrome, maximum content
 * - Soft, warm colors (cream background in light mode)
 * - Subtle transparency/blur
 * - Clean monospace font (JetBrains Mono or similar)
 */

#pragma once

#include <functional>
#include <memory>
#include <opensef/opensef.h>
#include <string>
#include <vector>


namespace vitusos::terminal {

// ============================================================================
// Terminal Theme
// ============================================================================

struct TerminalTheme {
  // Background (slightly warmer than pure black/white)
  std::string background = "#1E1E2E";      // Dark: soft charcoal
  std::string backgroundLight = "#FAF8F5"; // Light: warm cream

  // Text
  std::string foreground = "#CDD6F4";
  std::string foregroundLight = "#1A1A2E";

  // Cursor (Space Orange - Ares accent)
  std::string cursor = "#E85D04";
  std::string cursorText = "#1E1E2E";

  // Selection
  std::string selection = "#45475A";
  std::string selectionLight = "#E5DFD5";

  // ANSI colors (Ares-inspired palette)
  std::string black = "#45475A";
  std::string red = "#F38BA8";
  std::string green = "#A6E3A1";
  std::string yellow = "#F9E2AF";
  std::string blue = "#89B4FA";
  std::string magenta = "#CBA6F7";
  std::string cyan = "#94E2D5";
  std::string white = "#BAC2DE";

  // Bright variants
  std::string brightBlack = "#585B70";
  std::string brightRed = "#F38BA8";
  std::string brightGreen = "#A6E3A1";
  std::string brightYellow = "#F9E2AF";
  std::string brightBlue = "#89B4FA";
  std::string brightMagenta = "#CBA6F7";
  std::string brightCyan = "#94E2D5";
  std::string brightWhite = "#A6ADC8";
};

// ============================================================================
// Terminal Configuration
// ============================================================================

struct TerminalConfig {
  std::string fontFamily = "JetBrains Mono";
  int fontSize = 13;
  float lineHeight = 1.2;
  float letterSpacing = 0.0;

  int scrollbackLines = 10000;
  bool cursorBlink = true;
  std::string cursorStyle = "block"; // block, underline, bar

  float backgroundOpacity = 0.95; // Slight transparency
  float blurRadius = 8.0;         // Subtle blur behind

  std::string shell = "/bin/bash";
  std::vector<std::string> shellArgs = {};

  TerminalTheme theme;
};

// ============================================================================
// TerminalBuffer - Screen buffer with scrollback
// ============================================================================

struct TerminalCell {
  char32_t character = ' ';
  uint8_t fgColor = 7; // White
  uint8_t bgColor = 0; // Black
  bool bold = false;
  bool italic = false;
  bool underline = false;
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
  std::vector<std::vector<TerminalCell>> scrollback_;
  int cols_, rows_;
  int cursorCol_ = 0, cursorRow_ = 0;
};

// ============================================================================
// TerminalView - openSEF view for rendering
// ============================================================================

class TerminalView {
public:
  TerminalView();
  ~TerminalView();

  OSFView *view() { return view_; }

  void setBuffer(TerminalBuffer *buffer);
  void setConfig(const TerminalConfig &config);
  void render();

  // Input
  void onKeyDown(const std::string &key, unsigned int modifiers);
  void onKeyUp(const std::string &key, unsigned int modifiers);
  void onTextInput(const std::string &text);

  // Callbacks
  std::function<void(const std::string &)> onInput;

private:
  OSFGlassPanel *view_;
  TerminalBuffer *buffer_;
  TerminalConfig config_;

  void drawCell(int col, int row, const TerminalCell &cell);
  void drawCursor();
};

// ============================================================================
// TerminalWindow - Main window
// ============================================================================

class TerminalWindow {
public:
  TerminalWindow();
  ~TerminalWindow();

  void show();
  void hide();

  void setTitle(const std::string &title);
  void newTab();
  void closeTab(int index);

private:
  OSFWindow *window_;
  std::vector<std::unique_ptr<TerminalView>> tabs_;
  int activeTab_ = 0;

  void setupUI();
  void setupTabBar();
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
  TerminalConfig config_;
  std::unique_ptr<TerminalWindow> window_;

  void loadConfig();
  void saveConfig();
};

} // namespace vitusos::terminal
