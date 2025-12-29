/**
 * welcome-screen.cpp - Apple-style welcome with FADE animation
 *
 * Features:
 * - Smooth fade transitions between greetings
 * - 60fps animation loop
 * - Traffic light buttons with close action
 * - Ares theme colors
 */

#include <cmath>
#include <iostream>
#include <opensef/OpenSEFBackend.h>
#include <opensef/OpenSEFBase.h>
#include <opensef/OpenSEFUI.h>
#include <vector>


using namespace opensef;

// Multilingual subtitles
const std::vector<std::string> subtitles = {"Welcome",    "Bienvenue",
                                            "Bienvenido", "Benvenuto",
                                            "Willkommen", "Bem-vindo"};

// Animation state
struct AnimState {
  int currentIndex = 0;
  int nextIndex = 1;
  float alpha = 1.0f;               // Current text alpha (1.0 = visible)
  bool fadingOut = false;           // true = fading out, false = fading in
  float holdTime = 0.0f;            // Time to hold before next fade
  const float fadeSpeed = 0.05f;    // Alpha change per frame
  const float holdDuration = 60.0f; // Frames to hold (60 = ~1 second at 60fps)
};

// Global state
AnimState g_anim;
OSFWaylandSurface *g_surface = nullptr;
OSFTextRenderer *g_textRenderer = nullptr;

// Ares theme colors
OSFColor g_bgColor;
OSFColor g_textColor;
OSFColor g_subtitleColor;
OSFColor g_buttonColor;
OSFColor g_buttonText;
OSFColor g_closeColor;
OSFColor g_minimizeColor;
OSFColor g_maximizeColor;

// Fill with color
void fillBackground(uint32_t *buffer, int width, int height,
                    const OSFColor &color) {
  uint32_t pixel = (static_cast<uint32_t>(color.a * 255) << 24) |
                   (static_cast<uint32_t>(color.r * 255) << 16) |
                   (static_cast<uint32_t>(color.g * 255) << 8) |
                   static_cast<uint32_t>(color.b * 255);
  for (int i = 0; i < width * height; i++)
    buffer[i] = pixel;
}

// Draw rectangle
void fillRect(uint32_t *buffer, int bw, int bh, int x, int y, int w, int h,
              const OSFColor &c) {
  uint32_t pixel = (static_cast<uint32_t>(c.a * 255) << 24) |
                   (static_cast<uint32_t>(c.r * 255) << 16) |
                   (static_cast<uint32_t>(c.g * 255) << 8) |
                   static_cast<uint32_t>(c.b * 255);
  for (int row = y; row < y + h && row < bh; row++)
    for (int col = x; col < x + w && col < bw; col++)
      if (row >= 0 && col >= 0)
        buffer[row * bw + col] = pixel;
}

// Draw circle
void fillCircle(uint32_t *buffer, int bw, int bh, int cx, int cy, int r,
                const OSFColor &c) {
  uint32_t pixel = (static_cast<uint32_t>(c.a * 255) << 24) |
                   (static_cast<uint32_t>(c.r * 255) << 16) |
                   (static_cast<uint32_t>(c.g * 255) << 8) |
                   static_cast<uint32_t>(c.b * 255);
  for (int y = cy - r; y <= cy + r; y++)
    for (int x = cx - r; x <= cx + r; x++)
      if ((x - cx) * (x - cx) + (y - cy) * (y - cy) <= r * r && x >= 0 &&
          x < bw && y >= 0 && y < bh)
        buffer[y * bw + x] = pixel;
}

