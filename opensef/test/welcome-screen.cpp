/**
 * welcome-screen.cpp - VitusOS Ares Welcome Screen
 *
 * Premium, polished welcome experience matching the Ares design spec:
 * - Warm cream background (#FBFBFB)
 * - Large "Hello" with fading multilingual subtitle
 * - Space Orange "Continue" button with rounded corners
 * - VitusOS branding at bottom
 *
 * Decorations (traffic lights, title bar) handled by compositor.
 */

#include <cmath>
#include <iostream>
#include <opensef/OpenSEFBackend.h>
#include <opensef/OpenSEFBase.h>
#include <opensef/OpenSEFUI.h>
#include <vector>


using namespace opensef;

// Multilingual greetings (matching Apple's setup experience)
const std::vector<std::string> greetings = {
    "Welcome",    "Bienvenue",        "Bienvenido", "Benvenuto",
    "Willkommen", "Bem-vindo",        "Welkom",     "Välkommen",
    "Tervetuloa", "Добро пожаловать", "欢迎",       "ようこそ"};

// Ares theme colors (from Design Reference)
namespace Ares {
// Primary
constexpr uint32_t SpaceOrange = 0xE85D04;
constexpr uint32_t MarsDust = 0xFB8500;
constexpr uint32_t MissionBlue = 0x3D5A80;
constexpr uint32_t LunarGray = 0xF0F0F0;

// Warmth
constexpr uint32_t SoftCream = 0xFBFBFB; // Background
constexpr uint32_t LavenderMist = 0xEFEBF7;

// Text
constexpr uint32_t TextPrimary = 0x2D2D2D;
constexpr uint32_t TextSecondary = 0x6B6B6B;
constexpr uint32_t TextTertiary = 0x9E9E9E;
constexpr uint32_t White = 0xFFFFFF;
} // namespace Ares

// Animation state
struct AnimState {
  int currentIndex = 0;
  float alpha = 1.0f;
  bool fadingOut = false;
  float holdTime = 0.0f;
  const float fadeSpeed = 0.04f;    // Slightly slower for elegance
  const float holdDuration = 90.0f; // ~1.5 seconds at 60fps
};

// Global state
AnimState g_anim;
OSFWaylandSurface *g_surface = nullptr;
OSFTextRenderer *g_textRenderer = nullptr;

// Drawing helpers

void setPixel(uint32_t *buf, int bw, int bh, int x, int y, uint32_t color) {
  if (x >= 0 && x < bw && y >= 0 && y < bh) {
    buf[y * bw + x] = color;
  }
}

void fillBackground(uint32_t *buffer, int w, int h, uint32_t color) {
  uint32_t pixel = 0xFF000000 | color;
  for (int i = 0; i < w * h; i++) {
    buffer[i] = pixel;
  }
}

void fillRect(uint32_t *buf, int bw, int bh, int x, int y, int w, int h,
              uint32_t color) {
  uint32_t pixel = 0xFF000000 | color;
  for (int py = y; py < y + h && py < bh; py++) {
    if (py < 0)
      continue;
    for (int px = x; px < x + w && px < bw; px++) {
      if (px < 0)
        continue;
      buf[py * bw + px] = pixel;
    }
  }
}

// Fill circle for rounded corners
void fillCircle(uint32_t *buf, int bw, int bh, int cx, int cy, int r,
                uint32_t color) {
  uint32_t pixel = 0xFF000000 | color;
  for (int y = -r; y <= r; y++) {
    for (int x = -r; x <= r; x++) {
      if (x * x + y * y <= r * r) {
        setPixel(buf, bw, bh, cx + x, cy + y, pixel);
      }
    }
  }
}

// Draw rounded rectangle (proper implementation)
void fillRoundedRect(uint32_t *buf, int bw, int bh, int x, int y, int w, int h,
                     int r, uint32_t color) {
  // Clamp radius
  if (r > h / 2)
    r = h / 2;
  if (r > w / 2)
    r = w / 2;

  // Center rectangle (excluding corners)
  fillRect(buf, bw, bh, x + r, y, w - 2 * r, h, color);

  // Left and right strips
  fillRect(buf, bw, bh, x, y + r, r, h - 2 * r, color);
  fillRect(buf, bw, bh, x + w - r, y + r, r, h - 2 * r, color);

  // Four corner circles
  fillCircle(buf, bw, bh, x + r, y + r, r, color);         // Top-left
  fillCircle(buf, bw, bh, x + w - r - 1, y + r, r, color); // Top-right
  fillCircle(buf, bw, bh, x + r, y + h - r - 1, r, color); // Bottom-left
  fillCircle(buf, bw, bh, x + w - r - 1, y + h - r - 1, r,
             color); // Bottom-right
}

