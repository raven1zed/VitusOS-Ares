/**
 * InstallPage.cpp - Installation progress
 *
 * Layout:
 * ┌────────────────────────────────────────┐
 * │                                        │
 * │         Installing VitusOS             │
 * │                                        │
 * │    ████████████████░░░░░░░░░░░░░░     │  ← Progress bar
 * │                45%                     │
 * │                                        │
 * │    Copying system files...             │  ← Current task
 * │                                        │
 * │    ● Formatting disk                ✓  │
 * │    ● Copying files                  ⟳  │
 * │    ○ Configuring system                │
 * │    ○ Creating user account             │
 * │    ○ Finishing up                      │
 * │                                        │
 * └────────────────────────────────────────┘
 */

#include "Installer.h"
#include <chrono>
#include <iostream>
#include <thread>


namespace vitusos::installer {

InstallPage::InstallPage() : InstallerPage("Install") {
  contentView_ = [[OSFView alloc] init];

  // Title
  statusLabel_ = [[OSFLabel alloc] init];
  statusLabel_.frame = CGRectMake(0, 80, 600, 50);
  statusLabel_.text = @"Installing VitusOS";
  statusLabel_.font = [OSFTypography headlineMedium];
  statusLabel_.textColor = [OSFColors textPrimary];
  [contentView_ addSubview:statusLabel_];

  // Progress bar background
  OSFView *progressBg = [[OSFView alloc] init];
  progressBg.frame = CGRectMake(50, 160, 500, 8);
  // TODO: Draw rounded rect with secondary color
  [contentView_ addSubview:progressBg];

  // Progress bar fill (will be animated)
  OSFView *progressFill = [[OSFView alloc] init];
  progressFill.frame = CGRectMake(50, 160, 0, 8); // Starts at 0 width
  // TODO: Draw rounded rect with primary color
  [contentView_ addSubview:progressFill];

  // Percentage label
  OSFLabel *percentLabel = [[OSFLabel alloc] init];
  percentLabel.frame = CGRectMake(0, 180, 600, 30);
  percentLabel.text = @"0%";
  percentLabel.font = [OSFTypography bodyMedium];
  percentLabel.textColor = [OSFColors textSecondary];
  [contentView_ addSubview:percentLabel];

  // Current task detail
  detailLabel_ = [[OSFLabel alloc] init];
  detailLabel_.frame = CGRectMake(0, 220, 600, 24);
  detailLabel_.text = @"Preparing installation...";
  detailLabel_.font = [OSFTypography bodyMedium];
  detailLabel_.textColor = [OSFColors textSecondary];
  [contentView_ addSubview:detailLabel_];

  // Step list
  NSArray *steps = @[
    @"○ Formatting disk", @"○ Copying system files", @"○ Configuring NixOS",
    @"○ Creating user account", @"○ Setting up desktop"
  ];

  for (int i = 0; i < 5; i++) {
    OSFLabel *step = [[OSFLabel alloc] init];
    step.frame = CGRectMake(100, 280 + i * 30, 400, 24);
    step.text = steps[i];
    step.font = [OSFTypography bodyMedium];
    step.textColor = [OSFColors textSecondary];
    [contentView_ addSubview:step];
  }
}

OSFView *InstallPage::view() { return contentView_; }

bool InstallPage::validate() { return complete_; }

void InstallPage::saveConfig(InstallConfig &config) {
  // Nothing to save - installation handles it
}

void InstallPage::startInstallation(const InstallConfig &config) {
  std::cout << "[Installer] Starting installation..." << std::endl;
  std::cout << "[Installer] Target disk: " << config.targetDisk << std::endl;
  std::cout << "[Installer] Username: " << config.username << std::endl;
  std::cout << "[Installer] Dark mode: " << (config.darkMode ? "yes" : "no")
            << std::endl;

  // Simulate installation steps
  // In real implementation, this would run nixos-install commands

  updateProgress(0.1f, "Formatting disk...");
  // TODO: Run partitioning commands

  updateProgress(0.3f, "Copying system files...");
  // TODO: Run nixos-install

  updateProgress(0.6f, "Configuring NixOS...");
  // TODO: Generate configuration.nix

  updateProgress(0.8f, "Creating user account...");
  // TODO: Set up user

  updateProgress(0.95f, "Setting up desktop...");
  // TODO: Configure openSEF desktop

  updateProgress(1.0f, "Installation complete!");
  complete_ = true;

  // Update UI to show completion
  statusLabel_.text = @"Installation Complete!";
  detailLabel_.text = @"Please restart your computer to begin using VitusOS.";

  if (onComplete) {
    onComplete();
  }
}

void InstallPage::updateProgress(float progress, const std::string &status) {
  progress_ = progress;
  detailLabel_.text = [NSString stringWithUTF8String:status.c_str()];

  // TODO: Animate progress bar width
  // progressFill.frame.size.width = 500 * progress;

  std::cout << "[Installer] " << (int)(progress * 100) << "% - " << status
            << std::endl;
}

} // namespace vitusos::installer
