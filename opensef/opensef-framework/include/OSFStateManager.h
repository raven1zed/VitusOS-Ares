#pragma once

#include <memory>
#include <string>
#include <vector>

namespace OpenSEF {

// Forward declarations
class OSFWindow;
class OSFApplication;
class OSFWorkspace;

/**
 * OSFStateManager - Centralized State Management
 *
 * Single source of truth for all desktop state.
 * All components query state from here, never maintain their own copies.
 *
 * Usage:
 *   auto* state = desktop->stateManager();
 *   auto* activeWin = state->activeWindow();
 *   auto allWins = state->allWindows();
 */
class OSFStateManager {
public:
  OSFStateManager();
  ~OSFStateManager();

  // Window state
  std::vector<OSFWindow *> allWindows();
  OSFWindow *activeWindow();
  OSFWindow *windowById(const std::string &id);

  // Application state
  std::vector<OSFApplication *> runningApps();
  OSFApplication *appById(const std::string &id);

  // Desktop state
  int currentWorkspace();
  std::vector<OSFWorkspace *> allWorkspaces();

  // State updates (called by framework internals)
  void setActiveWindow(OSFWindow *window);
  void addWindow(OSFWindow *window);
  void removeWindow(const std::string &id);

  void addApplication(OSFApplication *app);
  void removeApplication(const std::string &id);

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

/**
 * OSFWindow - Window representation
 */
class OSFWindow {
public:
  OSFWindow(const std::string &id, const std::string &title,
            const std::string &appId);

  std::string id() const { return id_; }
  std::string title() const { return title_; }
  std::string appId() const { return appId_; }

  void setTitle(const std::string &title) { title_ = title; }

  bool isMinimized() const { return minimized_; }
  bool isMaximized() const { return maximized_; }
  bool isFocused() const { return focused_; }

  void setMinimized(bool minimized) { minimized_ = minimized; }
  void setMaximized(bool maximized) { maximized_ = maximized; }
  void setFocused(bool focused) { focused_ = focused; }

private:
  std::string id_;
  std::string title_;
  std::string appId_;
  bool minimized_ = false;
  bool maximized_ = false;
  bool focused_ = false;
};

/**
 * OSFApplication - Application representation
 */
class OSFApplication {
public:
  OSFApplication(const std::string &id, const std::string &name);

  std::string id() const { return id_; }
  std::string name() const { return name_; }

private:
  std::string id_;
  std::string name_;
};

/**
 * OSFWorkspace - Workspace representation
 */
class OSFWorkspace {
public:
  OSFWorkspace(int id, const std::string &name);

  int id() const { return id_; }
  std::string name() const { return name_; }

private:
  int id_;
  std::string name_;
};

} // namespace OpenSEF
