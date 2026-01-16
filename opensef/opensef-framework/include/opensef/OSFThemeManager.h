#pragma once

#include <memory>
#include <string>

namespace OpenSEF {

/**
 * OSFThemeManager - Unified Theme System
 *
 * Single theme system for all components.
 * Provides colors, fonts, dimensions, etc.
 *
 * Usage:
 *   auto* theme = desktop->themeManager();
 *   auto color = theme->primaryColor();
 *   int height = theme->panelHeight();
 */
class OSFThemeManager {
public:
  struct Color {
    double r, g, b, a;

    Color() : r(0), g(0), b(0), a(1) {}
    Color(double r, double g, double b, double a = 1.0)
        : r(r), g(g), b(b), a(a) {}
  };

  OSFThemeManager();
  ~OSFThemeManager();

  // Color palette
  Color primaryColor();
  Color backgroundColor();
  Color textColor();
  Color accentColor();
  Color borderColor();
  Color highlightColor();

  // Dimensions
  int panelHeight();
  int dockIconSize();
  int windowBorderWidth();
  int cornerRadius();

  // Fonts
  std::string systemFontFamily();
  int systemFontSize();
  std::string titleFontFamily();
  int titleFontSize();

  // Theme switching
  void loadTheme(const std::string &name);
  std::string currentTheme();

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace OpenSEF
