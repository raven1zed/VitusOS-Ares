/**
 * welcome-screen.cpp - Apple-style multilingual welcome
 *
 * Layout:
 *   "Hello" (big, centered)
 *   "Welcome" / "Bonjour" / "Hola" (subtitle, cycling)
 *   [Continue] button
 *
 * Traffic lights: Close/Minimize/Maximize with hover effect
 */

#include <iostream>
#include <opensef/OpenSEFBackend.h>
#include <opensef/OpenSEFBase.h>
#include <opensef/OpenSEFUI.h>
#include <vector>


using namespace opensef;

// Multilingual subtitles (cycles under "Hello")
const std::vector<std::string> subtitles = {
    "Welcome",    "Bienvenue", "Bienvenido",      "Benvenuto",
    "Willkommen", "Bem-vindo", "Добро пожаловать"};

// Global state
int g_subtitleIndex = 0;
OSFWaylandSurface *g_surface = nullptr;
OSFTextRenderer *g_textRenderer = nullptr;

// === ARES THEME COLORS ===
OSFColor g_bgColor;
OSFColor g_textColor;
OSFColor g_subtitleColor;
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

// Draw a filled circle
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

  // 1. Fill Lunar Gray background
  fillBackground(pixels, w, h, g_bgColor);

  // 2. Draw traffic light buttons (top-left)
  int btnRadius = 7;
  int btnY = 20;
  int btnSpacing = 22;
  fillCircle(pixels, w, h, 20, btnY, btnRadius, g_closeColor);
  fillCircle(pixels, w, h, 20 + btnSpacing, btnY, btnRadius, g_minimizeColor);
  fillCircle(pixels, w, h, 20 + btnSpacing * 2, btnY, btnRadius,
             g_maximizeColor);

  // 3. Draw "Hello" (big, centered, fixed)
  const std::string mainText = "Hello";
  int mainFontSize = 72;
  int mainWidth = g_textRenderer->measureTextWidth(mainText, mainFontSize);
  int mainX = (w - mainWidth) / 2;
  int mainY = h / 2 - 60;
  g_textRenderer->drawText(pixels, w, h, mainX, mainY, mainText, g_textColor,
                           mainFontSize);

  // 4. Draw cycling subtitle below "Hello"
  const std::string &subtitle = subtitles[g_subtitleIndex];
  int subFontSize = 32;
  int subWidth = g_textRenderer->measureTextWidth(subtitle, subFontSize);
  int subX = (w - subWidth) / 2;
  int subY = h / 2 + 10;
  g_textRenderer->drawText(pixels, w, h, subX, subY, subtitle, g_subtitleColor,
                           subFontSize);

  // 5. Draw "Continue" button
  int btnWidth = 200;
  int btnHeight = 50;
  int btnX = (w - btnWidth) / 2;
  int btnBotY = h / 2 + 100;
  fillRect(pixels, w, h, btnX, btnBotY, btnWidth, btnHeight, g_buttonColor);

  // 6. Draw button text
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

// Animation callback
void onAnimationTick() {
  g_subtitleIndex = (g_subtitleIndex + 1) % subtitles.size();
  std::cout << "[VitusOS] " << subtitles[g_subtitleIndex] << std::endl;
  drawFrame();
}

int main() {
  std::cout << "╔════════════════════════════════════════════════════════╗"
            << std::endl;
  std::cout << "║       VitusOS - Welcome Screen                         ║"
            << std::endl;
  std::cout << "║       Hello + Cycling Subtitles                        ║"
            << std::endl;
  std::cout << "╚════════════════════════════════════════════════════════╝"
            << std::endl;
  std::cout << std::endl;

  // Connect
  if (!OSFBackend::shared().connect()) {
    std::cerr << "Error: Could not connect to Wayland." << std::endl;
    return 1;
  }

  // Text renderer
  g_textRenderer = &OSFTextRenderer::shared();
  if (!g_textRenderer->initialize()) {
    std::cerr << "Warning: Text renderer failed" << std::endl;
  }

  // Window
  auto surface = OSFWaylandSurface::create(800, 600, "Welcome to VitusOS");
  if (!surface) {
    std::cerr << "Error: Failed to create window" << std::endl;
    OSFBackend::shared().disconnect();
    return 1;
  }
  g_surface = surface.get();

  // Initialize Ares colors
  g_bgColor = OSFColors::background();           // Lunar Gray
  g_textColor = OSFColors::textPrimary();        // Space Charcoal
  g_subtitleColor = OSFColors::textSecondary();  // Lighter gray for subtitle
  g_buttonColor = OSFColors::primary();          // Space Orange
  g_buttonText = OSFColor::fromHex(0xFFFFFF);    // White
  g_closeColor = OSFColor::fromHex(0xFF5F57);    // Red
  g_minimizeColor = OSFColor::fromHex(0xFFBD2E); // Yellow
  g_maximizeColor = OSFColor::fromHex(0x28CA41); // Green

  std::cout << "[VitusOS] Window ready - Ares Theme" << std::endl;

  // Initial frame
  drawFrame();

  // Run with animation (subtitle cycles every 2.5 seconds)
  OSFBackend::shared().runWithCallback(onAnimationTick, 2500);

  // Cleanup
  g_surface = nullptr;
  surface.reset();
  OSFBackend::shared().disconnect();

  std::cout << "[VitusOS] Goodbye!" << std::endl;
  return 0;
}
