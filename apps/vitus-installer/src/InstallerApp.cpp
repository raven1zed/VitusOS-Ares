/**
 * InstallerApp.cpp - Main installer application
 */

#include "Installer.h"
#include <iostream>

namespace vitusos::installer {

InstallerApp::InstallerApp() {}

InstallerApp::~InstallerApp() {}

int InstallerApp::run(int argc, char **argv) {
  std::cout << "[VitusOS Installer] Starting..." << std::endl;
  std::cout << "[VitusOS Installer] Built with openSEF "
            << OPENSEF_VERSION_STRING << std::endl;

  // Connect to Wayland
  OSFBackend *backend = [OSFBackend sharedBackend];
  if (![backend connect]) {
    std::cerr << "[VitusOS Installer] Failed to connect to display"
              << std::endl;
    return 1;
  }

  // Apply light theme (installer uses light mode by default)
  [OSFTheme currentTheme].mode = OSFThemeModeLight;
  [[OSFTheme currentTheme] apply];

  // Create and show window
  window_ = std::make_unique<InstallerWindow>();
  window_->show();

  std::cout << "[VitusOS Installer] Ready" << std::endl;

  // Run event loop
  [backend run];

  [backend disconnect];
  return 0;
}

// ============================================================================
// InstallerWindow
// ============================================================================

InstallerWindow::InstallerWindow() {
  setupUI();
  setupPages();
  navigateToPage(0);
}

InstallerWindow::~InstallerWindow() {}

void InstallerWindow::setupUI() {
  // Create fullscreen window
  CGRect screenBounds = CGRectMake(0, 0, 1024, 768); // Will be fullscreen
  window_ = [OSFWindow windowWithTitle:@"VitusOS Installer" frame:screenBounds];

  // Glass background covering entire screen
  background_ = [OSFGlassPanel glassPanelWithFrame:screenBounds];
  background_.blurRadius = [OSFStyle blurRadiusHeavy]; // 24pt blur
  background_.tintColor = [OSFColors surface];
  [window_ setContentView:background_];

  // Centered content container (600x500, centered)
  float contentWidth = 600;
  float contentHeight = 500;
  float contentX = (screenBounds.size.width - contentWidth) / 2;
  float contentY = (screenBounds.size.height - contentHeight) / 2 - 30;

  pageContainer_ = [[OSFView alloc] init];
  pageContainer_.frame =
      CGRectMake(contentX, contentY, contentWidth, contentHeight);
  [background_ addSubview:pageContainer_];

  // Step indicator (5 dots at bottom)
  float indicatorY = contentY + contentHeight + 30;
  stepIndicator_ = [[OSFView alloc] init];
  stepIndicator_.frame = CGRectMake(contentX, indicatorY, contentWidth, 20);
  [background_ addSubview:stepIndicator_];

  // Navigation buttons
  float buttonY = indicatorY + 40;

  backButton_ = [OSFButton buttonWithLabel:@"Back"
                                    action:^{
                                      onBack();
                                    }];
  backButton_.frame = CGRectMake(contentX, buttonY, 100, 44);
  [background_ addSubview:backButton_];

  continueButton_ = [OSFButton buttonWithLabel:@"Continue"
                                        action:^{
                                          onContinue();
                                        }];
  continueButton_.frame =
      CGRectMake(contentX + contentWidth - 140, buttonY, 140, 44);
  [background_ addSubview:continueButton_];
}

void InstallerWindow::setupPages() {
  pages_.push_back(std::make_unique<WelcomePage>());
  pages_.push_back(std::make_unique<DiskPage>());
  pages_.push_back(std::make_unique<AccountPage>());
  pages_.push_back(std::make_unique<CustomizePage>());
  pages_.push_back(std::make_unique<InstallPage>());
}

void InstallerWindow::navigateToPage(int index) {
  if (index < 0 || index >= static_cast<int>(pages_.size()))
    return;

  // Remove current page view
  for (OSFView *subview in pageContainer_.subviews) {
    [subview removeFromSuperview];
  }

  // Add new page view
  currentPageIndex_ = index;
  OSFView *pageView = pages_[index]->view();
  pageView.frame = CGRectMake(0, 0, 600, 500);
  [pageContainer_ addSubview:pageView];

  // Update step indicator
  updateStepIndicator();

  // Update button states
  backButton_.hidden = (index == 0);

  if (index == static_cast<int>(pages_.size()) - 1) {
    continueButton_.label = @"Install";
  } else {
    continueButton_.label = @"Continue";
  }
}

void InstallerWindow::onBack() {
  if (currentPageIndex_ > 0) {
    navigateToPage(currentPageIndex_ - 1);
  }
}

void InstallerWindow::onContinue() {
  InstallerPage *currentPage = pages_[currentPageIndex_].get();

  // Validate current page
  if (!currentPage->validate()) {
    // Show validation error (TODO: toast notification)
    return;
  }

  // Save config
  currentPage->saveConfig(config_);

  // Move to next page or start install
  if (currentPageIndex_ < static_cast<int>(pages_.size()) - 1) {
    navigateToPage(currentPageIndex_ + 1);
  } else {
    // Start installation
    auto *installPage = static_cast<InstallPage *>(pages_.back().get());
    installPage->startInstallation(config_);
  }
}

void InstallerWindow::updateStepIndicator() {
  // Clear existing indicators
  for (OSFView *subview in stepIndicator_.subviews) {
    [subview removeFromSuperview];
  }

  // Create 5 dots centered
  float dotSize = 8;
  float dotSpacing = 16;
  float totalWidth =
      (pages_.size() * dotSize) + ((pages_.size() - 1) * dotSpacing);
  float startX = (600 - totalWidth) / 2;

  for (size_t i = 0; i < pages_.size(); i++) {
    OSFLabel *dot = [[OSFLabel alloc] init];
    dot.frame =
        CGRectMake(startX + i * (dotSize + dotSpacing), 6, dotSize, dotSize);

    if (static_cast<int>(i) == currentPageIndex_) {
      dot.text = @"●"; // Filled
      dot.textColor = [OSFColors primary];
    } else if (static_cast<int>(i) < currentPageIndex_) {
      dot.text = @"●"; // Filled (completed)
      dot.textColor = [OSFColors textSecondary];
    } else {
      dot.text = @"○"; // Empty (future)
      dot.textColor = [OSFColors textDisabled];
    }

    [stepIndicator_ addSubview:dot];
  }
}

void InstallerWindow::show() { [window_ show]; }

} // namespace vitusos::installer
