/**
 * OSFWindow.cpp - Window implementation
 */

#include <cairo/cairo.h>
#include <iostream>
#include <opensef/OpenSEFAppKit.h>


namespace opensef {

OSFWindow::OSFWindow() = default;

OSFWindow::OSFWindow(const std::string &title, const OSFRect &frame)
    : title_(title) {
  frame_ = frame;
}

OSFWindow::~OSFWindow() { close(); }

std::shared_ptr<OSFWindow> OSFWindow::create(const std::string &title,
                                             const OSFRect &frame) {
  return std::make_shared<OSFWindow>(title, frame);
}

void OSFWindow::show() {
  visible_ = true;
  std::cout << "[openSEF] Window '" << title_ << "' shown (" << frame_.width
            << "x" << frame_.height << ")" << std::endl;

  if (!contentView_) {
    return;
  }

  int width = static_cast<int>(frame_.width);
  int height = static_cast<int>(frame_.height);
  if (width <= 0 || height <= 0) {
    return;
  }

  cairo_surface_t *surface =
      cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cairo_t *cr = cairo_create(surface);

  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
  cairo_paint(cr);

  contentView_->render(cr);

  cairo_destroy(cr);
  cairo_surface_destroy(surface);
}

void OSFWindow::close() {
  visible_ = false;
  std::cout << "[openSEF] Window '" << title_ << "' closed" << std::endl;

  // TODO: Destroy Wayland surface
}

void OSFWindow::setContentView(std::shared_ptr<OSFView> view) {
  contentView_ = view;
  if (view) {
    view->setFrame(OSFRect(0, 0, frame_.width, frame_.height));
  }
}

} // namespace opensef
