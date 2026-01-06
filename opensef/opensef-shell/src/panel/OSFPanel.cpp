#include "OSFPanel.h"
#include "OSFAresTheme.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <librsvg/rsvg.h> // For icon loading
#include <linux/input-event-codes.h>
#include <sstream>

namespace opensef {

// Constants for layout
constexpr double MultitaskButtonMargin = 0; // Flush left
constexpr double MultitaskButtonSize = 28;  // Full button sizeon (Full Height)
constexpr double MenuSpacing = 24;
constexpr double DropdownWidth = 180;
constexpr double DropdownItemHeight = 28;
constexpr double SeparatorHeight = 9;

OSFPanel::OSFPanel() {
  surface_ = std::make_unique<OSFSurface>("osf-panel");

  // Configure for top panel
  surface_->setLayer(Layer::Top);
  surface_->setAnchor(Anchor::Top | Anchor::Left | Anchor::Right);

  // Height: We need enough space for dropdowns to be visible.
  // We set the surface height to 400px (arbitrary dropdown limit),
  // BUT the exclusive zone remains PanelHeight so windows don't shift down.
  // The area below the panel will be transparent.
  int expandedHeight = 400;
  surface_->setSize(0, expandedHeight);
  surface_->setExclusiveZone(AresTheme::PanelHeight);

  initMenus();

  // Bind callbacks
  surface_->onDraw([this](cairo_t *cr, int w, int h) { this->draw(cr, w, h); });
  surface_->onMouseMove(
      [this](int x, int y) { this->handlePointerMove(x, y); });
  surface_->onMouseDown([this](int x, int y, uint32_t button) {
    this->handlePointerDown(x, y, button);
  });
  surface_->onMouseUp([this](int x, int y, uint32_t button) {
    this->handlePointerUp(x, y, button);
  });
  surface_->onMouseLeave([this]() { this->clearHover(); });
  surface_->onTick([this]() { this->updateClock(); });
  surface_->setFrameTimer(1000); // Check every second
}

void OSFPanel::initMenus() {
  // Filer menu
  MenuDef filer;
  filer.title = "Filer";
  filer.items = {
      {"New Window", []() { std::cout << "[Filer] New Window" << std::endl; }},
      {"New Folder", []() { std::cout << "[Filer] New Folder" << std::endl; }},
      {"Open...", []() { std::cout << "[Filer] Open" << std::endl; }},
      {"Close", []() { std::cout << "[Filer] Close" << std::endl; }},
      {"---", nullptr, true}, // Separator
      {"Quit", []() { std::cout << "[Filer] Quit" << std::endl; }}};
  menus_.push_back(filer);

  // Menu menu (general app menu)
  MenuDef menu;
  menu.title = "Menu";
  menu.items = {{"About VitusOS",
                 []() { std::cout << "[Menu] About VitusOS" << std::endl; }},
                {"Preferences...",
                 []() { std::cout << "[Menu] Preferences" << std::endl; }}};
  menus_.push_back(menu);

  // Settings menu
  MenuDef settings;
  settings.title = "Settings";
  settings.items = {
      {"Display", []() { std::cout << "[Settings] Display" << std::endl; }},
      {"Network", []() { std::cout << "[Settings] Network" << std::endl; }},
      {"Sound", []() { std::cout << "[Settings] Sound" << std::endl; }},
      {"---", nullptr, true},
      {"System...", []() { std::cout << "[Settings] System" << std::endl; }}};
  menus_.push_back(settings);

  // Help menu
  MenuDef help;
  help.title = "Help";
  help.items = {{"VitusOS Help",
                 []() { std::cout << "[Help] VitusOS Help" << std::endl; }},
                {"Report Issue",
                 []() { std::cout << "[Help] Report Issue" << std::endl; }}};
  menus_.push_back(help);
}

void OSFPanel::run() {
  if (surface_->connect()) {
    surface_->run();
  } else {
    std::cerr << "Failed to connect OSFPanel to display." << std::endl;
  }
}

void OSFPanel::draw(cairo_t *cr, int width, int height) {
  // 1. Clear implementation for transparency (Critical for dropdowns)
  cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
  cairo_paint(cr);

  // 2. Draw Panel Background (Top Strip Only)
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  AresTheme::setCairoColor(cr, AresTheme::PanelBackground);
  cairo_rectangle(cr, 0, 0, width, AresTheme::PanelHeight);
  cairo_fill(cr);

  // 3. Draw Multitask Button - Explicitly set OVER operator
  cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
  drawMultitaskButton(cr);

  // 4. Draw menu titles
  drawMenuTitles(cr, width);

  // 5. Draw clock on right
  drawSystemTray(cr, width, height); // WiFi / Battery
  drawClock(cr, width, height);

  // 6. Draw dropdown if open
  std::vector<cairo_rectangle_int_t> inputRects;
  // Always include the top bar
  inputRects.push_back({0, 0, width, (int)AresTheme::PanelHeight});

  if (openMenuIndex_ >= 0 && openMenuIndex_ < (int)menus_.size()) {
    drawDropdown(cr, openMenuIndex_);

    // Add dropdown input region
    double x = AresTheme::PanelHeight + 16;
    for (int i = 0; i < openMenuIndex_; ++i) {
      cairo_text_extents_t extents;
      cairo_text_extents(cr, menus_[i].title.c_str(), &extents);
      x += extents.width + MenuSpacing;
    }

    // Calculate dropdown dimensions (reproduce logic from drawDropdown)
    // We assume DropdownWidth and rough height estimation
    int dropdownH =
        menus_[openMenuIndex_].items.size() * DropdownItemHeight + 10;
    inputRects.push_back(
        {(int)x, (int)AresTheme::PanelHeight, (int)DropdownWidth, dropdownH});
  }

  // Update Input Region to fix flickering
  surface_->setInputRegion(inputRects);

  // Set Opaque Region for the panel bar itself (optimization)
  std::vector<cairo_rectangle_int_t> opaqueRects;
  opaqueRects.push_back({0, 0, width, (int)AresTheme::PanelHeight});
  surface_->setOpaqueRegion(opaqueRects);
}

void OSFPanel::drawMultitaskButton(cairo_t *cr) {
  // Orange button for multitask view - Top Left Corner (28x28)
  AresTheme::setCairoColor(cr, AresTheme::MarsOrange);
  cairo_rectangle(cr, 0, 0, AresTheme::PanelHeight,
                  AresTheme::PanelHeight); // Full square
  cairo_fill(cr);
}

void OSFPanel::drawMenuTitles(cairo_t *cr, int width) {
  cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, 14);
  AresTheme::setCairoColor(cr, AresTheme::StarWhite); // Dark text

