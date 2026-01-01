/**
 * hello-window.cpp - Visual openSEF test
 *
 * Creates an actual window with Space Orange background!
 */

#include <iostream>
#include <memory>
#include <opensef/OSFWindowDecorations.h>
#include <opensef/OpenSEFAppKit.h>
#include <opensef/OpenSEFBackend.h>
#include <opensef/OpenSEFBase.h>

using namespace opensef;

int main() {
  std::cout << "╔════════════════════════════════════════════════════════╗"
            << std::endl;
  std::cout << "║       VitusOS Ares - openSEF Hello Window Test         ║"
            << std::endl;
  std::cout << "╚════════════════════════════════════════════════════════╝"
            << std::endl;
  std::cout << std::endl;

  // Connect to Wayland
  if (!OSFBackend::shared().connect()) {
    std::cerr << "Error: Could not connect to Wayland." << std::endl;
    std::cerr << "Make sure you're running in a Wayland session:" << std::endl;
    std::cerr << "  echo $XDG_SESSION_TYPE  # should say 'wayland'"
              << std::endl;
    return 1;
  }

  // Create window with XDG shell
  auto surface = OSFWaylandSurface::create(400, 300, "Hello VitusOS");
  if (!surface) {
    std::cerr << "Error: Failed to create window" << std::endl;
    OSFBackend::shared().disconnect();
    return 1;
  }

  // Fill with Space Orange (Ares primary color)
  OSFColor spaceOrange = OSFColors::primary();
  surface->draw(spaceOrange);

  std::cout << std::endl;
  std::cout << "╔════════════════════════════════════════════╗" << std::endl;
  std::cout << "║  Window created!                           ║" << std::endl;
  std::cout << "║                                            ║" << std::endl;
  std::cout << "║  Theme: Ares (The Martian)                 ║" << std::endl;
  std::cout << "║  Color: Space Orange #E85D04               ║" << std::endl;
  std::cout << "║                                            ║" << std::endl;
  std::cout << "║  Close the window to exit.                 ║" << std::endl;
  std::cout << "╚════════════════════════════════════════════╝" << std::endl;

  // Run event loop until window is closed
  OSFBackend::shared().run();

  // Cleanup
  surface.reset();
  OSFBackend::shared().disconnect();

  std::cout << "[openSEF] Goodbye!" << std::endl;
  return 0;
}
