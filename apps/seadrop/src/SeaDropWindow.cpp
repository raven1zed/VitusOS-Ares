/**
 * SeaDropWindow.cpp - Main window (openSEF native)
 */

#include "SeaDrop.h"

namespace vitusos::seadrop {

SeaDropWindow::SeaDropWindow() {
  setupUI();
  setupMenu();
}

SeaDropWindow::~SeaDropWindow() {
  // openSEF handles cleanup via ARC
}

void SeaDropWindow::setupUI() {
  // Create window with OS1 glass effect
  window_ =
      [OSFWindow windowWithTitle:@"SeaDrop" frame:CGRectMake(0, 0, 400, 600)];

  // Glass background panel
  backgroundPanel_ =
      [OSFGlassPanel glassPanelWithFrame:CGRectMake(0, 0, 400, 600)];
  backgroundPanel_.blurRadius = [OSFStyle blurRadiusMedium];
  [window_ setContentView:backgroundPanel_];

  // Search field at top
  searchField_ = [[OSFTextField alloc] init];
  searchField_.frame = CGRectMake(16, 16, 368, 36);
  searchField_.placeholder = @"Search clipboard history...";
  searchField_.onTextChanged = ^(NSString *text) {
    onSearchChanged([text UTF8String]);
  };
  [backgroundPanel_ addSubview:searchField_];

  // Clipboard history table
  historyTableView_ = [[OSFTableView alloc] init];
  historyTableView_.frame = CGRectMake(16, 68, 368, 480);
  historyTableView_.onSelection = ^(NSInteger index) {
    onHistoryItemSelected(static_cast<int>(index));
  };
  [backgroundPanel_ addSubview:historyTableView_];

  // Status bar at bottom
  statusLabel_ = [[OSFLabel alloc] init];
  statusLabel_.frame = CGRectMake(16, 564, 280, 24);
  statusLabel_.text = @"Connecting...";
  statusLabel_.font = [OSFTypography bodySmall];
  statusLabel_.textColor = [OSFColors textSecondary];
  [backgroundPanel_ addSubview:statusLabel_];

  // Sync button
  syncButton_ = [OSFButton buttonWithLabel:@"Sync"
                                    action:^{
                                      onSyncClicked();
                                    }];
  syncButton_.frame = CGRectMake(320, 560, 64, 32);
  [backgroundPanel_ addSubview:syncButton_];
}

void SeaDropWindow::setupMenu() {
  // File menu
  auto *fileMenu = [[OSFMenu alloc] initWithTitle:@"File"];

  auto *quitItem = [[OSFMenuItem alloc] initWithTitle:@"Quit SeaDrop"];
  quitItem.shortcut = @"Cmd+Q";
  quitItem.triggered = ^{
    // Quit application
  };
  [fileMenu addItem:quitItem];

  // Edit menu
  auto *editMenu = [[OSFMenu alloc] initWithTitle:@"Edit"];
  [editMenu addItem:[[OSFMenuItem alloc] initWithTitle:@"Copy"]];
  [editMenu addItem:[[OSFMenuItem alloc] initWithTitle:@"Paste"]];
  [editMenu addItem:[[OSFMenuItem alloc] initWithTitle:@"Clear History"]];

  // Register with global menu bar
  [[OSFMenuBar shared] addMenu:fileMenu];
  [[OSFMenuBar shared] addMenu:editMenu];
}

void SeaDropWindow::show() { [window_ show]; }

void SeaDropWindow::hide() { [window_ close]; }

bool SeaDropWindow::isVisible() const { return window_.visible; }

void SeaDropWindow::updateHistory(
    const std::vector<seadrop::ClipboardEntry> &entries) {
  NSMutableArray *items = [NSMutableArray array];
  for (const auto &entry : entries) {
    [items addObject:[NSString stringWithUTF8String:entry.preview.c_str()]];
  }
  historyTableView_.items = items;
  [historyTableView_ reloadData];
}

void SeaDropWindow::setStatus(const std::string &status, int deviceCount) {
  NSString *text;
  if (deviceCount > 0) {
    text = [NSString
        stringWithFormat:@"%s (%d devices)", status.c_str(), deviceCount];
  } else {
    text = [NSString stringWithUTF8String:status.c_str()];
  }
  statusLabel_.text = text;
}

void SeaDropWindow::setSearchQuery(const std::string &query) {
  searchField_.text = [NSString stringWithUTF8String:query.c_str()];
}

void SeaDropWindow::onHistoryItemSelected(int index) {
  // Copy selected item to system clipboard
  auto &entry = SeaDropApp::instance().backend().getHistory()[index];
  SeaDropApp::instance().backend().copyToClipboard(entry);
}

void SeaDropWindow::onSearchChanged(const std::string &query) {
  // Filter history
  // TODO: Implement filtering
}

void SeaDropWindow::onSyncClicked() {
  statusLabel_.text = @"Syncing...";
  SeaDropApp::instance().backend().requestSync();
}

} // namespace vitusos::seadrop