  double x = AresTheme::PanelHeight + 16;

  for (size_t i = 0; i < menus_.size(); ++i) {
    auto &menu = menus_[i];

    cairo_text_extents_t ext;
    cairo_text_extents(cr, menu.title.c_str(), &ext);

    // Store position for hit testing
    menu.x = x;
    // Ensure minimal width for hit testing
    menu.width = (ext.width > 10 ? ext.width : 40) + 16; // Padding

    // Highlight if hovered or open
    if ((int)i == hoveredMenuIndex_ || (int)i == openMenuIndex_) {
      cairo_set_source_rgba(cr, 0, 0, 0, 0.1); // Darker highlight
      AresTheme::roundedRect(cr, x - 8, 4, menu.width,
                             AresTheme::PanelHeight - 8, 4);
      cairo_fill(cr);
      AresTheme::setCairoColor(cr, AresTheme::StarWhite);
    }

    // Draw text
    cairo_move_to(cr, x, (AresTheme::PanelHeight + ext.height) / 2.0 - 2);
    cairo_show_text(cr, menu.title.c_str());

    x += menu.width + MenuSpacing;
  }
}

void OSFPanel::drawClock(cairo_t *cr, int width, int height) {
  updateClock();

  cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, 13);
  AresTheme::setCairoColor(cr, AresTheme::StarWhite); // Dark text

  cairo_text_extents_t ext;
  cairo_text_extents(cr, currentClockText_.c_str(), &ext);

  double x = width - ext.width - 24;
  cairo_move_to(cr, x, (AresTheme::PanelHeight + ext.height) / 2.0 - 2);
  cairo_show_text(cr, currentClockText_.c_str());
}

