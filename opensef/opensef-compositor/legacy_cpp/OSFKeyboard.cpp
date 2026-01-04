/**
 * OSFKeyboard.cpp - Keyboard input handling
 */

#include "OSFKeyboard.h"
#include "OSFCompositor.h"

#include <iostream>
#include <xkbcommon/xkbcommon.h>

extern "C" {
#include <wlr/types/wlr_seat.h>
}

namespace opensef {

#define WL_CONTAINER_OF(ptr, type, member)                                     \
  reinterpret_cast<type *>(reinterpret_cast<char *>(ptr) -                     \
                           offsetof(type, member))

OSFKeyboard::OSFKeyboard(OSFCompositor *compositor, wlr_keyboard *keyboard)
    : compositor_(compositor), keyboard_(keyboard) {

  // Set up keymap
  xkb_context *context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
  xkb_keymap *keymap =
      xkb_keymap_new_from_names(context, nullptr, XKB_KEYMAP_COMPILE_NO_FLAGS);

  wlr_keyboard_set_keymap(keyboard, keymap);
  xkb_keymap_unref(keymap);
  xkb_context_unref(context);

  wlr_keyboard_set_repeat_info(keyboard, 25, 600);

  // Set up event listeners
  modifiers_.notify = handleModifiers;
  wl_signal_add(&keyboard->events.modifiers, &modifiers_);

  key_.notify = handleKey;
  wl_signal_add(&keyboard->events.key, &key_);

  destroy_.notify = handleDestroy;
  wl_signal_add(&keyboard->base.events.destroy, &destroy_);

  wlr_seat_set_keyboard(compositor->seat(), keyboard);
}

OSFKeyboard::~OSFKeyboard() {
  wl_list_remove(&modifiers_.link);
  wl_list_remove(&key_.link);
  wl_list_remove(&destroy_.link);
}

void OSFKeyboard::handleModifiers(wl_listener *listener, void *data) {
  OSFKeyboard *self = WL_CONTAINER_OF(listener, OSFKeyboard, modifiers_);
  (void)data;

  wlr_seat_set_keyboard(self->compositor_->seat(), self->keyboard_);
  wlr_seat_keyboard_notify_modifiers(self->compositor_->seat(),
                                     &self->keyboard_->modifiers);
}

void OSFKeyboard::handleKey(wl_listener *listener, void *data) {
  OSFKeyboard *self = WL_CONTAINER_OF(listener, OSFKeyboard, key_);
  wlr_keyboard_key_event *event = static_cast<wlr_keyboard_key_event *>(data);

  wlr_seat *seat = self->compositor_->seat();
  uint32_t keycode = event->keycode + 8;

  const xkb_keysym_t *syms;
  int nsyms =
      xkb_state_key_get_syms(self->keyboard_->xkb_state, keycode, &syms);

  bool handled = false;
  uint32_t modifiers = wlr_keyboard_get_modifiers(self->keyboard_);

  if ((modifiers & WLR_MODIFIER_ALT) &&
      event->state == WL_KEYBOARD_KEY_STATE_PRESSED) {
    for (int i = 0; i < nsyms; i++) {
      handled = self->handleKeybinding(syms[i]);
      if (handled)
        break;
    }
  }

  if (!handled) {
    wlr_seat_set_keyboard(seat, self->keyboard_);
    wlr_seat_keyboard_notify_key(seat, event->time_msec, event->keycode,
                                 event->state);
  }
}

void OSFKeyboard::handleDestroy(wl_listener *listener, void *data) {
  (void)listener;
  (void)data;
}

bool OSFKeyboard::handleKeybinding(xkb_keysym_t sym) {
  // Alt+Escape = Quit compositor
  if (sym == XKB_KEY_Escape) {
    std::cout << "[openSEF] Alt+Escape pressed - shutting down" << std::endl;
    wl_display_terminate(compositor_->display());
    return true;
  }

  // Alt+F4 = Close focused window (TODO)
  if (sym == XKB_KEY_F4) {
    std::cout << "[openSEF] Alt+F4 pressed - close window (TODO)" << std::endl;
    return true;
  }

  return false;
}

} // namespace opensef
