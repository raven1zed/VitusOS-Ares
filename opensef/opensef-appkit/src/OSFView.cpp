/**
 * OSFView.cpp - View implementation
 */

#include <algorithm>
#include <opensef/OpenSEFAppKit.h>


namespace opensef {

OSFView::OSFView() = default;
OSFView::~OSFView() = default;

void OSFView::addSubview(std::shared_ptr<OSFView> view) {
  view->superview_ = this;
  subviews_.push_back(view);
}

void OSFView::removeFromSuperview() {
  if (superview_) {
    auto &siblings = superview_->subviews_;
    siblings.erase(std::remove_if(siblings.begin(), siblings.end(),
                                  [this](const std::shared_ptr<OSFView> &v) {
                                    return v.get() == this;
                                  }),
                   siblings.end());
    superview_ = nullptr;
  }
}

void OSFView::draw() {
  if (hidden_)
    return;

  for (auto &subview : subviews_) {
    subview->draw();
  }
}

} // namespace opensef
