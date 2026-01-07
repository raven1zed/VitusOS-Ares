/**
 * OSFAuth.cpp - PAM Authentication Implementation
 */

#include "OSFAuth.h"
#include <cstring>
#include <iostream>
#include <pwd.h>
#include <security/pam_appl.h>
#include <unistd.h>


namespace opensef {

// PAM conversation function
static int pam_conv_func(int num_msg, const struct pam_message **msg,
                         struct pam_response **resp, void *appdata_ptr) {
  const char *password = static_cast<const char *>(appdata_ptr);

  *resp = (struct pam_response *)calloc(num_msg, sizeof(struct pam_response));
  if (*resp == nullptr) {
    return PAM_BUF_ERR;
  }

  for (int i = 0; i < num_msg; i++) {
    if (msg[i]->msg_style == PAM_PROMPT_ECHO_OFF) {
      // Password prompt
      (*resp)[i].resp = strdup(password);
      (*resp)[i].resp_retcode = 0;
    } else {
      (*resp)[i].resp = nullptr;
      (*resp)[i].resp_retcode = 0;
    }
  }

  return PAM_SUCCESS;
}

bool OSFAuth::authenticate(const std::string &username,
                           const std::string &password) {
  pam_handle_t *pamh = nullptr;

  struct pam_conv conv = {pam_conv_func, const_cast<char *>(password.c_str())};

  // Start PAM session
  int ret = pam_start("login", username.c_str(), &conv, &pamh);
  if (ret != PAM_SUCCESS) {
    std::cerr << "PAM start failed: " << pam_strerror(pamh, ret) << std::endl;
    return false;
  }

  // Authenticate
  ret = pam_authenticate(pamh, 0);
  if (ret != PAM_SUCCESS) {
    std::cerr << "PAM authentication failed: " << pam_strerror(pamh, ret)
              << std::endl;
    pam_end(pamh, ret);
    return false;
  }

  // Validate account
  ret = pam_acct_mgmt(pamh, 0);
  if (ret != PAM_SUCCESS) {
    std::cerr << "PAM account validation failed: " << pam_strerror(pamh, ret)
              << std::endl;
    pam_end(pamh, ret);
    return false;
  }

  // End PAM session
  pam_end(pamh, PAM_SUCCESS);
  return true;
}

std::string OSFAuth::getCurrentUser() {
  uid_t uid = getuid();
  struct passwd *pw = getpwuid(uid);
  if (pw) {
    return std::string(pw->pw_name);
  }
  return "";
}

std::string OSFAuth::getUserAvatar(const std::string &username) {
  // Check common avatar locations
  std::string paths[] = {"/var/lib/AccountsService/icons/" + username,
                         "/home/" + username + "/.face",
                         "/home/" + username + "/.face.icon"};

  for (const auto &path : paths) {
    if (access(path.c_str(), F_OK) == 0) {
      return path;
    }
  }

  return "";
}

std::string OSFAuth::getUserFullName(const std::string &username) {
  struct passwd *pw = getpwnam(username.c_str());
  if (pw && pw->pw_gecos) {
    std::string gecos(pw->pw_gecos);
    // GECOS field format: "Full Name,Room,Work Phone,Home Phone"
    size_t comma = gecos.find(',');
    if (comma != std::string::npos) {
      return gecos.substr(0, comma);
    }
    return gecos;
  }
  return username;
}

} // namespace opensef
