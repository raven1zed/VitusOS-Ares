/**
 * SeaDropWindow.cpp - Main window (openSEF native)
 *
 * SeaDrop for VitusOS - Ares theme (Space Orange + Lunar Gray)
 */

#include "SeaDrop.h"
#include <iostream>

namespace vitusos::seadrop {

SeaDropWindow::SeaDropWindow() { setupUI(); }

SeaDropWindow::~SeaDropWindow() {
  // openSEF handles cleanup via ARC
}

void SeaDropWindow::setupUI() {
  // Create window with Ares glass effect
  window_ =
      [OSFWindow windowWithTitle:@"SeaDrop" frame:CGRectMake(0, 0, 400, 600)];

  // Glass background panel (Lunar Gray with blur)
  backgroundPanel_ =
      [OSFGlassPanel glassPanelWithFrame:CGRectMake(0, 0, 400, 600)];
  backgroundPanel_.blurRadius = [OSFStyle blurRadiusMedium];
  backgroundPanel_.tintColor = [OSFColors surface];
  [window_ setContentView:backgroundPanel_];

  // Header with app name
  OSFLabel *headerLabel = [[OSFLabel alloc] init];
  headerLabel.frame = CGRectMake(16, 16, 200, 32);
  headerLabel.text = @"SeaDrop";
  headerLabel.font = [OSFTypography headlineMedium];
  headerLabel.textColor = [OSFColors textPrimary];
  [backgroundPanel_ addSubview:headerLabel];

  // Connection indicator (Space Orange dot when connected)
  OSFLabel *connectionDot = [[OSFLabel alloc] init];
  connectionDot.frame = CGRectMake(360, 24, 20, 20);
  connectionDot.text = @"●";
  connectionDot.textColor = [OSFColors textDisabled]; // Gray when disconnected
  [backgroundPanel_ addSubview:connectionDot];

  // Search field
  searchField_ = [[OSFTextField alloc] init];
  searchField_.frame = CGRectMake(16, 56, 368, 40);
  searchField_.placeholder = @"Search clipboard history...";
  searchField_.onTextChanged = ^(NSString *text) {
    onSearchChanged([text UTF8String]);
  };
  [backgroundPanel_ addSubview:searchField_];

  // Clipboard history table
  historyTableView_ = [[OSFTableView alloc] init];
  historyTableView_.frame = CGRectMake(16, 112, 368, 428);
  historyTableView_.onSelection = ^(NSInteger index) {
    onHistoryItemSelected(static_cast<int>(index));
  };
  [backgroundPanel_ addSubview:historyTableView_];

  // Status bar at bottom
  statusLabel_ = [[OSFLabel alloc] init];
  statusLabel_.frame = CGRectMake(16, 556, 250, 24);
  statusLabel_.text = @"Starting...";
  statusLabel_.font = [OSFTypography bodySmall];
  statusLabel_.textColor = [OSFColors textSecondary];
  [backgroundPanel_ addSubview:statusLabel_];

  // Sync button (Space Orange accent)
  syncButton_ = [OSFButton buttonWithLabel:@"Sync"
                                    action:^{
                                      onSyncClicked();
                                    }];
  syncButton_.frame = CGRectMake(300, 550, 84, 36);
  // TODO: Set button color to primary (Space Orange)
  [backgroundPanel_ addSubview:syncButton_];
}

void SeaDropWindow::show() {
  [window_ show];
  std::cout << "[SeaDrop] Window shown" << std::endl;
}

void SeaDropWindow::hide() {
  [window_ close];
  std::cout << "[SeaDrop] Window hidden" << std::endl;
}

bool SeaDropWindow::isVisible() const { return window_.visible; }

void SeaDropWindow::updateHistory(const std::vector<ClipboardEntry> &entries) {
  NSMutableArray *items = [NSMutableArray array];
  for (const auto &entry : entries) {
    // Format: icon + preview text
    NSString *icon;
    if (entry.type == "image") {
      icon = @"🖼️";
    } else if (entry.type == "file") {
      icon = @"📄";
    } else {
      icon = @"📋";
    }

    NSString *preview = [NSString stringWithUTF8String:entry.preview.c_str()];
    NSString *item = [NSString stringWithFormat:@"%@ %@", icon, preview];
    [items addObject:item];
  }
  historyTableView_.items = items;
  [historyTableView_ reloadData];
}

void SeaDropWindow::setStatus(const std::string &status, int deviceCount) {
  NSString *text;
  if (deviceCount > 0) {
    text = [NSString stringWithFormat:@"%s • %d device%s", status.c_str(),
                                      deviceCount, deviceCount > 1 ? "s" : ""];
  } else {
    text = [NSString stringWithUTF8String:status.c_str()];
  }
  statusLabel_.text = text;
}

void SeaDropWindow::setSearchQuery(const std::string &query) {
  searchField_.text = [NSString stringWithUTF8String:query.c_str()];
}

void SeaDropWindow::onHistoryItemSelected(int index) {
  std::cout << "[SeaDrop] Selected item: " << index << std::endl;

#if HAVE_LIBSEADROP
  // Copy to system clipboard via libseadrop
  auto &entry = SeaDropApp::instance().backend().getHistory()[index];
  SeaDropApp::instance().backend().copyToClipboard(entry);
#else
  // UI-only mode - just log
  std::cout << "[SeaDrop] UI-only mode - clipboard not synced" << std::endl;
#endif
}

void SeaDropWindow::onSearchChanged(const std::string &query) {
  std::cout << "[SeaDrop] Search: " << query << std::endl;
  // TODO: Filter history by query
}

void SeaDropWindow::onSyncClicked() {
  std::cout << "[SeaDrop] Manual sync requested" << std::endl;
  statusLabel_.text = @"Syncing...";

#if HAVE_LIBSEADROP
  SeaDropApp::instance().backend().requestSync();
#else
  // UI-only mode
  statusLabel_.text = @"UI-only mode (no sync)";
#endif
}

} // namespace vitusos::seadrop
