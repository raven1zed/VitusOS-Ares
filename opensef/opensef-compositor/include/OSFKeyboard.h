/**
 * OSFKeyboard.h - Keyboard input handling
 */

#pragma once

extern "C" {
#include <wlr/types/wlr_keyboard.h>
}

#include <wayland-server-core.h>

namespace opensef {

class OSFCompositor;

/**
 * OSFKeyboard - Keyboard input device
 */
class OSFKeyboard {
public:
  OSFKeyboard(OSFCompositor *compositor, wlr_keyboard *keyboard);
  ~OSFKeyboard();

  wlr_keyboard *keyboard() const { return keyboard_; }

private:
  OSFCompositor *compositor_;
  wlr_keyboard *keyboard_;

  wl_listener modifiers_;
  wl_listener key_;
  wl_listener destroy_;

  static void handleModifiers(wl_listener *listener, void *data);
  static void handleKey(wl_listener *listener, void *data);
  static void handleDestroy(wl_listener *listener, void *data);

  bool handleKeybinding(xkb_keysym_t sym);
};

} // namespace opensef
