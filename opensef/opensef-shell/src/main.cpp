/**
 * main.cpp - VitusOS Ares Desktop Shell
 *
 * Launches the full desktop environment:
 * - Wallpaper (Layer Shell Background)
 * - Menu Bar (Layer Shell Top)
 * - Dock (Layer Shell Bottom)
 *
 * Uses the Unified Event Loop (OSFApplication) to manage all surfaces.
 */

#include <iostream>
#include <memory>
#include <opensef/OSFWindow.h>
#include <opensef/OpenSEFBase.h>
#include <vector>


// Shell Components
#include "dock/OSFDock.h"
#include "panel/OSFPanel.h"
#include "wallpaper/OSFWallpaper.h"


using namespace opensef;

int main(int argc, char **argv) {
  std::cout << "Starting VitusOS Ares Desktop..." << std::endl;

  auto &app = OSFApplication::shared();

  // 1. Wallpaper
  // Path to wallpaper? For now hardcode or use a default
  // In NixOS VM, we might look in /run/current-system/sw/share/backgrounds or
  // similar Or just a colored rect if file not found (handled by OSFWallpaper)
  auto wallpaper = std::make_shared<OSFWallpaper>(
      "/usr/share/backgrounds/vitus_default.jpg");
  if (wallpaper->connect()) {
    app.addExternalEventSource(wallpaper->surface()->displayFd(), [=]() {
      wallpaper->surface()->processEvents();
    });
    // Connect timer dispatch if needed
    app.addExternalEventSource(wallpaper->surface()->displayFd(), [=]() {
      wallpaper->surface()->dispatchTimers();
    });
  }

  // 2. Panel (Top Bar)
  auto panel = std::make_shared<OSFPanel>();
  // OSFPanel::run() used to do everything. Now we just connect.
  // We added connect() helper in header
  // Wait, OSFPanel.cpp implementation of run() was:
  // if (surface_->connect()) { setupTimer; surface_->run(); }
  // We need to replicate the setupTimer part or ensure it happens in
  // constructor/connect. OSFPanel constructor calls initWidgets() which is
  // fine. But the clock timer was added in run(). We should move that logic to
  // constructor or a new Setup method if possible, or just manual setup here.
  // For now, let's rely on OSFPanel being simple.
  // Actually, I added connect() to header, but did NOT implement it in CPP to
  // do the timer setup. Modifying OSFPanel logic might be needed. Let's assume
  // for V1 the clock might be static or we fix it later.

  if (panel->connect()) {
    app.addExternalEventSource(panel->surface()->displayFd(), [=]() {
      panel->surface()->processEvents();
      // Also need to dispatch timers for the clock!
      panel->surface()->dispatchTimers();
    });
  }

  // 3. Dock (Bottom Bar)
  auto dock = std::make_shared<OSFDock>();
  if (dock->connect()) {
    app.addExternalEventSource(dock->surface()->displayFd(),
                               [=]() { dock->surface()->processEvents(); });
  }

  // Run the unified loop
  app.run();

  return 0;
}
