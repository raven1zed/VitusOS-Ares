/**
 * CustomizePage.cpp - Theme and appearance selection
 *
 * Layout:
 * ┌────────────────────────────────────────┐
 * │         Customize your system          │
 * │                                        │
 * │    ┌─────────┐    ┌─────────┐         │
 * │    │  Light  │    │  Dark   │         │  ← Theme selection
 * │    │   ☀️    │    │   🌙    │         │
 * │    └─────────┘    └─────────┘         │
 * │                                        │
 * │    Wallpaper                           │
 * │    ┌───┐ ┌───┐ ┌───┐ ┌───┐           │
 * │    │   │ │   │ │   │ │   │           │  ← Wallpaper grid
 * │    └───┘ └───┘ └───┘ └───┘           │
 * │                                        │
 * │    Accent Color                        │
 * │    ● ● ● ● ● ●                        │  ← Color picker
 * │                                        │
 * └────────────────────────────────────────┘
 */

#include "Installer.h"

namespace vitusos::installer {

CustomizePage::CustomizePage() : InstallerPage("Customize") {
  contentView_ = [[OSFView alloc] init];

  // Title
  OSFLabel *title = [[OSFLabel alloc] init];
  title.frame = CGRectMake(0, 20, 600, 40);
  title.text = @"Customize your system";
  title.font = [OSFTypography headlineMedium];
  title.textColor = [OSFColors textPrimary];
  [contentView_ addSubview:title];

  // Theme section label
  OSFLabel *themeLabel = [[OSFLabel alloc] init];
  themeLabel.frame = CGRectMake(50, 80, 200, 24);
  themeLabel.text = @"Appearance";
  themeLabel.font = [OSFTypography labelLarge];
  themeLabel.textColor = [OSFColors textPrimary];
  [contentView_ addSubview:themeLabel];

  // Light theme preview card
  themePreviewLight_ = [[OSFView alloc] init];
  themePreviewLight_.frame = CGRectMake(100, 110, 160, 100);
  // TODO: Draw light theme preview
  [contentView_ addSubview:themePreviewLight_];

  OSFLabel *lightLabel = [[OSFLabel alloc] init];
  lightLabel.frame = CGRectMake(100, 110, 160, 80);
  lightLabel.text = @"☀️\nLight";
  lightLabel.font = [OSFTypography headlineSmall];
  lightLabel.textColor = [OSFColors textPrimary];
  [themePreviewLight_ addSubview:lightLabel];

  // Dark theme preview card
  themePreviewDark_ = [[OSFView alloc] init];
  themePreviewDark_.frame = CGRectMake(300, 110, 160, 100);
  [contentView_ addSubview:themePreviewDark_];

  OSFLabel *darkLabel = [[OSFLabel alloc] init];
  darkLabel.frame = CGRectMake(0, 0, 160, 80);
  darkLabel.text = @"🌙\nDark";
  darkLabel.font = [OSFTypography headlineSmall];
  darkLabel.textColor = [OSFColors textSecondary];
  [themePreviewDark_ addSubview:darkLabel];

  // Wallpaper section
  OSFLabel *wallpaperLabel = [[OSFLabel alloc] init];
  wallpaperLabel.frame = CGRectMake(50, 230, 200, 24);
  wallpaperLabel.text = @"Wallpaper";
  wallpaperLabel.font = [OSFTypography labelLarge];
  wallpaperLabel.textColor = [OSFColors textPrimary];
  [contentView_ addSubview:wallpaperLabel];

  // Wallpaper grid (4 thumbnails)
  wallpaperGrid_ = [[OSFView alloc] init];
  wallpaperGrid_.frame = CGRectMake(50, 260, 500, 100);
  [contentView_ addSubview:wallpaperGrid_];

  // Create 4 wallpaper thumbnails
  NSArray *wallpapers = @[ @"🌄", @"🌊", @"🏔️", @"🌸" ];
  for (int i = 0; i < 4; i++) {
    OSFView *thumb = [[OSFView alloc] init];
    thumb.frame = CGRectMake(i * 120, 0, 100, 70);

    OSFLabel *emoji = [[OSFLabel alloc] init];
    emoji.frame = CGRectMake(0, 0, 100, 70);
    emoji.text = wallpapers[i];
    emoji.font = [OSFTypography displaySmall];
    [thumb addSubview:emoji];

    [wallpaperGrid_ addSubview:thumb];
  }

  // Accent color section
  OSFLabel *accentLabel = [[OSFLabel alloc] init];
  accentLabel.frame = CGRectMake(50, 380, 200, 24);
  accentLabel.text = @"Accent Color";
  accentLabel.font = [OSFTypography labelLarge];
  accentLabel.textColor = [OSFColors textPrimary];
  [contentView_ addSubview:accentLabel];

  // Color picker (row of colored circles)
  accentColorPicker_ = [[OSFView alloc] init];
  accentColorPicker_.frame = CGRectMake(50, 410, 400, 40);
  [contentView_ addSubview:accentColorPicker_];

  // Colors: Coral (default), Teal, Blue, Purple, Green, Orange
  NSArray *colors = @[ @"🔴", @"🔵", @"🟣", @"🟢", @"🟠", @"🟡" ];
  for (int i = 0; i < 6; i++) {
    OSFLabel *colorDot = [[OSFLabel alloc] init];
    colorDot.frame = CGRectMake(i * 50, 0, 40, 40);
    colorDot.text = colors[i];
    colorDot.font = [OSFTypography headlineMedium];
    [accentColorPicker_ addSubview:colorDot];
  }
}

OSFView *CustomizePage::view() { return contentView_; }

bool CustomizePage::validate() {
  return true; // Always valid
}

void CustomizePage::saveConfig(InstallConfig &config) {
  config.darkMode = darkModeSelected_;
  config.wallpaper =
      selectedWallpaper_.empty() ? "default" : selectedWallpaper_;
  config.accentColor = selectedAccent_;
}

} // namespace vitusos::installer
