/**
 * phase3_app.cpp - Phase 3 Validation: REAL Unified Event Loop
 *
 * This sample app proves that Phase 3 works:
 * - OSFApplication::run() is the SINGLE entry point
 * - Windows register with the app and are polled together
 * - Responder chain is active (Clicking button works!)
 */

#include <cairo/cairo.h>
#include <iostream>
#include <opensef/OSFStackView.h>
#include <opensef/OSFWindow.h>
#include <opensef/OpenSEFAppKit.h> // Triggers dependency on OSFView/Geometry
#include <opensef/OpenSEFBase.h>

using namespace opensef;

int main() {
  std::cout
      << "╔════════════════════════════════════════════════════════════╗\n";
  std::cout
      << "║       openSEF Phase 3 - REAL Unified Event Loop            ║\n";
  std::cout
      << "╚════════════════════════════════════════════════════════════╝\n\n";

  auto &app = OSFApplication::shared();

  app.setOnLaunch(
      []() { std::cout << "    [App] OSFApplication::onLaunch() called\n"; });

  app.setOnTerminate([]() {
    std::cout << "    [App] OSFApplication::onTerminate() called\n";
  });

  // === Create Window ===
  auto window = OSFWindow::create(800, 600, "Phase 3 - Unified Event Loop");

  // === Connect to Display ===
  if (!window->connect()) {
    std::cerr << "    [Error] Failed to connect to Wayland display.\n";
    return 1;
  }

  // === Setup UI (AppKit) ===
  // Root View (Background)
  auto rootView = std::make_shared<OSFView>();
  rootView->setFrame(OSFRect(0, 0, 800, 600));

  // === Phase 3 NEW: OSFStackView Layout Demo ===
  // Create a vertical stack for buttons
  auto buttonStack = OSFStackView::create(OSFStackAxis::Vertical);
  buttonStack->setFrame(OSFRect(50, 200, 300, 300));
  buttonStack->setSpacing(16);
  buttonStack->setAlignment(OSFStackAlignment::Fill);
  buttonStack->setDistribution(OSFStackDistribution::EqualSpacing);
  buttonStack->setPadding(20, 20, 20, 20);

  // Button 1
  auto button = OSFButton::create("Click Me!", []() {
    std::cout << "\n    >>> BUTTON 1 CLICKED! Responder Chain Works! <<<\n\n";
  });
  buttonStack->addSubview(button);

  // Button 2
  auto button2 = OSFButton::create(
      "Button Two", []() { std::cout << "    >> Button Two clicked\n"; });
  buttonStack->addSubview(button2);

  // Button 3 (Exit)
  auto exitButton = OSFButton::create("Exit App", [&]() {
    std::cout << "    [App] Exit requested via button.\n";
    app.stop();
  });
  buttonStack->addSubview(exitButton);

  // Add stack to root
  rootView->addSubview(buttonStack);

  // === Horizontal Stack Demo ===
  auto hStack = OSFStackView::create(OSFStackAxis::Horizontal);
  hStack->setFrame(OSFRect(400, 200, 350, 50));
  hStack->setSpacing(8);

  auto btn_a = OSFButton::create("A", []() { std::cout << "A\n"; });
  auto btn_b = OSFButton::create("B", []() { std::cout << "B\n"; });
  auto btn_c = OSFButton::create("C", []() { std::cout << "C\n"; });
  hStack->addSubview(btn_a);
  hStack->addSubview(btn_b);
  hStack->addSubview(btn_c);
  rootView->addSubview(hStack);

  // Set Content View
  window->setContentView(rootView);
  std::cout << "    [Setup] View hierarchy with OSFStackView created.\n";

  // === Draw Callback (Integration) ===
  // AppKit views render themselves, but we clear the window first.
  window->onDraw([rootView](cairo_t *cr, int width, int height) {
    // 1. Draw Background (Mars Gradient)
    cairo_pattern_t *gradient =
        cairo_pattern_create_linear(0, 0, width, height);
    cairo_pattern_add_color_stop_rgb(gradient, 0.0, 0.91, 0.36, 0.02);
    cairo_pattern_add_color_stop_rgb(gradient, 0.5, 0.83, 0.50, 0.10);
    cairo_pattern_add_color_stop_rgb(gradient, 1.0, 0.20, 0.12, 0.08);
    cairo_set_source(cr, gradient);
    cairo_paint(cr);
    cairo_pattern_destroy(gradient);

    // 2. Render Overlay Text
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 36);
    cairo_move_to(cr, 50, 70);
    cairo_show_text(cr, "Phase 3 Complete!");

    cairo_set_font_size(cr, 18);
    cairo_move_to(cr, 50, 110);
    cairo_show_text(cr, "Interactive Controls: Click the buttons below.");
  });

  // === Show Window ===
  window->show();

  // === RUN THE APPLICATION ===
  std::cout << "    [Run] Starting Unified Event Loop...\n";
  app.run();

  window->disconnect();
  return 0;
}
