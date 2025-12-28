/**
 * AccountPage.cpp - User account setup
 *
 * Layout:
 * ┌────────────────────────────────────────┐
 * │         Create your account            │
 * │                                        │
 * │              ┌─────┐                   │
 * │              │ AB  │                   │  ← Avatar (initials)
 * │              └─────┘                   │
 * │                                        │
 * │    Full Name     [________________]    │
 * │    Username      [________________]    │
 * │    Password      [________________]    │
 * │    Confirm       [________________]    │
 * │                                        │
 * └────────────────────────────────────────┘
 */

#include "Installer.h"

namespace vitusos::installer {

AccountPage::AccountPage() : InstallerPage("Create Account") {
  contentView_ = [[OSFView alloc] init];

  // Title
  OSFLabel *title = [[OSFLabel alloc] init];
  title.frame = CGRectMake(0, 20, 600, 40);
  title.text = @"Create your account";
  title.font = [OSFTypography headlineMedium];
  title.textColor = [OSFColors textPrimary];
  [contentView_ addSubview:title];

  // Avatar placeholder (circular with initials)
  avatarView_ = [[OSFView alloc] init];
  avatarView_.frame = CGRectMake(250, 80, 100, 100);
  // TODO: Draw circular avatar with gradient background
  [contentView_ addSubview:avatarView_];

  OSFLabel *avatarLabel = [[OSFLabel alloc] init];
  avatarLabel.frame = CGRectMake(250, 80, 100, 100);
  avatarLabel.text = @"?";
  avatarLabel.font = [OSFTypography displaySmall];
  avatarLabel.textColor = [OSFColors background];
  [avatarView_ addSubview:avatarLabel];

  // Form fields
  float fieldX = 150;
  float labelX = 50;
  float fieldWidth = 300;
  float rowHeight = 50;
  float startY = 200;

  // Full Name
  fullNameLabel_ = [[OSFLabel alloc] init];
  fullNameLabel_.frame = CGRectMake(labelX, startY, 100, 30);
  fullNameLabel_.text = @"Full Name";
  fullNameLabel_.font = [OSFTypography labelLarge];
  fullNameLabel_.textColor = [OSFColors textPrimary];
  [contentView_ addSubview:fullNameLabel_];

  fullNameField_ = [[OSFTextField alloc] init];
  fullNameField_.frame = CGRectMake(fieldX, startY, fieldWidth, 36);
  fullNameField_.placeholder = @"John Doe";
  [contentView_ addSubview:fullNameField_];

  // Username
  usernameLabel_ = [[OSFLabel alloc] init];
  usernameLabel_.frame = CGRectMake(labelX, startY + rowHeight, 100, 30);
  usernameLabel_.text = @"Username";
  usernameLabel_.font = [OSFTypography labelLarge];
  usernameLabel_.textColor = [OSFColors textPrimary];
  [contentView_ addSubview:usernameLabel_];

  usernameField_ = [[OSFTextField alloc] init];
  usernameField_.frame = CGRectMake(fieldX, startY + rowHeight, fieldWidth, 36);
  usernameField_.placeholder = @"johndoe";
  [contentView_ addSubview:usernameField_];

  // Password
  passwordLabel_ = [[OSFLabel alloc] init];
  passwordLabel_.frame = CGRectMake(labelX, startY + rowHeight * 2, 100, 30);
  passwordLabel_.text = @"Password";
  passwordLabel_.font = [OSFTypography labelLarge];
  passwordLabel_.textColor = [OSFColors textPrimary];
  [contentView_ addSubview:passwordLabel_];

  passwordField_ = [[OSFTextField alloc] init];
  passwordField_.frame =
      CGRectMake(fieldX, startY + rowHeight * 2, fieldWidth, 36);
  passwordField_.placeholder = @"••••••••";
  // TODO: passwordField_.secureTextEntry = YES;
  [contentView_ addSubview:passwordField_];

  // Confirm Password
  confirmLabel_ = [[OSFLabel alloc] init];
  confirmLabel_.frame = CGRectMake(labelX, startY + rowHeight * 3, 100, 30);
  confirmLabel_.text = @"Confirm";
  confirmLabel_.font = [OSFTypography labelLarge];
  confirmLabel_.textColor = [OSFColors textPrimary];
  [contentView_ addSubview:confirmLabel_];

  confirmField_ = [[OSFTextField alloc] init];
  confirmField_.frame =
      CGRectMake(fieldX, startY + rowHeight * 3, fieldWidth, 36);
  confirmField_.placeholder = @"••••••••";
  [contentView_ addSubview:confirmField_];
}

OSFView *AccountPage::view() { return contentView_; }

bool AccountPage::validate() {
  // Check all fields are filled
  NSString *fullName = fullNameField_.text;
  NSString *username = usernameField_.text;
  NSString *password = passwordField_.text;
  NSString *confirm = confirmField_.text;

  if (fullName.length == 0 || username.length == 0 || password.length == 0) {
    return false;
  }

  // Check passwords match
  if (![password isEqualToString:confirm]) {
    return false;
  }

  return true;
}

void AccountPage::saveConfig(InstallConfig &config) {
  config.fullName = [fullNameField_.text UTF8String];
  config.username = [usernameField_.text UTF8String];
  config.password = [passwordField_.text UTF8String];
}

} // namespace vitusos::installer
