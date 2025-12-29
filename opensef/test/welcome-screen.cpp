/**
 * welcome-screen.cpp - Apple-style multilingual welcome
 *
 * Displays greetings in multiple languages with Ares theme
 */

#include <chrono>
#include <iostream>
#include <opensef/OpenSEFBackend.h>
#include <opensef/OpenSEFBase.h>
#include <opensef/OpenSEFUI.h>
#include <thread>
#include <vector>


using namespace opensef;

// Multilingual greetings (Apple-style)
const std::vector<std::string> greetings = {
    "Hello",   "مرحبا", "שלום", "你好",  "こんにちは", "Привет", "Hola",
    "Bonjour", "Ciao",  "Olá",  "Hallo", "Hej",        "Welcome"};

int main() {
  std::cout << "╔════════════════════════════════════════════════════════╗"
            << std::endl;
  std::cout << "║       VitusOS - Welcome Screen                         ║"
            << std::endl;
  std::cout << "║       Multilingual Greetings with Inter Font           ║"
            << std::endl;
  std::cout << "╚════════════════════════════════════════════════════════╝"
            << std::endl;
  std::cout << std::endl;

  // Connect to Wayland
  if (!OSFBackend::shared().connect()) {
    std::cerr << "Error: Could not connect to Wayland." << std::endl;
    return 1;
  }

  // Initialize text renderer
  if (!OSFTextRenderer::shared().initialize()) {
    std::cerr << "Warning: Text renderer failed to initialize" << std::endl;
  }

  // Create window
  auto surface = OSFWaylandSurface::create(800, 600, "Welcome to VitusOS");
  if (!surface) {
    std::cerr << "Error: Failed to create window" << std::endl;
    OSFBackend::shared().disconnect();
    return 1;
  }

  std::cout << std::endl;
  std::cout << "╔════════════════════════════════════════════╗" << std::endl;
  std::cout << "║  Welcome Screen Created!                   ║" << std::endl;
  std::cout << "║                                            ║" << std::endl;
  std::cout << "║  Theme: Ares (Space Charcoal)              ║" << std::endl;
  std::cout << "║  Font: Inter (or fallback)                 ║" << std::endl;
  std::cout << "║                                            ║" << std::endl;
  std::cout << "║  Close the window to exit.                 ║" << std::endl;
  std::cout << "╚════════════════════════════════════════════╝" << std::endl;

  // For now, just show the background color
  // TODO: Replace with animated greeting cycle once basic rendering works
  OSFColor bgColor = OSFColors::backgroundDark(); // Space Charcoal
  surface->draw(bgColor);

  // Run event loop
  OSFBackend::shared().run();

  // Cleanup
  surface.reset();
  OSFBackend::shared().disconnect();

  std::cout << "[VitusOS] Goodbye!" << std::endl;
  return 0;
}
