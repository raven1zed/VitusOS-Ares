/**
 * welcome-screen.cpp - Apple-style multilingual welcome with animation
 *
 * Uses Ares Theme:
 * - Lunar Gray background (#F0F0F0)
 * - Space Charcoal text (#1A1A2E)
 * - Space Orange accent (#E85D04)
 *
 * Traffic Light Buttons (macOS style)
 */

#include <atomic>
#include <chrono>
#include <iostream>
#include <opensef/OpenSEFBackend.h>
#include <opensef/OpenSEFBase.h>
#include <opensef/OpenSEFUI.h>
#include <thread>
#include <vector>


using namespace opensef;

// Multilingual greetings (Apple-style - cycles through)
const std::vector<std::string> greetings = {
    "Hello", "Bonjour", "Hola", "Ciao", "Hallo", "Olá", "Welcome"};

// Fill buffer with solid color
void fillBackground(uint32_t *buffer, int width, int height,
                    const OSFColor &color) {
  uint32_t pixel = (static_cast<uint32_t>(color.a * 255) << 24) |
                   (static_cast<uint32_t>(color.r * 255) << 16) |
                   (static_cast<uint32_t>(color.g * 255) << 8) |
                   static_cast<uint32_t>(color.b * 255);

  for (int i = 0; i < width * height; i++) {
    buffer[i] = pixel;
  }
}

// Draw a filled rectangle
void fillRect(uint32_t *buffer, int bufWidth, int bufHeight, int x, int y,
              int w, int h, const OSFColor &color) {
  uint32_t pixel = (static_cast<uint32_t>(color.a * 255) << 24) |
                   (static_cast<uint32_t>(color.r * 255) << 16) |
                   (static_cast<uint32_t>(color.g * 255) << 8) |
                   static_cast<uint32_t>(color.b * 255);

  for (int row = y; row < y + h && row < bufHeight; row++) {
    for (int col = x; col < x + w && col < bufWidth; col++) {
      if (row >= 0 && col >= 0) {
        buffer[row * bufWidth + col] = pixel;
      }
    }
  }
}

// Draw a filled circle (for traffic light buttons)
void fillCircle(uint32_t *buffer, int bufWidth, int bufHeight, int cx, int cy,
                int radius, const OSFColor &color) {
  uint32_t pixel = (static_cast<uint32_t>(color.a * 255) << 24) |
                   (static_cast<uint32_t>(color.r * 255) << 16) |
                   (static_cast<uint32_t>(color.g * 255) << 8) |
                   static_cast<uint32_t>(color.b * 255);

  for (int y = cy - radius; y <= cy + radius; y++) {
    for (int x = cx - radius; x <= cx + radius; x++) {
      int dx = x - cx;
      int dy = y - cy;
      if (dx * dx + dy * dy <= radius * radius) {
        if (x >= 0 && x < bufWidth && y >= 0 && y < bufHeight) {
          buffer[y * bufWidth + x] = pixel;
        }
      }
    }
  }
}

// Global for animation control
std::atomic<bool> running{true};

