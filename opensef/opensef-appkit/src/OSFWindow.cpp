/**
 * OSFWindow.cpp - Window implementation
 */

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

  // TODO: Create Wayland surface and show
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
