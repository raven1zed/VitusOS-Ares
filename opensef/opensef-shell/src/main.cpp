/**
 * main.cpp - VitusOS Ares Desktop Shell
 *
 * Launches the full desktop environment:
 * - Wallpaper (Layer Shell Background)
 * - Menu Bar (Layer Shell Top)
 * - Dock (Layer Shell Bottom)
 */

#include <iostream>
#include <memory>
#include <opensef/OSFWindow.h>
#include <opensef/OpenSEFBase.h>
#include <thread>
#include <vector>


// Shell Components
#include "dock/OSFDock.h"
#include "panel/OSFPanel.h"
#include "wallpaper/OSFWallpaper.h"

using namespace opensef;

int main(int argc, char **argv) {
  std::cout << "Starting VitusOS Ares Desktop..." << std::endl;

  // 1. Wallpaper
  auto wallpaper = std::make_shared<OSFWallpaper>("resources/wallpaper.png");

  // 2. Panel (Top Bar)
  auto panel = std::make_shared<OSFPanel>();

  // 3. Dock (Bottom Bar)
  auto dock = std::make_shared<OSFDock>();

  // Run each surface in its own thread
  // Each surface has a working run() method that processes events correctly
  std::thread wallpaperThread([wallpaper]() {
    std::cout << "Wallpaper thread starting..." << std::endl;
    wallpaper->run();
  });

  std::thread panelThread([panel]() {
    std::cout << "Panel thread starting..." << std::endl;
    panel->run();
  });

  std::thread dockThread([dock]() {
    std::cout << "Dock thread starting..." << std::endl;
    dock->run();
  });

  std::cout << "All surfaces running in separate threads" << std::endl;

  // Wait for all threads
  wallpaperThread.join();
  panelThread.join();
  dockThread.join();

  return 0;
}
