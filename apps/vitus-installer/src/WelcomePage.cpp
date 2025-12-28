/**
 * WelcomePage.cpp - Welcome screen with large logo
 *
 * Layout:
 * ┌────────────────────────────────────────┐
 * │                                        │
 * │         Welcome to VitusOS             │  ← Large title
 * │                                        │
 * │    Let's set up your new system.       │  ← Subtitle
 * │                                        │
 * │    Language: [English (US)      ▾]     │
 * │    Region:   [United States     ▾]     │
 * │                                        │
 * └────────────────────────────────────────┘
 */

#include "Installer.h"

namespace vitusos::installer {

WelcomePage::WelcomePage() : InstallerPage("Welcome") {
  contentView_ = [[OSFView alloc] init];

  // Large centered title
  titleLabel_ = [[OSFLabel alloc] init];
  titleLabel_.frame = CGRectMake(0, 100, 600, 60);
  titleLabel_.text = @"Welcome to VitusOS";
  titleLabel_.font = [OSFTypography displaySmall]; // 36pt
  titleLabel_.textColor = [OSFColors textPrimary];
  // TODO: Center text alignment
  [contentView_ addSubview:titleLabel_];

  // Subtitle
  subtitleLabel_ = [[OSFLabel alloc] init];
  subtitleLabel_.frame = CGRectMake(0, 170, 600, 30);
  subtitleLabel_.text = @"Let's set up your new system.";
  subtitleLabel_.font = [OSFTypography bodyLarge]; // 16pt
  subtitleLabel_.textColor = [OSFColors textSecondary];
  [contentView_ addSubview:subtitleLabel_];

  // Language section
  languageLabel_ = [[OSFLabel alloc] init];
  languageLabel_.frame = CGRectMake(100, 250, 120, 30);
  languageLabel_.text = @"Language";
  languageLabel_.font = [OSFTypography labelLarge];
  languageLabel_.textColor = [OSFColors textPrimary];
  [contentView_ addSubview:languageLabel_];

  // TODO: Language dropdown (placeholder for now)
  OSFLabel *langValue = [[OSFLabel alloc] init];
  langValue.frame = CGRectMake(230, 250, 200, 30);
  langValue.text = @"English (US)";
  langValue.font = [OSFTypography bodyMedium];
  langValue.textColor = [OSFColors textSecondary];
  [contentView_ addSubview:langValue];

  // Region section
  regionLabel_ = [[OSFLabel alloc] init];
  regionLabel_.frame = CGRectMake(100, 300, 120, 30);
  regionLabel_.text = @"Region";
  regionLabel_.font = [OSFTypography labelLarge];
  regionLabel_.textColor = [OSFColors textPrimary];
  [contentView_ addSubview:regionLabel_];

  OSFLabel *regionValue = [[OSFLabel alloc] init];
  regionValue.frame = CGRectMake(230, 300, 200, 30);
  regionValue.text = @"United States";
  regionValue.font = [OSFTypography bodyMedium];
  regionValue.textColor = [OSFColors textSecondary];
  [contentView_ addSubview:regionValue];
}

OSFView *WelcomePage::view() { return contentView_; }

bool WelcomePage::validate() {
  return true; // Always valid for now
}

void WelcomePage::saveConfig(InstallConfig &config) {
  config.language = "en_US";
  config.region = "US";
}

// ============================================================================
// InstallerPage base
// ============================================================================

InstallerPage::InstallerPage(const std::string &title) : title_(title) {}

} // namespace vitusos::installer
