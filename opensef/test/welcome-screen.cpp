/**
 * welcome-screen.cpp - VitusOS Welcome Screen
 *
 * Clean, content-focused welcome screen.
 * Decorations (title bar, traffic lights) are handled by the compositor.
 *
 * Features:
 * - Ares theme colors (warm cream background)
 * - Smooth fade transitions between greetings
 * - 60fps animation loop
 */

#include <iostream>
#include <opensef/OpenSEFBackend.h>
#include <opensef/OpenSEFBase.h>
#include <opensef/OpenSEFUI.h>
#include <vector>

using namespace opensef;

// Multilingual greetings
const std::vector<std::string> greetings = {
    "Welcome",   "Bienvenue", "Bienvenido", "Benvenuto", "Willkommen",
    "Bem-vindo", "Welkom",    "Välkommen",  "Tervetuloa"};

// Animation state (subtitle fade only - compositor handles window animations)
struct AnimState {
  int currentIndex = 0;
  float alpha = 1.0f;
  bool fadingOut = false;
  float holdTime = 0.0f;
  const float fadeSpeed = 0.05f;
  const float holdDuration = 60.0f; // ~1 second at 60fps
};

// Global state
AnimState g_anim;
OSFWaylandSurface *g_surface = nullptr;
OSFTextRenderer *g_textRenderer = nullptr;

// Ares theme colors
namespace AresColors {
constexpr uint32_t SoftCream = 0xFBFBFB;     // Background
constexpr uint32_t TextPrimary = 0x2D2D2D;   // "Hello"
constexpr uint32_t TextSecondary = 0x6B6B6B; // Subtitle
constexpr uint32_t SpaceOrange = 0xE85D04;   // Primary button
constexpr uint32_t White = 0xFFFFFF;         // Button text
} // namespace AresColors

// Helper: Fill background
void fillBackground(uint32_t *buffer, int w, int h, uint32_t color) {
  uint32_t pixel = 0xFF000000 | color; // Full opacity
  for (int i = 0; i < w * h; i++) {
    buffer[i] = pixel;
  }
}

// Helper: Draw rectangle
void fillRect(uint32_t *buffer, int bw, int bh, int x, int y, int w, int h,
              uint32_t color) {
  uint32_t pixel = 0xFF000000 | color;
  for (int py = y; py < y + h && py < bh; py++) {
    if (py < 0)
      continue;
    for (int px = x; px < x + w && px < bw; px++) {
      if (px < 0)
        continue;
      buffer[py * bw + px] = pixel;
    }
  }
}

// Helper: Draw rounded rectangle (simplified - corners not rounded yet)
void fillRoundedRect(uint32_t *buffer, int bw, int bh, int x, int y, int w,
                     int h, int radius, uint32_t color) {
  (void)radius; // TODO: Implement corner rounding
  fillRect(buffer, bw, bh, x, y, w, h, color);
}

// Draw frame content (compositor handles decorations)
void drawFrame() {
  if (!g_surface)
    return;
  uint32_t *pixels = g_surface->buffer();
  if (!pixels)
    return;

  int w = g_surface->width();
  int h = g_surface->height();

  // 1. Warm cream background (Ares: #FBFBFB)
  fillBackground(pixels, w, h, AresColors::SoftCream);

  // 2. "Hello" - Large title (centered)
  const std::string title = "Hello";
  int titleSize = 72;
  int titleWidth = g_textRenderer->measureTextWidth(title, titleSize);
  int titleX = (w - titleWidth) / 2;
  int titleY = h / 2 - 60;

  OSFColor titleColor = OSFColor::fromHex(AresColors::TextPrimary);
  g_textRenderer->drawText(pixels, w, h, titleX, titleY, title, titleColor,
                           titleSize);

  // 3. Cycling subtitle with fade effect
  const std::string &subtitle = greetings[g_anim.currentIndex];
  int subSize = 32;
  int subWidth = g_textRenderer->measureTextWidth(subtitle, subSize);
  int subX = (w - subWidth) / 2;
  int subY = h / 2 + 10;

  OSFColor subColor = OSFColor::fromHex(AresColors::TextSecondary);
  subColor.a = g_anim.alpha; // Apply fade
  g_textRenderer->drawText(pixels, w, h, subX, subY, subtitle, subColor,
                           subSize);

  // 4. Continue button (Ares: Space Orange, 28px height, 6px radius)
  int btnW = 180, btnH = 44;
  int btnX = (w - btnW) / 2;
  int btnY = h / 2 + 100;
  fillRoundedRect(pixels, w, h, btnX, btnY, btnW, btnH, 6,
                  AresColors::SpaceOrange);

  // Button label
  const std::string btnLabel = "Continue";
  int btnFontSize = 18;
  int btnTextW = g_textRenderer->measureTextWidth(btnLabel, btnFontSize);
  int btnTextX = btnX + (btnW - btnTextW) / 2;
  int btnTextY = btnY + btnH / 2 + btnFontSize / 3;

  OSFColor btnTextColor = OSFColor::fromHex(AresColors::White);
  g_textRenderer->drawText(pixels, w, h, btnTextX, btnTextY, btnLabel,
                           btnTextColor, btnFontSize);

  g_surface->commit();
}

// Animation tick: Update subtitle fade (60fps)
void onAnimationTick() {
  // Subtitle fade animation
  if (g_anim.holdTime > 0) {
    g_anim.holdTime -= 1.0f;
  } else if (g_anim.fadingOut) {
    g_anim.alpha -= g_anim.fadeSpeed;
    if (g_anim.alpha <= 0.0f) {
      g_anim.alpha = 0.0f;
      g_anim.currentIndex = (g_anim.currentIndex + 1) % greetings.size();
      g_anim.fadingOut = false;
    }
  } else {
    g_anim.alpha += g_anim.fadeSpeed;
    if (g_anim.alpha >= 1.0f) {
      g_anim.alpha = 1.0f;
      g_anim.holdTime = g_anim.holdDuration;
      g_anim.fadingOut = true;
    }
  }

  drawFrame();
}

int main() {
  std::cout << "╔════════════════════════════════════════════════════════╗"
            << std::endl;
  std::cout << "║        VitusOS Ares - Welcome Screen                   ║"
            << std::endl;
  std::cout << "║        (Decorations handled by compositor)             ║"
            << std::endl;
  std::cout << "╚════════════════════════════════════════════════════════╝"
            << std::endl;

  // Connect to Wayland (compositor or display server)
  if (!OSFBackend::shared().connect()) {
    std::cerr << "[welcome-screen] Could not connect to Wayland compositor"
              << std::endl;
    return 1;
  }

  // Initialize text renderer
  g_textRenderer = &OSFTextRenderer::shared();
  g_textRenderer->initialize();

  // Create window (compositor adds decorations)
  auto surface = OSFWaylandSurface::create(800, 600, "Welcome to VitusOS");
  if (!surface) {
    std::cerr << "[welcome-screen] Failed to create window" << std::endl;
    OSFBackend::shared().disconnect();
    return 1;
  }
  g_surface = surface.get();

  // Initialize animation
  g_anim.holdTime = g_anim.holdDuration;
  g_anim.fadingOut = true;

  // Initial draw
  drawFrame();
  std::cout << "[welcome-screen] Running at 60fps..." << std::endl;

  // Run animation loop at 60fps
  OSFBackend::shared().runWithCallback(onAnimationTick, 16);

  // Cleanup
  g_surface = nullptr;
  surface.reset();
  OSFBackend::shared().disconnect();

  std::cout << "[welcome-screen] Goodbye!" << std::endl;
  return 0;
}
