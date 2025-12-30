/**
 * OSFSceneMenuBar.cpp - Scene-based Menu Bar Implementation
 *
 * Based on VitusOS Ares Desktop mockup:
 * - Semi-transparent menu bar across top
 * - Orange multitask button (far left)
 * - Menu items: "Filer Menu Settings Help"
 * - Date/time on right: "Tuesday,12 10:15 PM"
 */

#include "OSFSceneMenuBar.h"
#include "OSFAresTheme.h"
#include "OSFCompositor.h"
#include "OSFDesktopLayers.h"


#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>


namespace opensef {

OSFSceneMenuBar::OSFSceneMenuBar(OSFCompositor *compositor,
                                 OSFDesktopLayers *layers)
    : compositor_(compositor), layers_(layers) {}

OSFSceneMenuBar::~OSFSceneMenuBar() {
  // Scene nodes cleaned up by scene tree
}

void OSFSceneMenuBar::create(int screenWidth) {
  screenWidth_ = screenWidth;

  // Menu bar background (semi-transparent)
  float bgColor[4];
  AresTheme::hexToRGBA(AresTheme::MenuBarBg, bgColor, 0.85f); // 85% opacity

  background_ =
      layers_->createRect(DesktopLayer::Overlay, screenWidth, Height, bgColor);
  if (background_) {
    wlr_scene_node_set_position(&background_->node, 0, 0);
  }

  // Multitask button (orange square, far left)
  float buttonColor[4];
  AresTheme::hexToRGBA(AresTheme::SpaceOrange, buttonColor);

  multitaskButton_ =
      layers_->createRect(DesktopLayer::Overlay, MultitaskButtonSize,
                          MultitaskButtonSize, buttonColor);
  if (multitaskButton_) {
    int buttonX = MultitaskButtonMargin;
    int buttonY = (Height - MultitaskButtonSize) / 2;
    wlr_scene_node_set_position(&multitaskButton_->node, buttonX, buttonY);

    // Store button bounds for hit testing
    multitaskBounds_ = {buttonX, buttonY, MultitaskButtonSize,
                        MultitaskButtonSize};
  }

  // Menu items would be rendered using OSFTextRenderer
  // Menu items: "Filer", "Menu", "Settings", "Help"
  // For now, we note their approximate positions for future text rendering

  // Layout: [■] Filer  Menu  Settings  Help                    Tuesday,12 10:15
  // PM
  //         ^-- multitask button starts at x=8
  //         Menu text starts at x = 8 + 20 + 12 = 40

  menuStartX_ = MultitaskButtonMargin + MultitaskButtonSize + 12;

  std::cout << "[openSEF] Menu bar created (width: " << screenWidth << "px)"
            << std::endl;
  std::cout << "[openSEF]   Layout: [■] Filer  Menu  Settings  Help | "
            << getFormattedDateTime() << std::endl;
}

void OSFSceneMenuBar::resize(int screenWidth) {
  if (screenWidth_ == screenWidth) {
    return;
  }
  screenWidth_ = screenWidth;

  // Resize background
  if (background_) {
    wlr_scene_rect_set_size(background_, screenWidth, Height);
  }
}

std::string OSFSceneMenuBar::getFormattedDateTime() const {
  std::time_t now = std::time(nullptr);
  std::tm *local = std::localtime(&now);

  // Day names
  const char *days[] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                        "Thursday", "Friday", "Saturday"};

  // Format: "Tuesday,12 10:15 PM"
  int hour = local->tm_hour;
  const char *ampm = (hour >= 12) ? "PM" : "AM";
  if (hour > 12)
    hour -= 12;
  if (hour == 0)
    hour = 12;

  std::ostringstream oss;
  oss << days[local->tm_wday] << "," << local->tm_mday << " " << hour << ":"
      << std::setfill('0') << std::setw(2) << local->tm_min << " " << ampm;

  return oss.str();
}

bool OSFSceneMenuBar::hitTestMultitask(int x, int y) const {
  return x >= multitaskBounds_.x &&
         x < multitaskBounds_.x + multitaskBounds_.w &&
         y >= multitaskBounds_.y && y < multitaskBounds_.y + multitaskBounds_.h;
}

int OSFSceneMenuBar::hitTestMenuItem(int x, int y) const {
  if (y < 0 || y >= Height)
    return -1;

  // Approximate menu item positions (based on text width estimates)
  // [Filer] [Menu] [Settings] [Help]
  // Starts at menuStartX_, each item ~60px wide with 8px spacing

  int itemX = menuStartX_;
  int itemWidth = 60;
  int spacing = 8;

  for (int i = 0; i < 4; i++) {
    if (x >= itemX && x < itemX + itemWidth) {
      return i; // 0=Filer, 1=Menu, 2=Settings, 3=Help
    }
    itemX += itemWidth + spacing;
  }

  return -1;
}

void OSFSceneMenuBar::updateTime() {
  // TODO: Update time display text
  // Would call OSFLabel::setText() on time label
}

} // namespace opensef