int main() {
  std::cout << "╔════════════════════════════════════════════════════════╗"
            << std::endl;
  std::cout << "║       VitusOS - Welcome Screen                         ║"
            << std::endl;
  std::cout << "║       Animated Greetings + Traffic Light Buttons       ║"
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
  auto &textRenderer = OSFTextRenderer::shared();
  if (!textRenderer.initialize()) {
    std::cerr << "Warning: Text renderer failed to initialize" << std::endl;
  }

  // Create window
  auto surface = OSFWaylandSurface::create(800, 600, "Welcome to VitusOS");
  if (!surface) {
    std::cerr << "Error: Failed to create window" << std::endl;
    OSFBackend::shared().disconnect();
    return 1;
  }

  std::cout << "[VitusOS] Window created - Ares Theme with Animation"
            << std::endl;

  // === ARES THEME COLORS ===
  OSFColor bgColor = OSFColors::background();        // Lunar Gray
  OSFColor textColor = OSFColors::textPrimary();     // Space Charcoal
  OSFColor buttonColor = OSFColors::primary();       // Space Orange
  OSFColor buttonText = OSFColor::fromHex(0xFFFFFF); // White

  // Traffic light button colors (macOS style)
  OSFColor closeColor = OSFColor::fromHex(0xFF5F57);    // Red
  OSFColor minimizeColor = OSFColor::fromHex(0xFFBD2E); // Yellow
  OSFColor maximizeColor = OSFColor::fromHex(0x28CA41); // Green

  int greetingIndex = 0;

  // Animation thread - cycles greetings every 2 seconds
  std::thread animThread([&]() {
    while (running) {
      std::this_thread::sleep_for(std::chrono::seconds(2));
      if (!running)
        break;

      greetingIndex = (greetingIndex + 1) % greetings.size();

      uint32_t *pixels = surface->buffer();
      if (!pixels)
        continue;

      int w = surface->width();
      int h = surface->height();

      // === REDRAW FRAME ===

      // 1. Fill background
      fillBackground(pixels, w, h, bgColor);

      // 2. Draw traffic light buttons (top-left, macOS style)
      int btnRadius = 7;
      int btnY = 20;
      int btnSpacing = 22;
      fillCircle(pixels, w, h, 20, btnY, btnRadius, closeColor);
      fillCircle(pixels, w, h, 20 + btnSpacing, btnY, btnRadius, minimizeColor);
      fillCircle(pixels, w, h, 20 + btnSpacing * 2, btnY, btnRadius,
                 maximizeColor);

      // 3. Draw greeting text (centered, animated)
      const std::string &greeting = greetings[greetingIndex];
      int fontSize = 72;
      int textWidth = textRenderer.measureTextWidth(greeting, fontSize);
      int x = (w - textWidth) / 2;
      int y = h / 2 - 50;
      textRenderer.drawText(pixels, w, h, x, y, greeting, textColor, fontSize);

      // 4. Draw "Continue" button
      int btnWidth = 200;
      int btnHeight = 50;
      int btnX = (w - btnWidth) / 2;
      int btnBotY = h / 2 + 80;
      fillRect(pixels, w, h, btnX, btnBotY, btnWidth, btnHeight, buttonColor);

      // 5. Draw button text
      const std::string btnLabel = "Continue";
      int btnFontSize = 20;
      int btnTextWidth = textRenderer.measureTextWidth(btnLabel, btnFontSize);
      int btnTextX = btnX + (btnWidth - btnTextWidth) / 2;
      int btnTextY = btnBotY + btnHeight / 2 + btnFontSize / 3;
      textRenderer.drawText(pixels, w, h, btnTextX, btnTextY, btnLabel,
                            buttonText, btnFontSize);

      // Commit
      surface->commit();

      std::cout << "[VitusOS] Greeting: " << greeting << std::endl;
    }
  });

  // Draw initial frame
  uint32_t *pixels = surface->buffer();
  if (pixels) {
    int w = surface->width();
    int h = surface->height();

    fillBackground(pixels, w, h, bgColor);

    // Traffic light buttons
    int btnRadius = 7;
    int btnY = 20;
    int btnSpacing = 22;
    fillCircle(pixels, w, h, 20, btnY, btnRadius, closeColor);
    fillCircle(pixels, w, h, 20 + btnSpacing, btnY, btnRadius, minimizeColor);
    fillCircle(pixels, w, h, 20 + btnSpacing * 2, btnY, btnRadius,
               maximizeColor);

    // Initial greeting
    const std::string &greeting = greetings[0];
    int fontSize = 72;
    int textWidth = textRenderer.measureTextWidth(greeting, fontSize);
    int x = (w - textWidth) / 2;
    int y = h / 2 - 50;
    textRenderer.drawText(pixels, w, h, x, y, greeting, textColor, fontSize);

    // Button
    int btnWidth = 200;
    int btnHeight = 50;
    int btnX = (w - btnWidth) / 2;
    int btnBotY = h / 2 + 80;
    fillRect(pixels, w, h, btnX, btnBotY, btnWidth, btnHeight, buttonColor);

    const std::string btnLabel = "Continue";
    int btnFontSize = 20;
    int btnTextWidth = textRenderer.measureTextWidth(btnLabel, btnFontSize);
    int btnTextX = btnX + (btnWidth - btnTextWidth) / 2;
    int btnTextY = btnBotY + btnHeight / 2 + btnFontSize / 3;
    textRenderer.drawText(pixels, w, h, btnTextX, btnTextY, btnLabel,
                          buttonText, btnFontSize);

    surface->commit();
    std::cout << "[VitusOS] Initial: " << greeting << std::endl;
  }

  // Run event loop
  OSFBackend::shared().run();

  // Cleanup
  running = false;
  animThread.join();
  surface.reset();
  OSFBackend::shared().disconnect();

  std::cout << "[VitusOS] Goodbye!" << std::endl;
  return 0;
}
