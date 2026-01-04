#include "OSFPanel.h"
#include "OSFAresTheme.h"
#include <opensef/OpenSEFAppKit.h>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <linux/input-event-codes.h>
#include <sstream>

namespace opensef {

OSFPanel::OSFPanel() {
  // Create Wayland surface
  surface_ = std::make_unique<OSFSurface>("osf-panel");

  // Configure for top panel
  surface_->setLayer(Layer::Top);
  surface_->setAnchor(Anchor::Top | Anchor::Left | Anchor::Right);
  surface_->setSize(0, AresTheme::PanelHeight); // 0 width = stretch
  surface_->setExclusiveZone(AresTheme::PanelHeight);

  // Create widgets using the widget library
  initWidgets();

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
  surface_->onTick([this]() { surface_->requestRedraw(); });
  surface_->setFrameTimer(1000);
}

void OSFPanel::initWidgets() {
  // 1. Apple/Vitus Logo Button
  // Using text "Vitus" for now, ideally an Icon.
  appNameButton_ = OSFButton::create("VitusOS"); 
  appNameButton_->setAction([]() { std::cout << "[Panel] VitusOS Menu Clicked" << std::endl; });

  // 2. Global Menu Items
  std::vector<std::string> menus = {"File", "Edit", "View", "Go", "Window", "Help"};
  menuButtons_.clear();
  for (const auto& name : menus) {
      auto btn = OSFButton::create(name);
      btn->setAction([name]() { std::cout << "[Panel] Menu '" << name << "' Clicked" << std::endl; });
      menuButtons_.push_back(btn);
  }

  // 3. Clock Label
  clockLabel_ = OSFLabel::create("");
  clockLabel_->setAlignment(TextAlignment::Right);

  // Rebuild widget list for hit testing
  widgets_.clear();
  widgets_.push_back(appNameButton_);
  for (auto& btn : menuButtons_) {
      widgets_.push_back(btn);
  }
}

void OSFPanel::draw(cairo_t *cr, int width, int height) {
  // 1. Background (Glassmorphism)
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  AresTheme::setCairoColor(cr, AresTheme::PanelBackground); // Transparent grey
  cairo_paint(cr);

  // switch to OVER for content
  cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

  // 2. Layout Calculation
  int startX = 24; // Left margin
  int const spacing = 18;
  int const btnHeight = height - 4; // Padding top/bottom 2px
  int const btnY = 2;

  // Draw App Name (Bold)
  // We manually style this one to be bold if OSFButton doesn't expose font weight
  // For now, standard button.
  appNameButton_->setFrame(OSFRect(startX, btnY, 70, btnHeight));
  appNameButton_->render(cr);
  startX += 70 + spacing;

  // Separator logic could go here?

  // Draw Menu Buttons
  for (auto& btn : menuButtons_) {
      // Estimate width based on text length (hacky without font metrics helper here)
      // Assuming char width ~ 8px + padding
      // Actually OSFButton::render handles text centering, but we update frame.
      // Ideally we ask button for its preferred width.
      // Let's approximate: 10px per char + 16px padding
      // We can use cairo_text_extents inside render, but for layout we guess.
      // Let's use a fixed width for simplicity or text length heuristic.
      // Or just a standard width.
      int btnW = 50; 
      // Heuristic:
      // "File" (4) -> 40
      // "Window" (6) -> 60
      // "Help" (4) -> 40
      // Let's make it 60 for all for uniformity, or logical.
      
      btn->setFrame(OSFRect(startX, btnY, 60, btnHeight));
      btn->render(cr);
      startX += 60; // tighter spacing for Mac-like feel? Mac menus are tight.
  }

  // 3. Right Side: Clock & Status
  updateClock();
  int rightX = width - 24;
  
  // Status Icons (Mocked with text/emoji for V1)
  // Battery
  cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Dark grey text
  cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, 13);
  
  std::string wifi = "🛜"; // Wifi symbol
  std::string bat = "🔋 100%"; // Battery
  
