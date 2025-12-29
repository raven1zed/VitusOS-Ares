/**
 * welcome-screen.cpp - Apple-style multilingual welcome
 *
 * Uses Ares Theme:
 * - Lunar Gray background (#F0F0F0)
 * - Space Charcoal text (#1A1A2E)
 * - Space Orange accent (#E85D04)
 */

#include <iostream>
#include <opensef/OpenSEFBackend.h>
#include <opensef/OpenSEFBase.h>
#include <opensef/OpenSEFUI.h>
#include <vector>


using namespace opensef;

// Multilingual greetings (Apple-style)
const std::vector<std::string> greetings = {"Hello", "Hola",  "Bonjour",
                                            "Ciao",  "Hallo", "Welcome"};

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

// Draw a filled rectangle (for buttons)
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

int main() {
  std::cout << "╔════════════════════════════════════════════════════════╗"
            << std::endl;
  std::cout << "║       VitusOS - Welcome Screen                         ║"
            << std::endl;
  std::cout << "║       Ares Theme - Lunar Gray + Space Orange           ║"
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

  std::cout << "[VitusOS] Window created - Ares Theme" << std::endl;

  // === ARES THEME COLORS ===
  OSFColor bgColor = OSFColors::background();        // Lunar Gray #F0F0F0
  OSFColor textColor = OSFColors::textPrimary();     // Space Charcoal #1A1A2E
  OSFColor buttonColor = OSFColors::primary();       // Space Orange #E85D04
  OSFColor buttonText = OSFColor::fromHex(0xFFFFFF); // White on orange

  // Draw frame
  uint32_t *pixels = surface->buffer();
  if (pixels) {
    int w = surface->width();
    int h = surface->height();

    // 1. Fill Lunar Gray background
    fillBackground(pixels, w, h, bgColor);

    // 2. Draw greeting text (centered)
    const std::string &greeting = greetings[0]; // "Hello"
    int fontSize = 72;
    int textWidth = textRenderer.measureTextWidth(greeting, fontSize);
    int x = (w - textWidth) / 2;
    int y = h / 2 - 50; // Above center
    textRenderer.drawText(pixels, w, h, x, y, greeting, textColor, fontSize);

    // 3. Draw "Continue" button (Space Orange)
    int btnWidth = 200;
    int btnHeight = 50;
    int btnX = (w - btnWidth) / 2;
    int btnY = h / 2 + 80; // Below greeting
    fillRect(pixels, w, h, btnX, btnY, btnWidth, btnHeight, buttonColor);

    // 4. Draw button text
    const std::string btnLabel = "Continue";
    int btnFontSize = 20;
    int btnTextWidth = textRenderer.measureTextWidth(btnLabel, btnFontSize);
    int btnTextX = btnX + (btnWidth - btnTextWidth) / 2;
    int btnTextY = btnY + btnHeight / 2 + btnFontSize / 3;
    textRenderer.drawText(pixels, w, h, btnTextX, btnTextY, btnLabel,
                          buttonText, btnFontSize);

    // Commit to display
    surface->commit();

    std::cout << "[VitusOS] Displaying: " << greeting << std::endl;
    std::cout << "[VitusOS] Button: Continue (Space Orange)" << std::endl;
  }

  // Run event loop
  OSFBackend::shared().run();

  // Cleanup
  surface.reset();
  OSFBackend::shared().disconnect();

  std::cout << "[VitusOS] Goodbye!" << std::endl;
  return 0;
}
