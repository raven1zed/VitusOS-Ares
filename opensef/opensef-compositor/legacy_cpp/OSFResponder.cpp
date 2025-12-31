/**
 * OSFResponder.cpp - Event Responder Chain Implementation
 */

#include "OSFResponder.h"

namespace opensef {

// Default implementations pass events up the responder chain

void OSFResponder::mouseDown(OSFEvent *event) {
  if (nextResponder_ && !event->isConsumed()) {
    nextResponder_->mouseDown(event);
  }
}

void OSFResponder::mouseUp(OSFEvent *event) {
  if (nextResponder_ && !event->isConsumed()) {
    nextResponder_->mouseUp(event);
  }
}

void OSFResponder::mouseMoved(OSFEvent *event) {
  if (nextResponder_ && !event->isConsumed()) {
    nextResponder_->mouseMoved(event);
  }
}

void OSFResponder::mouseDragged(OSFEvent *event) {
  if (nextResponder_ && !event->isConsumed()) {
    nextResponder_->mouseDragged(event);
  }
}

void OSFResponder::mouseEntered(OSFEvent *event) {
  if (nextResponder_ && !event->isConsumed()) {
    nextResponder_->mouseEntered(event);
  }
}

void OSFResponder::mouseExited(OSFEvent *event) {
  if (nextResponder_ && !event->isConsumed()) {
    nextResponder_->mouseExited(event);
  }
}

void OSFResponder::scrollWheel(OSFEvent *event) {
  if (nextResponder_ && !event->isConsumed()) {
    nextResponder_->scrollWheel(event);
  }
}

void OSFResponder::keyDown(OSFEvent *event) {
  if (nextResponder_ && !event->isConsumed()) {
    nextResponder_->keyDown(event);
  }
}

void OSFResponder::keyUp(OSFEvent *event) {
  if (nextResponder_ && !event->isConsumed()) {
    nextResponder_->keyUp(event);
  }
}

} // namespace opensef
