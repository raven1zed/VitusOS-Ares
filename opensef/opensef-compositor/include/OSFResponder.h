/**
 * OSFResponder.h - Event Responder Chain
 *
 * Based on GNUStep NSResponder pattern from R&D research.
 * Implements the chain of responsibility for event handling.
 *
 * Event flow: View → Superview → ... → Window → Application
 * If a responder doesn't handle an event, it passes to nextResponder.
 */

#pragma once

#include <cstdint>
#include <memory>

namespace opensef {

// Forward declarations
class OSFEvent;

/**
 * OSFResponder - Base class for objects that handle events
 */
class OSFResponder {
public:
  virtual ~OSFResponder() = default;

  // === Mouse Events ===

  virtual void mouseDown(OSFEvent *event);
  virtual void mouseUp(OSFEvent *event);
  virtual void mouseMoved(OSFEvent *event);
  virtual void mouseDragged(OSFEvent *event);
  virtual void mouseEntered(OSFEvent *event);
  virtual void mouseExited(OSFEvent *event);
  virtual void scrollWheel(OSFEvent *event);

  // === Keyboard Events ===

  virtual void keyDown(OSFEvent *event);
  virtual void keyUp(OSFEvent *event);

  // === Focus ===

  virtual bool acceptsFirstResponder() const { return false; }
  virtual bool becomeFirstResponder() { return true; }
  virtual bool resignFirstResponder() { return true; }

  // === Responder Chain ===

  void setNextResponder(OSFResponder *next) { nextResponder_ = next; }
  OSFResponder *nextResponder() const { return nextResponder_; }

protected:
  OSFResponder *nextResponder_ = nullptr;
};

/**
 * OSFEvent - Input event data
 */
class OSFEvent {
public:
  enum class Type {
    None,
    // Mouse
    MouseDown,
    MouseUp,
    MouseMoved,
    MouseDragged,
    MouseEntered,
    MouseExited,
    ScrollWheel,
    // Keyboard
    KeyDown,
    KeyUp
  };

  enum class MouseButton { None = 0, Left = 1, Right = 2, Middle = 3 };

  Type type = Type::None;

  // Mouse data
  float x = 0.0f;
  float y = 0.0f;
  float deltaX = 0.0f;
  float deltaY = 0.0f;
  MouseButton button = MouseButton::None;

  // Keyboard data
  uint32_t keycode = 0;
  uint32_t modifiers = 0;

  // Timing
  float timestamp = 0.0f;

  // Event consumption
  bool isConsumed() const { return consumed_; }
  void consume() { consumed_ = true; }

private:
  bool consumed_ = false;
};

} // namespace opensef