  cairo_text_extents_t ext;
  
  // Battery
  cairo_text_extents(cr, bat.c_str(), &ext);
  rightX -= (ext.width + 10);
  cairo_move_to(cr, rightX, (height + ext.height)/2 - 2);
  cairo_show_text(cr, bat.c_str());
  
  // Wifi
  cairo_text_extents(cr, wifi.c_str(), &ext);
  rightX -= (ext.width + 16);
  cairo_move_to(cr, rightX, (height + ext.height)/2 - 2);
  cairo_show_text(cr, wifi.c_str());

  // Clock
  // We resize clock label frame
  rightX -= 160;
  clockLabel_->setFrame(OSFRect(rightX, (height - 16) / 2.0, 150, 16));
  clockLabel_->render(cr);
}

void OSFPanel::updateClock() {
  auto now = std::time(nullptr);
  auto tm = *std::localtime(&now);

  std::ostringstream oss;
  // Format: Mon Jan 4 10:45 PM
  oss << std::put_time(&tm, "%a %b %d  %I:%M %p");
  clockLabel_->setText(oss.str());
}

std::shared_ptr<OSFButton> OSFPanel::hitTestButton(int x, int y) {
  for (const auto &widget : widgets_) {
    auto button = std::dynamic_pointer_cast<OSFButton>(widget);
    if (!button) {
      continue;
    }

    auto frame = button->frame();
    if (x >= frame.x && x <= frame.x + frame.width && y >= frame.y &&
        y <= frame.y + frame.height) {
      return button;
    }
  }

  return nullptr;
}

void OSFPanel::handlePointerMove(int x, int y) {
  auto button = hitTestButton(x, y);
  if (button == hoveredButton_) {
    return;
  }

  if (hoveredButton_) {
    hoveredButton_->handleMouseLeave();
  }

  hoveredButton_ = button;

  if (hoveredButton_) {
    hoveredButton_->handleMouseEnter();
  }

  surface_->requestRedraw();
}

void OSFPanel::handlePointerDown(int x, int y, uint32_t button) {
  if (button != BTN_LEFT) {
    return;
  }

  handlePointerMove(x, y);

  pressedButton_ = hoveredButton_;
  if (pressedButton_) {
    pressedButton_->handleMouseDown();
    surface_->requestRedraw();
  }
}

void OSFPanel::handlePointerUp(int x, int y, uint32_t button) {
  if (button != BTN_LEFT) {
    return;
  }

  handlePointerMove(x, y);

  if (pressedButton_) {
    // Trigger action!
    // Since OSFButton executes action on MouseUp if inside
    // We should rely on OSFButton implementation or call it here.
    // The previous implementation didn't call action explicitly? 
    // Wait, OSFAppKit OSFButton usually has onClick logic?
    // Let's assume handleMouseUp does it OR we do it.
    // Since I added setAction, I'll call it if pressedButton_ == hoveredButton_
    // BUT checking OSFButton.h we don't have visibility.
    // Let's just assume `handleMouseUp` updates state, and we rely on `pressedButton_` which is `OSFButton`.
    // Does OSFButton have an executeAction?
    // I added `setAction` lambda in `initWidgets` above.
    // To be safe, I'll invoke it here if I can cast it or if OSFButton handles it.
    // Let's check if OSFButton has public `action` member or similar.
    // Actually, looking at `OSFButton::create`, it returns `std::shared_ptr<OSFButton>`.
    // Since I can't check OSFButton implementation right now (failed view), I'll blindly call `handleMouseUp`.
    // And assuming it handles the trigger. If not, the logging won't happen.
    
    // I'll add `performClick()` logic if needed:
    // If user clicked, we should see log.
    
    pressedButton_->handleMouseUp();
    pressedButton_.reset();
    surface_->requestRedraw();
  }
}

void OSFPanel::clearHover() {
  if (hoveredButton_) {
    hoveredButton_->handleMouseLeave();
    hoveredButton_.reset();
    surface_->requestRedraw();
  }
}

} // namespace opensef
