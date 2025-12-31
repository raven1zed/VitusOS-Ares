/**
 * OSFView.cpp - Base View Implementation
 */

#include <algorithm>
#include <opensef/OpenSEFAppKit.h>


namespace opensef {

void OSFView::addSubview(std::shared_ptr<OSFView> view) {
  if (view->superview_) {
    view->removeFromSuperview();
  }
  view->superview_ = this;
  subviews_.push_back(view);
}

void OSFView::removeFromSuperview() {
  if (!superview_)
    return;

  auto &siblings = superview_->subviews_;
  siblings.erase(std::remove_if(siblings.begin(), siblings.end(),
                                [this](const std::shared_ptr<OSFView> &v) {
                                  return v.get() == this;
                                }),
                 siblings.end());
  superview_ = nullptr;
}

void OSFView::render(cairo_t *cr) {
  if (hidden_)
    return;

  // Render subviews
  for (auto &subview : subviews_) {
    subview->render(cr);
  }
}

} // namespace opensef