// Restore System Tray (WiFi + Battery)
void OSFPanel::drawSystemTray(cairo_t *cr, int width, int height) {
  cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, 14);
  AresTheme::setCairoColor(cr, AresTheme::StarWhite);

  // WiFi Symbol (Fake for now)
  std::string wifi = " WiFi ";
  cairo_text_extents_t wExt;
  cairo_text_extents(cr, wifi.c_str(), &wExt);

  // Battery Symbol
  std::string bat = " 100% ";
  cairo_text_extents_t bExt;
  cairo_text_extents(cr, bat.c_str(), &bExt);

  // Layout from right: Clock is at width - clockWidth - 16.
  // We need to shift Clock left or draw Tray to the left of Clock.
  // Let's modify drawClock to handle layout or just hardcode offsets for this
  // Hotfix. Actually, drawClock uses 'width' to align right. We should offset
  // the Tray to be left of the Clock. But drawClock renders at absolute
  // position. Let's render Tray at specific offset for now.

  double trayX = width - 300;

  // WiFi Icon
  // Verified Path:
  // resources/icons/WhiteSur/src/status/24/network-wireless-100.svg
  std::string wifiIconPath =
      "resources/icons/WhiteSur/src/status/24/network-wireless-100.svg";
  GError *error = nullptr;
  RsvgHandle *wifiHandle =
      rsvg_handle_new_from_file(wifiIconPath.c_str(), &error);
  /* Deprecated: rsvg_handle_render_cairo
  if (wifiHandle) {
    RsvgDimensionData dim;
    rsvg_handle_get_dimensions(wifiHandle, &dim);

    // Draw centered
    double iconSize = 20;
    double scale = iconSize / std::max(dim.width, dim.height);

    cairo_save(cr);
    cairo_translate(cr, trayX, (AresTheme::PanelHeight - iconSize) / 2);
    cairo_scale(cr, scale, scale);
    rsvg_handle_render_cairo(wifiHandle, cr);
    cairo_restore(cr);
    g_object_unref(wifiHandle);

    trayX += iconSize + 8;
  } else { */
  cairo_move_to(cr, trayX, (AresTheme::PanelHeight + 10) / 2);
  cairo_show_text(cr, "WiFi");
  trayX += 40;
  /* } */

  // Battery Icon
  // Verified Path:
  // resources/icons/WhiteSur/src/status/24/battery-full-charged.svg
  /*
  std::string batIconPath =
      "resources/icons/WhiteSur/src/status/24/battery-full-charged.svg";
  RsvgHandle *batHandle =
      rsvg_handle_new_from_file(batIconPath.c_str(), &error);
  if (batHandle) {
    RsvgDimensionData dim;
    rsvg_handle_get_dimensions(batHandle, &dim);
    double iconSize = 20;
    double scale = iconSize / std::max(dim.width, dim.height);

    cairo_save(cr);
    cairo_translate(cr, trayX, (AresTheme::PanelHeight - iconSize) / 2);
    cairo_scale(cr, scale, scale);
    rsvg_handle_render_cairo(batHandle, cr);
    cairo_restore(cr);
    g_object_unref(batHandle);
  } else { */
  cairo_move_to(cr, trayX, (AresTheme::PanelHeight + 10) / 2);
  cairo_show_text(cr, "100%");
  /* } */
}

