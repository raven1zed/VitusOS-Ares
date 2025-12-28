/**
 * VitusOS Installer - Modern Minimalist GUI
 *
 * macOS-style installation experience built with openSEF.
 * Features:
 * - Glass/blur backgrounds
 * - Smooth animations
 * - Step-by-step wizard
 * - Visual disk partitioning
 * - Live wallpaper preview
 */

#pragma once

#include <functional>
#include <memory>
#include <opensef/opensef.h>
#include <string>
#include <vector>


namespace vitusos::installer {

// ============================================================================
// Installation Configuration
// ============================================================================

struct InstallConfig {
  // Step 1: Welcome
  std::string language = "en_US";
  std::string region = "US";
  std::string keyboard = "us";

  // Step 2: Disk
  std::string targetDisk;
  bool eraseAll = true;
  bool encrypt = false;
  std::string encryptPassword;

  // Step 3: Account
  std::string fullName;
  std::string username;
  std::string password;
  std::string avatarPath;

  // Step 4: Customize
  bool darkMode = false;
  std::string wallpaper = "default";
  std::string accentColor = "coral";
};

// ============================================================================
// InstallerPage - Base class for wizard pages
// ============================================================================

class InstallerPage {
public:
  InstallerPage(const std::string &title);
  virtual ~InstallerPage() = default;

  virtual OSFView *view() = 0;
  virtual bool validate() = 0;
  virtual void saveConfig(InstallConfig &config) = 0;

  std::string title() const { return title_; }

protected:
  std::string title_;
  OSFView *contentView_ = nullptr;
};

// ============================================================================
// WelcomePage - Language and region selection
// ============================================================================

class WelcomePage : public InstallerPage {
public:
  WelcomePage();

  OSFView *view() override;
  bool validate() override;
  void saveConfig(InstallConfig &config) override;

private:
  // Large "Welcome to VitusOS" title
  OSFLabel *titleLabel_;
  OSFLabel *subtitleLabel_;

  // Language dropdown
  OSFLabel *languageLabel_;
  // TODO: OSFDropdown* languageDropdown_;

  // Region dropdown
  OSFLabel *regionLabel_;
  // TODO: OSFDropdown* regionDropdown_;
};

// ============================================================================
// DiskPage - Visual disk partitioning
// ============================================================================

struct DiskInfo {
  std::string id;
  std::string name;
  std::string model;
  uint64_t sizeBytes;
  bool hasOS;
};

class DiskPage : public InstallerPage {
public:
  DiskPage();

  OSFView *view() override;
  bool validate() override;
  void saveConfig(InstallConfig &config) override;

  void refreshDisks();

private:
  std::vector<DiskInfo> disks_;
  int selectedDiskIndex_ = -1;

  // Visual disk selector (bar graph style)
  OSFView *diskVisualization_;

  // Disk list
  OSFTableView *diskList_;

  // Options
  OSFLabel *eraseLabel_;
  // TODO: OSFSwitch* eraseSwitch_;

  OSFLabel *encryptLabel_;
  // TODO: OSFSwitch* encryptSwitch_;
};

// ============================================================================
// AccountPage - User account setup
// ============================================================================

class AccountPage : public InstallerPage {
public:
  AccountPage();

  OSFView *view() override;
  bool validate() override;
  void saveConfig(InstallConfig &config) override;

private:
  // Avatar (circular image or initials)
  OSFView *avatarView_;

  // Form fields
  OSFLabel *fullNameLabel_;
  OSFTextField *fullNameField_;

  OSFLabel *usernameLabel_;
  OSFTextField *usernameField_;

  OSFLabel *passwordLabel_;
  OSFTextField *passwordField_;

  OSFLabel *confirmLabel_;
  OSFTextField *confirmField_;
};

// ============================================================================
// CustomizePage - Theme and appearance
// ============================================================================

class CustomizePage : public InstallerPage {
public:
  CustomizePage();

  OSFView *view() override;
  bool validate() override;
  void saveConfig(InstallConfig &config) override;

private:
  // Theme toggle (Light/Dark)
  OSFView *themePreviewLight_;
  OSFView *themePreviewDark_;
  bool darkModeSelected_ = false;

  // Wallpaper grid
  OSFView *wallpaperGrid_;
  std::string selectedWallpaper_;

  // Accent color picker
  OSFView *accentColorPicker_;
  std::string selectedAccent_ = "coral";
};

// ============================================================================
// InstallPage - Progress and completion
// ============================================================================

class InstallPage : public InstallerPage {
public:
  InstallPage();

  OSFView *view() override;
  bool validate() override;
  void saveConfig(InstallConfig &config) override;

  void startInstallation(const InstallConfig &config);

  std::function<void()> onComplete;

private:
  // Progress UI
  OSFLabel *statusLabel_;
  // TODO: OSFProgressBar* progressBar_;
  OSFLabel *detailLabel_;

  float progress_ = 0.0f;
  bool complete_ = false;

  void updateProgress(float progress, const std::string &status);
};

// ============================================================================
// InstallerWindow - Main wizard window
// ============================================================================

class InstallerWindow {
public:
  InstallerWindow();
  ~InstallerWindow();

  void show();

private:
  OSFWindow *window_;
  OSFGlassPanel *background_;

  // Page container
  OSFView *pageContainer_;

  // Navigation
  OSFButton *backButton_;
  OSFButton *continueButton_;
  OSFView *stepIndicator_;

  // Pages
  std::vector<std::unique_ptr<InstallerPage>> pages_;
  int currentPageIndex_ = 0;

  // Config
  InstallConfig config_;

  void setupUI();
  void setupPages();
  void navigateToPage(int index);
  void onBack();
  void onContinue();
  void updateStepIndicator();
};

// ============================================================================
// InstallerApp - Main application
// ============================================================================

class InstallerApp {
public:
  InstallerApp();
  ~InstallerApp();

  int run(int argc, char **argv);

private:
  std::unique_ptr<InstallerWindow> window_;
};

} // namespace vitusos::installer
