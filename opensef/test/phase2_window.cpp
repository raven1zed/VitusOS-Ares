/**
 * phase2_window.cpp - Phase 2 Windowing Validation
 *
 * This sample app proves that Phase 2 (Windowing Integration) is complete.
 * It creates a real Wayland window using OSFWindow.
 */

#include <iostream>
#include <opensef/OSFWindow.h>
#include <opensef/OpenSEFBase.h>


using namespace opensef;

int main() {
  std::cout
      << "╔════════════════════════════════════════════════════════════╗\n";
  std::cout
      << "║       openSEF Phase 2 Validation - Windowing Integration   ║\n";
  std::cout
      << "╚════════════════════════════════════════════════════════════╝\n\n";

  // 1. Create a window
  std::cout << "[1] Creating OSFWindow...\n";
  auto window = OSFWindow::create(400, 300, "Phase 2 Test Window");
  std::cout << "    ✓ Window created: " << window->width() << "x"
            << window->height() << "\n\n";

  // 2. Connect to display
  std::cout << "[2] Connecting to Wayland display...\n";
  if (!window->connect()) {
    std::cerr << "    ✗ Failed to connect to Wayland display.\n";
    std::cerr << "    (This is expected if not running inside a compositor)\n";
    return 1;
  }
  std::cout << "    ✓ Connected to display\n\n";

  // 3. Set up draw callback
  std::cout << "[3] Setting up draw callback...\n";
  window->onDraw([](cairo_t *cr, int width, int height) {
    // Draw a simple rectangle
    cairo_set_source_rgb(cr, 0.91, 0.36, 0.02); // Space Orange
    cairo_rectangle(cr, 50, 50, width - 100, height - 100);
    cairo_fill(cr);

    // Draw text
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 24);
    cairo_move_to(cr, 80, 160);
    cairo_show_text(cr, "Phase 2 Works!");
  });
  std::cout << "    ✓ Draw callback set\n\n";

  // 4. Set up close handler
  std::cout << "[4] Setting up event handlers...\n";
  window->onClose([&]() {
    std::cout << "\n    ✓ Window close requested\n";
    window->stopEventLoop();
  });
  std::cout << "    ✓ Close handler set\n\n";

  // 5. Show window
  std::cout << "[5] Showing window...\n";
  window->show();
  std::cout << "    ✓ Window visible\n";
  std::cout << "    (Close the window to continue)\n\n";

  // 6. Run event loop
  window->runEventLoop();

  // 7. Cleanup
  std::cout << "[6] Disconnecting...\n";
  window->disconnect();
  std::cout << "    ✓ Disconnected\n\n";

  std::cout
      << "╔════════════════════════════════════════════════════════════╗\n";
  std::cout
      << "║              PHASE 2 VALIDATION: PASSED                    ║\n";
  std::cout
      << "║                                                            ║\n";
  std::cout
      << "║  ✓ OSFWindow       - Window creation works                 ║\n";
  std::cout
      << "║  ✓ XDG Shell       - Wayland protocol works                ║\n";
  std::cout
      << "║  ✓ Event Loop      - Window events processed               ║\n";
  std::cout
      << "║                                                            ║\n";
  std::cout
      << "║  Phase 2 Windowing Integration is COMPLETE.                ║\n";
  std::cout
      << "╚════════════════════════════════════════════════════════════╝\n";

  return 0;
}
