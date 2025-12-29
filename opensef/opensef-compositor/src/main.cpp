/**
 * main.cpp - openSEF Compositor Entry Point
 *
 * Launches the VitusOS Ares Wayland compositor.
 */

#include "OSFCompositor.h"
#include <iostream>

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  std::cout << "╔════════════════════════════════════════════════════════╗"
            << std::endl;
  std::cout << "║        openSEF Compositor - VitusOS Ares               ║"
            << std::endl;
  std::cout << "║        Powered by wlroots                              ║"
            << std::endl;
  std::cout << "╚════════════════════════════════════════════════════════╝"
            << std::endl;

  // Initialize wlroots logging
  wlr_log_init(WLR_DEBUG, nullptr);

  auto &compositor = opensef::OSFCompositor::shared();

  if (!compositor.initialize()) {
    std::cerr << "[openSEF] Failed to initialize compositor" << std::endl;
    return 1;
  }

  std::cout << "[openSEF] Compositor initialized successfully" << std::endl;
  std::cout << "[openSEF] Starting event loop..." << std::endl;

  compositor.run();

  std::cout << "[openSEF] Shutting down..." << std::endl;
  compositor.shutdown();

  return 0;
}
