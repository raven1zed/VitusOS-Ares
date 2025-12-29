/**
 * OSFCursor.h - Cursor handling
 */

#pragma once

extern "C" {
#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_xcursor_manager.h>
}

namespace opensef {

// Cursor modes
enum class OSFCursorMode {
  PASSTHROUGH, // Normal mode
  MOVE,        // Window being dragged
  RESIZE       // Window being resized
};

} // namespace opensef
