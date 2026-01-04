/**
 * OSFResponder.h - Responder Chain Base Class
 *
 * Phase 3: Layout + Input + Responder Chain
 *
 * OSFResponder is the base class for all objects that can receive and handle
 * events. It implements a responder chain pattern similar to Cocoa/UIKit.
 *
 * Responder chain traversal:
 *   OSFView -> Parent View -> ... -> OSFWindow -> OSFApplication
 */

#pragma once

#include <memory>

// Forward declarations for event types
typedef struct _cairo cairo_t;

namespace opensef {

// Forward declarations
class OSFEvent;
class OSFResponder;

/**
 * OSFEvent - Base class for all events
 */
class OSFEvent {
public:
  enum class Type {
    Unknown,
    MouseDown,
    MouseUp,
    MouseMoved,
    MouseDragged,
    KeyDown,
    KeyUp,
    ScrollWheel,
    FocusIn,
    FocusOut,
  };

  OSFEvent(Type type = Type::Unknown) : type_(type) {}
  virtual ~OSFEvent() = default;

  Type type() const { return type_; }

  // Mouse event data
  double mouseX() const { return mouseX_; }
  double mouseY() const { return mouseY_; }
  void setMousePosition(double x, double y) {
    mouseX_ = x;
    mouseY_ = y;
  }

  // Keyboard event data
  uint32_t keyCode() const { return keyCode_; }
  void setKeyCode(uint32_t code) { keyCode_ = code; }

  // Modifiers (shift, ctrl, alt, etc.)
  uint32_t modifiers() const { return modifiers_; }
  void setModifiers(uint32_t mods) { modifiers_ = mods; }

  // Event propagation control
  bool isHandled() const { return handled_; }
  void setHandled(bool handled) { handled_ = handled; }

protected:
  Type type_;
  double mouseX_ = 0;
  double mouseY_ = 0;
  uint32_t keyCode_ = 0;
  uint32_t modifiers_ = 0;
  bool handled_ = false;
};

/**
 * OSFResponder - Base class for event handling
 *
 * Any object that can receive events should inherit from OSFResponder.
 * The responder chain allows events to bubble up from child views to parents.
 */
class OSFResponder {
public:
  OSFResponder() = default;
  virtual ~OSFResponder() = default;

  // === Responder Chain ===

  /**
   * Get the next responder in the chain.
   * Default implementation returns nullptr (end of chain).
   * OSFView returns its superview or window.
   * OSFWindow returns OSFApplication.
   */
  virtual OSFResponder *nextResponder() const { return nextResponder_; }

  /**
   * Set the next responder (used during view hierarchy construction)
   */
  void setNextResponder(OSFResponder *responder) { nextResponder_ = responder; }

  // === Event Handling ===

  /**
   * Called when a mouse button is pressed.
   * Return true if the event was handled.
   */
  virtual bool mouseDown(OSFEvent &event) {
    return forwardToNextResponder(event);
  }

  /**
   * Called when a mouse button is released.
   */
  virtual bool mouseUp(OSFEvent &event) {
    return forwardToNextResponder(event);
  }

  /**
   * Called when the mouse moves.
   */
  virtual bool mouseMoved(OSFEvent &event) {
    return forwardToNextResponder(event);
  }

  /**
   * Called when a key is pressed.
   */
  virtual bool keyDown(OSFEvent &event) {
    return forwardToNextResponder(event);
  }

  /**
   * Called when a key is released.
   */
  virtual bool keyUp(OSFEvent &event) { return forwardToNextResponder(event); }

  // === Focus ===

  /**
   * Returns true if this responder can become the first responder (receive
   * keyboard events).
   */
  virtual bool acceptsFirstResponder() const { return false; }

  /**
   * Called when this responder becomes the first responder.
   */
  virtual void becomeFirstResponder() {}

  /**
   * Called when this responder resigns as first responder.
   */
  virtual void resignFirstResponder() {}

protected:
  /**
   * Forward an event to the next responder in the chain.
   */
  bool forwardToNextResponder(OSFEvent &event) {
    if (nextResponder_ && !event.isHandled()) {
      switch (event.type()) {
      case OSFEvent::Type::MouseDown:
        return nextResponder_->mouseDown(event);
      case OSFEvent::Type::MouseUp:
        return nextResponder_->mouseUp(event);
      case OSFEvent::Type::MouseMoved:
        return nextResponder_->mouseMoved(event);
      case OSFEvent::Type::KeyDown:
        return nextResponder_->keyDown(event);
      case OSFEvent::Type::KeyUp:
        return nextResponder_->keyUp(event);
      default:
        return false;
      }
    }
    return false;
  }

private:
  OSFResponder *nextResponder_ = nullptr;
};

} // namespace opensef