void OSFPanel::drawDropdown(cairo_t *cr, int menuIndex) {
  if (menuIndex < 0 || menuIndex >= (int)menus_.size())
    return;

  auto &menu = menus_[menuIndex];

  // Calculate dropdown height
  double dropdownHeight = 8; // Top padding
  for (const auto &item : menu.items) {
    dropdownHeight += item.isSeparator ? SeparatorHeight : DropdownItemHeight;
  }
  dropdownHeight += 8; // Bottom padding

  double dropX = menu.x - 8;
  double dropY = AresTheme::PanelHeight;

  // IMPORTANT: Since the surface is clipped to PanelHeight, this drawing code
  // won't be visible unless we expand the surface. For stability, we keep it
  // clipped for now.

  // Shadow
  cairo_set_source_rgba(cr, 0, 0, 0, 0.3);
  AresTheme::roundedRect(cr, dropX + 2, dropY + 2, DropdownWidth,
                         dropdownHeight, 8);
  cairo_fill(cr);

  // Background
  cairo_set_source_rgba(cr, 0.15, 0.15, 0.15, 0.95); // Dark translucent
  AresTheme::roundedRect(cr, dropX, dropY, DropdownWidth, dropdownHeight, 8);
  cairo_fill(cr);

  // Draw items
  double itemY = dropY + 8;
  for (size_t i = 0; i < menu.items.size(); ++i) {
    const auto &item = menu.items[i];

    if (item.isSeparator) {
      // Draw separator line
      cairo_set_source_rgba(cr, 1, 1, 1, 0.2);
      cairo_move_to(cr, dropX + 12, itemY + SeparatorHeight / 2);
      cairo_line_to(cr, dropX + DropdownWidth - 12,
                    itemY + SeparatorHeight / 2);
      cairo_stroke(cr);
      itemY += SeparatorHeight;
    } else {
      // Highlight if hovered
      if ((int)i == hoveredItemIndex_) {
        AresTheme::setCairoColor(cr, AresTheme::MarsOrange);
        AresTheme::roundedRect(cr, dropX + 4, itemY, DropdownWidth - 8,
                               DropdownItemHeight, 4);
        cairo_fill(cr);
      }

      // Draw text
      cairo_set_source_rgb(cr, 1, 1, 1);
      cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                             CAIRO_FONT_WEIGHT_NORMAL);
      cairo_set_font_size(cr, 13);

      cairo_text_extents_t ext;
      cairo_text_extents(cr, item.label.c_str(), &ext);
      cairo_move_to(cr, dropX + 12,
                    itemY + (DropdownItemHeight + ext.height) / 2.0 - 2);
      cairo_show_text(cr, item.label.c_str());

      itemY += DropdownItemHeight;
    }
  }
}

void OSFPanel::updateClock() {
  auto now = std::time(nullptr);
  auto tm = *std::localtime(&now);

  std::ostringstream oss;
  // Format: "Tuesday,12 10:15 PM" (matches original screenshot)
  oss << std::put_time(&tm, "%A,%d %I:%M %p");
  std::string newText = oss.str();

  if (newText != currentClockText_) {
    currentClockText_ = newText;
    surface_->requestRedraw();
  }
}

int OSFPanel::hitTestMenuTitle(int x, int y) {
  if (y < 0 || y > AresTheme::PanelHeight)
    return -1;

  for (size_t i = 0; i < menus_.size(); ++i) {
    const auto &menu = menus_[i];
    if (x >= menu.x - 8 && x <= menu.x + menu.width) {
      return (int)i;
    }
  }
  return -1;
}

int OSFPanel::hitTestDropdownItem(int x, int y) {
  if (openMenuIndex_ < 0 || openMenuIndex_ >= (int)menus_.size())
    return -1;

  auto &menu = menus_[openMenuIndex_];
  double dropX = menu.x - 8;
  double dropY = AresTheme::PanelHeight;

  if (x < dropX || x > dropX + DropdownWidth)
    return -1;

  double itemY = dropY + 8;
  for (size_t i = 0; i < menu.items.size(); ++i) {
    const auto &item = menu.items[i];
    double itemHeight = item.isSeparator ? SeparatorHeight : DropdownItemHeight;

    if (y >= itemY && y < itemY + itemHeight && !item.isSeparator) {
      return (int)i;
    }
    itemY += itemHeight;
  }
  return -1;
}

