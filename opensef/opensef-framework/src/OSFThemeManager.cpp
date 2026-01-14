#include "OSFThemeManager.h"
#include <cstdint>

namespace OpenSEF {

struct OSFThemeManager::Impl {
  std::string currentThemeName = "Ares";

  // Ares theme colors (Mars-inspired)
  Color primary{0.8, 0.3, 0.2, 1.0};       // Mars red
  Color background{0.15, 0.12, 0.11, 1.0}; // Dark brown
  Color text{0.95, 0.95, 0.95, 1.0};       // Light gray
  Color accent{0.9, 0.5, 0.3, 1.0};        // Orange
  Color border{0.3, 0.25, 0.22, 1.0};      // Dark border
  Color highlight{0.95, 0.6, 0.4, 1.0};    // Light orange
};

OSFThemeManager::OSFThemeManager() : impl_(std::make_unique<Impl>()) {}

OSFThemeManager::~OSFThemeManager() = default;

OSFThemeManager::Color OSFThemeManager::primaryColor() {
  return impl_->primary;
}

OSFThemeManager::Color OSFThemeManager::backgroundColor() {
  return impl_->background;
}

OSFThemeManager::Color OSFThemeManager::textColor() { return impl_->text; }

OSFThemeManager::Color OSFThemeManager::accentColor() { return impl_->accent; }

OSFThemeManager::Color OSFThemeManager::borderColor() { return impl_->border; }

OSFThemeManager::Color OSFThemeManager::highlightColor() {
  return impl_->highlight;
}

int OSFThemeManager::panelHeight() { return 28; }

int OSFThemeManager::dockIconSize() { return 48; }

int OSFThemeManager::windowBorderWidth() { return 1; }

int OSFThemeManager::cornerRadius() { return 8; }

std::string OSFThemeManager::systemFontFamily() { return "Sans"; }

int OSFThemeManager::systemFontSize() { return 11; }

std::string OSFThemeManager::titleFontFamily() { return "Sans"; }

int OSFThemeManager::titleFontSize() { return 12; }

void OSFThemeManager::loadTheme(const std::string &name) {
  impl_->currentThemeName = name;
  // Would load theme from file here
}

std::string OSFThemeManager::currentTheme() { return impl_->currentThemeName; }

} // namespace OpenSEF
