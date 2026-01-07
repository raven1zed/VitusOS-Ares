/**
 * OSFPasswordField.h - Minimal password input widget for lock screen
 */

#pragma once

#include <functional>
#include <opensef/OSFView.h>
#include <string>


namespace opensef {

class OSFPasswordField : public OSFView {
public:
  OSFPasswordField();
  ~OSFPasswordField() override = default;

  // Get/set password value
  std::string value() const { return value_; }
  void setValue(const std::string &value);
  void clear();

  // Placeholder text
  void setPlaceholder(const std::string &placeholder) {
    placeholder_ = placeholder;
  }

  // Callbacks
  void onSubmit(std::function<void(const std::string &)> callback) {
    submitCallback_ = callback;
  }
  void onError(std::function<void()> callback) { errorCallback_ = callback; }

  // Visual states
  void setError(bool error);
  void startShakeAnimation();

  // OSFView overrides
  void render(cairo_t *cr) override;
  bool keyDown(OSFEvent &event) override;
  bool acceptsFirstResponder() const override { return true; }

private:
  std::string value_;
  std::string placeholder_;
  bool focused_ = false;
  bool error_ = false;
  double shakeOffset_ = 0.0;

  std::function<void(const std::string &)> submitCallback_;
  std::function<void()> errorCallback_;
};

} // namespace opensef
