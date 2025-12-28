/**
 * DiskPage.cpp - Visual disk selection
 *
 * Layout:
 * ┌────────────────────────────────────────┐
 * │         Select a disk                  │
 * │                                        │
 * │  ┌──────────────────────────────────┐  │
 * │  │ █████████████████████░░░░░░░░░░░ │  │  ← Visual disk usage
 * │  │ 256GB / 512GB used               │  │
 * │  └──────────────────────────────────┘  │
 * │                                        │
 * │  ○ NVME0 - Samsung 970 EVO (512GB)    │
 * │  ● NVME1 - WD Black (1TB)             │
 * │  ○ SDA - USB Drive (32GB)             │
 * │                                        │
 * │  ☑ Erase disk and install VitusOS     │
 * │  ☐ Encrypt disk (FileVault)           │
 * │                                        │
 * └────────────────────────────────────────┘
 */

#include "Installer.h"

namespace vitusos::installer {

DiskPage::DiskPage() : InstallerPage("Select Disk") {
  contentView_ = [[OSFView alloc] init];

  // Title
  OSFLabel *title = [[OSFLabel alloc] init];
  title.frame = CGRectMake(0, 20, 600, 40);
  title.text = @"Select a disk";
  title.font = [OSFTypography headlineMedium];
  title.textColor = [OSFColors textPrimary];
  [contentView_ addSubview:title];

  // Disk visualization (bar graph)
  diskVisualization_ = [[OSFView alloc] init];
  diskVisualization_.frame = CGRectMake(50, 80, 500, 60);
  // TODO: Draw rounded rect with usage visualization
  [contentView_ addSubview:diskVisualization_];

  // Usage label
  OSFLabel *usageLabel = [[OSFLabel alloc] init];
  usageLabel.frame = CGRectMake(50, 150, 500, 20);
  usageLabel.text = @"512GB available";
  usageLabel.font = [OSFTypography bodySmall];
  usageLabel.textColor = [OSFColors textSecondary];
  [contentView_ addSubview:usageLabel];

  // Disk list (simulated for now)
  diskList_ = [[OSFTableView alloc] init];
  diskList_.frame = CGRectMake(50, 190, 500, 150);
  diskList_.items = @[
    @"NVMe0 - Samsung 970 EVO (512GB)",
    @"NVMe1 - WD Black SN750 (1TB)",
  ];
  diskList_.onSelection = ^(NSInteger index) {
    selectedDiskIndex_ = static_cast<int>(index);
  };
  [contentView_ addSubview:diskList_];

  // Erase option
  eraseLabel_ = [[OSFLabel alloc] init];
  eraseLabel_.frame = CGRectMake(70, 360, 400, 24);
  eraseLabel_.text = @"☑ Erase disk and install VitusOS";
  eraseLabel_.font = [OSFTypography bodyMedium];
  eraseLabel_.textColor = [OSFColors textPrimary];
  [contentView_ addSubview:eraseLabel_];

  // Encrypt option
  encryptLabel_ = [[OSFLabel alloc] init];
  encryptLabel_.frame = CGRectMake(70, 390, 400, 24);
  encryptLabel_.text = @"☐ Encrypt disk (recommended)";
  encryptLabel_.font = [OSFTypography bodyMedium];
  encryptLabel_.textColor = [OSFColors textPrimary];
  [contentView_ addSubview:encryptLabel_];

  // Warning
  OSFLabel *warning = [[OSFLabel alloc] init];
  warning.frame = CGRectMake(50, 440, 500, 40);
  warning.text = @"⚠️ This will erase all data on the selected disk.";
  warning.font = [OSFTypography bodySmall];
  warning.textColor = [OSFColors warning];
  [contentView_ addSubview:warning];
}

OSFView *DiskPage::view() { return contentView_; }

bool DiskPage::validate() { return selectedDiskIndex_ >= 0; }

void DiskPage::saveConfig(InstallConfig &config) {
  if (selectedDiskIndex_ >= 0 &&
      selectedDiskIndex_ < static_cast<int>(disks_.size())) {
    config.targetDisk = disks_[selectedDiskIndex_].id;
  } else {
    config.targetDisk = "/dev/nvme0n1"; // Default
  }
  config.eraseAll = true;
  config.encrypt = false;
}

void DiskPage::refreshDisks() {
  // TODO: Scan /dev/ for available disks
  disks_.clear();
  disks_.push_back({"nvme0n1", "NVMe0", "Samsung 970 EVO",
                    512ULL * 1024 * 1024 * 1024, false});
}

} // namespace vitusos::installer
