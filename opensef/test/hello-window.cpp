/**
 * hello-window.cpp - Basic openSEF test
 *
 * Creates a window and displays text to verify the framework works.
 */

#include <iostream>
#include <opensef/OpenSEFAppKit.h>
#include <opensef/OpenSEFBackend.h>
#include <opensef/OpenSEFBase.h>
#include <opensef/OpenSEFUI.h>


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

  // Initialize Vulkan
  if (!OSFVulkanRenderer::shared().initialize()) {
    std::cerr << "Warning: Vulkan initialization failed (continuing anyway)"
              << std::endl;
  }

  // Create window
  auto window = OSFWindow::create("Hello VitusOS", OSFRect(0, 0, 400, 300));

  // Create glass panel (Ares style)
  auto glass = OSFGlassPanel::create(OSFRect(0, 0, 400, 300));
  glass->setTintColor(OSFColors::surface());

  // Create label
  auto label = std::make_shared<OSFLabel>("Welcome to VitusOS Ares!");
  label->setFrame(OSFRect(50, 100, 300, 40));
  label->setTextColor(OSFColors::primary()); // Space Orange

  // Create button
  auto button = OSFButton::create("Click Me", []() {
    std::cout << "[hello-window] Button clicked!" << std::endl;
  });
  button->setFrame(OSFRect(150, 180, 100, 40));

  // Build view hierarchy
  glass->addSubview(label);
  glass->addSubview(button);
  window->setContentView(glass);

  // Show window
  window->show();

  std::cout << std::endl;
  std::cout << "Window created successfully!" << std::endl;
  std::cout << "Theme: Ares (The Martian)" << std::endl;
  std::cout << "Primary color: Space Orange #E85D04" << std::endl;
  std::cout << std::endl;
  std::cout << "Press Ctrl+C to exit." << std::endl;

  // Run event loop
  OSFBackend::shared().run();

  // Cleanup
  OSFVulkanRenderer::shared().shutdown();
  OSFBackend::shared().disconnect();

  return 0;
}