// Draw frame content
void drawFrame() {
  if (!g_surface)
    return;
  uint32_t *pixels = g_surface->buffer();
  if (!pixels)
    return;

  int w = g_surface->width();
  int h = g_surface->height();

  // 1. Warm cream background
  fillBackground(pixels, w, h, Ares::SoftCream);

  // 2. VitusOS logo placeholder (small orange circle at top)
  int logoX = w / 2;
  int logoY = h / 4;
  fillCircle(pixels, w, h, logoX, logoY, 24, Ares::SpaceOrange);

  // 3. "Hello" - Large centered title
  const std::string title = "Hello";
  int titleSize = 64;
  int titleWidth = g_textRenderer->measureTextWidth(title, titleSize);
  int titleX = (w - titleWidth) / 2;
  int titleY = h / 2 - 30;

  OSFColor titleColor = OSFColor::fromHex(Ares::TextPrimary);
  g_textRenderer->drawText(pixels, w, h, titleX, titleY, title, titleColor,
                           titleSize);

  // 4. Cycling subtitle with smooth fade
  const std::string &subtitle = greetings[g_anim.currentIndex];
  int subSize = 28;
  int subWidth = g_textRenderer->measureTextWidth(subtitle, subSize);
  int subX = (w - subWidth) / 2;
  int subY = h / 2 + 30;

  OSFColor subColor = OSFColor::fromHex(Ares::TextSecondary);
  subColor.a = g_anim.alpha;
  g_textRenderer->drawText(pixels, w, h, subX, subY, subtitle, subColor,
                           subSize);

  // 5. "Continue" button with rounded corners
  int btnW = 160, btnH = 48;
  int btnX = (w - btnW) / 2;
  int btnY = h / 2 + 100;
  int btnRadius = 8;

  fillRoundedRect(pixels, w, h, btnX, btnY, btnW, btnH, btnRadius,
                  Ares::SpaceOrange);

  // Button label (white, centered)
  const std::string btnLabel = "Continue";
  int btnFontSize = 16;
  int btnTextW = g_textRenderer->measureTextWidth(btnLabel, btnFontSize);
  int btnTextX = btnX + (btnW - btnTextW) / 2;
  int btnTextY = btnY + btnH / 2 + btnFontSize / 3;

  OSFColor btnTextColor = OSFColor::fromHex(Ares::White);
  g_textRenderer->drawText(pixels, w, h, btnTextX, btnTextY, btnLabel,
                           btnTextColor, btnFontSize);

  // 6. Footer branding
  const std::string footer = "VitusOS Ares";
  int footerSize = 12;
  int footerWidth = g_textRenderer->measureTextWidth(footer, footerSize);
  int footerX = (w - footerWidth) / 2;
  int footerY = h - 40;

  OSFColor footerColor = OSFColor::fromHex(Ares::TextTertiary);
  g_textRenderer->drawText(pixels, w, h, footerX, footerY, footer, footerColor,
                           footerSize);

  g_surface->commit();
}

// Animation tick (60fps)
void onAnimationTick() {
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
  std::cout << "║        \"Bringing Life to the Beautiful Future\"         ║"
            << std::endl;
  std::cout << "╚════════════════════════════════════════════════════════╝"
            << std::endl;

  if (!OSFBackend::shared().connect()) {
    std::cerr << "[openSEF] Could not connect to Wayland compositor"
              << std::endl;
    return 1;
  }

  g_textRenderer = &OSFTextRenderer::shared();
  g_textRenderer->initialize();

  auto surface = OSFWaylandSurface::create(800, 600, "Welcome to VitusOS");
  if (!surface) {
    std::cerr << "[openSEF] Failed to create window" << std::endl;
    OSFBackend::shared().disconnect();
    return 1;
  }
  g_surface = surface.get();

  // Initialize with hold on first greeting
  g_anim.holdTime = g_anim.holdDuration * 2; // Hold first greeting longer
  g_anim.fadingOut = true;

  drawFrame();
  std::cout << "[openSEF] Running at 60fps..." << std::endl;

  OSFBackend::shared().runWithCallback(onAnimationTick, 16);

  g_surface = nullptr;
  surface.reset();
  OSFBackend::shared().disconnect();

  std::cout << "[openSEF] Goodbye!" << std::endl;
  return 0;
}
