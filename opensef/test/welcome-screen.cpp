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

#include <iostream>
#include <opensef/OpenSEFBackend.h>
#include <opensef/OpenSEFBase.h>
#include <opensef/OpenSEFUI.h>
#include <vector>


using namespace opensef;

// Multilingual greetings (Apple-style - cycles through)
const std::vector<std::string> greetings = {
    "Hello", "Bonjour", "Hola", "Ciao", "Hallo", "Olá", "Welcome"};

// Global state for animation
int g_greetingIndex = 0;
OSFWaylandSurface *g_surface = nullptr;
OSFTextRenderer *g_textRenderer = nullptr;

// === ARES THEME COLORS ===
OSFColor g_bgColor;
OSFColor g_textColor;
OSFColor g_buttonColor;
OSFColor g_buttonText;
OSFColor g_closeColor;
OSFColor g_minimizeColor;
OSFColor g_maximizeColor;

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

// Draw a single frame
void drawFrame() {
  if (!g_surface)
    return;

  uint32_t *pixels = g_surface->buffer();
  if (!pixels)
    return;

  int w = g_surface->width();
  int h = g_surface->height();

  // 1. Fill background
  fillBackground(pixels, w, h, g_bgColor);

  // 2. Draw traffic light buttons (top-left, macOS style)
  int btnRadius = 7;
  int btnY = 20;
  int btnSpacing = 22;
  fillCircle(pixels, w, h, 20, btnY, btnRadius, g_closeColor);
  fillCircle(pixels, w, h, 20 + btnSpacing, btnY, btnRadius, g_minimizeColor);
  fillCircle(pixels, w, h, 20 + btnSpacing * 2, btnY, btnRadius,
             g_maximizeColor);

  // 3. Draw greeting text (centered, animated)
  const std::string &greeting = greetings[g_greetingIndex];
  int fontSize = 72;
  int textWidth = g_textRenderer->measureTextWidth(greeting, fontSize);
  int x = (w - textWidth) / 2;
  int y = h / 2 - 50;
  g_textRenderer->drawText(pixels, w, h, x, y, greeting, g_textColor, fontSize);

  // 4. Draw "Continue" button
  int btnWidth = 200;
  int btnHeight = 50;
  int btnX = (w - btnWidth) / 2;
  int btnBotY = h / 2 + 80;
  fillRect(pixels, w, h, btnX, btnBotY, btnWidth, btnHeight, g_buttonColor);

  // 5. Draw button text
  const std::string btnLabel = "Continue";
  int btnFontSize = 20;
  int btnTextWidth = g_textRenderer->measureTextWidth(btnLabel, btnFontSize);
  int btnTextX = btnX + (btnWidth - btnTextWidth) / 2;
  int btnTextY = btnBotY + btnHeight / 2 + btnFontSize / 3;
  g_textRenderer->drawText(pixels, w, h, btnTextX, btnTextY, btnLabel,
                           g_buttonText, btnFontSize);

  // Commit
  g_surface->commit();
}

// Animation callback - called every 2 seconds
void onAnimationTick() {
  g_greetingIndex = (g_greetingIndex + 1) % greetings.size();
  std::cout << "[VitusOS] Greeting: " << greetings[g_greetingIndex]
            << std::endl;
  drawFrame();
}

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
  g_textRenderer = &OSFTextRenderer::shared();
  if (!g_textRenderer->initialize()) {
    std::cerr << "Warning: Text renderer failed to initialize" << std::endl;
  }

  // Create window
  auto surface = OSFWaylandSurface::create(800, 600, "Welcome to VitusOS");
  if (!surface) {
    std::cerr << "Error: Failed to create window" << std::endl;
    OSFBackend::shared().disconnect();
    return 1;
  }
  g_surface = surface.get();

  std::cout << "[VitusOS] Window created - Ares Theme with Animation"
            << std::endl;

  // Initialize colors
  g_bgColor = OSFColors::background();           // Lunar Gray
  g_textColor = OSFColors::textPrimary();        // Space Charcoal
  g_buttonColor = OSFColors::primary();          // Space Orange
  g_buttonText = OSFColor::fromHex(0xFFFFFF);    // White
  g_closeColor = OSFColor::fromHex(0xFF5F57);    // Red
  g_minimizeColor = OSFColor::fromHex(0xFFBD2E); // Yellow
  g_maximizeColor = OSFColor::fromHex(0x28CA41); // Green

  // Draw initial frame
  drawFrame();
  std::cout << "[VitusOS] Initial: " << greetings[0] << std::endl;

  // Run event loop with animation callback (every 2000ms)
  OSFBackend::shared().runWithCallback(onAnimationTick, 2000);

  // Cleanup
  g_surface = nullptr;
  surface.reset();
  OSFBackend::shared().disconnect();

  std::cout << "[VitusOS] Goodbye!" << std::endl;
  return 0;
}
