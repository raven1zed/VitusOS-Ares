/**
 * OSFAuth.h - Authentication API for VitusOS
 *
 * Provides PAM-based authentication for lock screen and login.
 */

#pragma once

#include <string>

namespace opensef {

class OSFAuth {
public:
  /**
   * Authenticate a user with username and password.
   * Returns true if authentication succeeds.
   */
  static bool authenticate(const std::string &username,
                           const std::string &password);

  /**
   * Get the current logged-in user.
   * Returns username or empty string if not available.
   */
  static std::string getCurrentUser();

  /**
   * Get the avatar path for a user.
   * Returns path to avatar image or empty string if not found.
   */
  static std::string getUserAvatar(const std::string &username);

  /**
   * Get the full name of a user.
   * Returns display name or username if not available.
   */
  static std::string getUserFullName(const std::string &username);
};

} // namespace opensef
