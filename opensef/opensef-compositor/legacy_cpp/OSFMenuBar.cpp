/**
 * OSFMenuBar.cpp - VitusOS Top Menu Bar
 */

#include "OSFMenuBar.h"
#include "OSFCompositor.h"
#include "OSFDecorations.h"

#include <ctime>
#include <iomanip>
#include <sstream>

namespace opensef {

using namespace AresMenuBar;
using namespace AresColors;

OSFMenuBar::OSFMenuBar(OSFCompositor *compositor) : compositor_(compositor) {
  updateClock();
}

void OSFMenuBar::render(uint32_t *buffer, int screenW, int screenH) {
  drawBackground(buffer, screenW, screenH);
  drawMultitaskButton(buffer, screenW, screenH, false);
  drawAppMenu(buffer, screenW, screenH);
  drawSystemTray(buffer, screenW, screenH);
  drawClock(buffer, screenW, screenH);
}

void OSFMenuBar::drawBackground(uint32_t *buf, int sw, int sh) {
  (void)sh;

  // Semi-transparent menu bar background
  uint32_t bgColor = (0xE8 << 24) | LunarGray; // 91% opacity

  for (int y = 0; y < Height; y++) {
    for (int x = 0; x < sw; x++) {
      buf[y * sw + x] = bgColor;
    }
  }

  // Subtle bottom border
  uint32_t borderColor = 0xFF000000 | Borders;
  for (int x = 0; x < sw; x++) {
    buf[(Height - 1) * sw + x] = borderColor;
  }
}

void OSFMenuBar::drawMultitaskButton(uint32_t *buf, int sw, int sh,
                                     bool hovered) {
  (void)sh;

  // Orange circle in top-left corner
  int cx = MultitaskButtonMargin + MultitaskButtonSize / 2;
  int cy = Height / 2;
  int r = MultitaskButtonSize / 2;

  uint32_t color = 0xFF000000 | SpaceOrange;
  if (hovered) {
    // Slightly brighter when hovered
    color = 0xFF000000 | MarsDust;
  }

  for (int y = -r; y <= r; y++) {
    for (int x = -r; x <= r; x++) {
      if (x * x + y * y <= r * r) {
        int px = cx + x;
        int py = cy + y;
        if (px >= 0 && px < sw && py >= 0 && py < Height) {
          buf[py * sw + px] = color;
        }
      }
    }
  }
}

void OSFMenuBar::drawAppMenu(uint32_t *buf, int sw, int sh) {
  (void)buf;
  (void)sw;
  (void)sh;
  // TODO: Draw app name and menu items
  // Will need text rendering integration
}

void OSFMenuBar::drawSystemTray(uint32_t *buf, int sw, int sh) {
  (void)buf;
  (void)sw;
  (void)sh;
  // TODO: Draw wifi, battery, volume icons
}

void OSFMenuBar::drawClock(uint32_t *buf, int sw, int sh) {
  (void)buf;
  (void)sh;
  (void)sw;
  // TODO: Draw time with text rendering
  // For now, clock is stored in currentTime_
}

void OSFMenuBar::updateClock() {
  time_t now = time(nullptr);
  tm *t = localtime(&now);

  std::ostringstream oss;
  oss << std::setfill('0') << std::setw(2) << t->tm_hour << ":"
      << std::setfill('0') << std::setw(2) << t->tm_min;
  currentTime_ = oss.str();
}

bool OSFMenuBar::handleClick(int x, int y) {
  if (y < 0 || y >= Height)
    return false;

  // Check multitask button
  if (isMultitaskHovered(x, y)) {
    if (onMultitaskClicked) {
      onMultitaskClicked();
    }
    return true;
  }

  return false;
}

bool OSFMenuBar::isMultitaskHovered(int x, int y) {
  int cx = MultitaskButtonMargin + MultitaskButtonSize / 2;
  int cy = Height / 2;
  int r = MultitaskButtonSize / 2;

  int dx = x - cx;
  int dy = y - cy;
  return (dx * dx + dy * dy) <= r * r;
}

void OSFMenuBar::setActiveApp(const std::string &appName) {
  activeApp_ = appName;
}

} // namespace opensef