// Draw frame with current animation state
void drawFrame() {
  if (!g_surface)
    return;
  uint32_t *pixels = g_surface->buffer();
  if (!pixels)
    return;

  int w = g_surface->width();
  int h = g_surface->height();

  // 1. Background
  fillBackground(pixels, w, h, g_bgColor);

  // 2. Traffic lights
  fillCircle(pixels, w, h, 20, 20, 7, g_closeColor);
  fillCircle(pixels, w, h, 42, 20, 7, g_minimizeColor);
  fillCircle(pixels, w, h, 64, 20, 7, g_maximizeColor);

  // 3. "Hello" (fixed, always visible)
  const std::string mainText = "Hello";
  int mainFontSize = 72;
  int mainWidth = g_textRenderer->measureTextWidth(mainText, mainFontSize);
  int mainX = (w - mainWidth) / 2;
  int mainY = h / 2 - 60;
  g_textRenderer->drawText(pixels, w, h, mainX, mainY, mainText, g_textColor,
                           mainFontSize);

  // 4. Cycling subtitle with FADE effect
  const std::string &subtitle = subtitles[g_anim.currentIndex];
  int subFontSize = 32;
  int subWidth = g_textRenderer->measureTextWidth(subtitle, subFontSize);
  int subX = (w - subWidth) / 2;
  int subY = h / 2 + 10;

  // Apply alpha to subtitle color
  OSFColor fadeColor = g_subtitleColor;
  fadeColor.a = g_anim.alpha;
  g_textRenderer->drawText(pixels, w, h, subX, subY, subtitle, fadeColor,
                           subFontSize);

  // 5. Continue button
  int btnW = 200, btnH = 50;
  int btnX = (w - btnW) / 2, btnY = h / 2 + 100;
  fillRect(pixels, w, h, btnX, btnY, btnW, btnH, g_buttonColor);

  const std::string btnLabel = "Continue";
  int btnFontSize = 20;
  int btnTextW = g_textRenderer->measureTextWidth(btnLabel, btnFontSize);
  int btnTextX = btnX + (btnW - btnTextW) / 2;
  int btnTextY = btnY + btnH / 2 + btnFontSize / 3;
  g_textRenderer->drawText(pixels, w, h, btnTextX, btnTextY, btnLabel,
                           g_buttonText, btnFontSize);

  g_surface->commit();
}

// Animation tick (called every ~16ms for 60fps)
void onAnimationTick() {
  // State machine: HOLD -> FADE_OUT -> SWITCH -> FADE_IN -> HOLD

  if (g_anim.holdTime > 0) {
    // Holding visible text
    g_anim.holdTime -= 1.0f;
  } else if (g_anim.fadingOut) {
    // Fading out
    g_anim.alpha -= g_anim.fadeSpeed;
    if (g_anim.alpha <= 0.0f) {
      g_anim.alpha = 0.0f;
      // Switch to next subtitle
      g_anim.currentIndex = (g_anim.currentIndex + 1) % subtitles.size();
      g_anim.fadingOut = false; // Start fading in
    }
  } else {
    // Fading in
    g_anim.alpha += g_anim.fadeSpeed;
    if (g_anim.alpha >= 1.0f) {
      g_anim.alpha = 1.0f;
      g_anim.holdTime = g_anim.holdDuration; // Hold for a while
      g_anim.fadingOut = true;               // Next cycle: fade out
    }
  }

  drawFrame();
}

int main() {
  std::cout << "╔════════════════════════════════════════════════════════╗"
            << std::endl;
  std::cout << "║       VitusOS - Welcome Screen (60fps Animation)       ║"
            << std::endl;
  std::cout << "╚════════════════════════════════════════════════════════╝"
            << std::endl;

  if (!OSFBackend::shared().connect()) {
    std::cerr << "Error: Could not connect to Wayland." << std::endl;
    return 1;
  }

  g_textRenderer = &OSFTextRenderer::shared();
  g_textRenderer->initialize();

  auto surface = OSFWaylandSurface::create(800, 600, "Welcome to VitusOS");
  if (!surface) {
    std::cerr << "Error: Failed to create window" << std::endl;
    OSFBackend::shared().disconnect();
    return 1;
  }
  g_surface = surface.get();

  // Ares colors
  g_bgColor = OSFColors::background();
  g_textColor = OSFColors::textPrimary();
  g_subtitleColor = OSFColors::textSecondary();
  g_buttonColor = OSFColors::primary();
  g_buttonText = OSFColor::fromHex(0xFFFFFF);
  g_closeColor = OSFColor::fromHex(0xFF5F57);
  g_minimizeColor = OSFColor::fromHex(0xFFBD2E);
  g_maximizeColor = OSFColor::fromHex(0x28CA41);

  // Initial state
  g_anim.holdTime = g_anim.holdDuration;
  g_anim.fadingOut = true;

  drawFrame();
  std::cout << "[VitusOS] 60fps animation started" << std::endl;

  // Run at ~60fps (16ms interval)
  OSFBackend::shared().runWithCallback(onAnimationTick, 16);

  g_surface = nullptr;
  surface.reset();
  OSFBackend::shared().disconnect();

  std::cout << "[VitusOS] Goodbye!" << std::endl;
  return 0;
}