void OSFPanel::handlePointerMove(int x, int y) {
  // Input Pass-through Logic:
  // If the pointer is below the panel bar AND no menu is open, we should
  // technically let the click pass through to the wallpaper/window below.
  // However, layer-shell surfaces consume input on their entire surface
  // usually. We can't easily "pass through" clicks on a layer surface in pure
  // DG styling without adjusting the input region. For now, we just handle the
  // menu logic. If menu is closed, clicks below are wasted.
  // TODO: Update input region dynamically based on menu state (Phase 4).

  // Debug log (throttled)
  // std::cout << "Move: " << x << "," << y << std::endl;

  int prevHoveredMenu = hoveredMenuIndex_;
  int prevHoveredItem = hoveredItemIndex_;

  hoveredMenuIndex_ = hitTestMenuTitle(x, y);
  hoveredItemIndex_ = hitTestDropdownItem(x, y);

  // If menu is open and hovering different menu title, switch menus
  if (openMenuIndex_ >= 0 && hoveredMenuIndex_ >= 0 &&
      hoveredMenuIndex_ != openMenuIndex_) {
    openMenuIndex_ = hoveredMenuIndex_;
    std::cout << "[Panel] Switched to menu index: " << openMenuIndex_
              << std::endl;
  }

  if (prevHoveredMenu != hoveredMenuIndex_ ||
      prevHoveredItem != hoveredItemIndex_) {
    surface_->requestRedraw();
  }
}

void OSFPanel::handlePointerDown(int x, int y, uint32_t button) {
  std::cout << "[Panel] Mouse Down at (" << x << ", " << y << ") Btn:" << button
            << std::endl;
  if (button != BTN_LEFT)
    return;

  int menuHit = hitTestMenuTitle(x, y);
  if (menuHit >= 0) {
    // Toggle menu
    if (openMenuIndex_ == menuHit) {
      openMenuIndex_ = -1;
      std::cout << "[Panel] Menu Closed" << std::endl;
    } else {
      openMenuIndex_ = menuHit;
      std::cout << "[Panel] Menu Opened: " << menus_[menuHit].title
                << std::endl;
    }
    surface_->requestRedraw();
    return;
  }

  // Click outside menus closes dropdown
  if (openMenuIndex_ >= 0) {
    int itemHit = hitTestDropdownItem(x, y);
    if (itemHit < 0) {
      openMenuIndex_ = -1;
      std::cout << "[Panel] Clicked outside, closing menu" << std::endl;
      surface_->requestRedraw();
    }
  }
}

void OSFPanel::handlePointerUp(int x, int y, uint32_t button) {
  std::cout << "[Panel] Mouse Up at (" << x << ", " << y << ")" << std::endl;
  if (button != BTN_LEFT)
    return;

  if (openMenuIndex_ >= 0) {
    int itemHit = hitTestDropdownItem(x, y);
    if (itemHit >= 0 && itemHit < (int)menus_[openMenuIndex_].items.size()) {
      auto &item = menus_[openMenuIndex_].items[itemHit];
      std::cout << "[Panel] Clicking Item: " << item.label << std::endl;
      if (item.action && !item.isSeparator) {
        item.action();
      }
      openMenuIndex_ = -1;
      surface_->requestRedraw();
    }
  }
}

void OSFPanel::clearHover() {
  if (hoveredMenuIndex_ >= 0 || hoveredItemIndex_ >= 0) {
    hoveredMenuIndex_ = -1;
    hoveredItemIndex_ = -1;
    surface_->requestRedraw();
  }
}

} // namespace opensef
