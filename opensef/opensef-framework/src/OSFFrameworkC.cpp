#include "OSFFrameworkC.h"
#include "OSFDesktop.h"
#include "OSFEventBus.h"
#include "OSFStateManager.h"
#include "OSFWindowManager.h"

#include <cstring>
#include <map>

using namespace OpenSEF;

// Internal window tracking
static std::map<std::string, OSFWindow *> window_map;

// Framework lifecycle
void osf_framework_init() {
  auto *desktop = OSFDesktop::shared();
  desktop->initialize();
}

void osf_framework_terminate() {
  auto *desktop = OSFDesktop::shared();
  desktop->terminate();

  // Cleanup windows
  for (auto &pair : window_map) {
    delete pair.second;
  }
  window_map.clear();
}

// Window management
OSFWindowC *osf_window_create(const char *id, const char *title,
                              const char *app_id) {
  auto *window = new OSFWindow(id, title, app_id);
  window_map[id] = window;
  return reinterpret_cast<OSFWindowC *>(window);
}

void osf_window_register(OSFWindowC *window_c) {
  auto *window = reinterpret_cast<OSFWindow *>(window_c);
  auto *desktop = OSFDesktop::shared();
  desktop->windowManager()->registerWindow(window);
}

void osf_window_unregister(const char *id) {
  auto *desktop = OSFDesktop::shared();
  desktop->windowManager()->unregisterWindow(id);

  // Remove from map
  auto it = window_map.find(id);
  if (it != window_map.end()) {
    delete it->second;
    window_map.erase(it);
  }
}

void osf_window_update_title(const char *id, const char *title) {
  auto *desktop = OSFDesktop::shared();
  desktop->windowManager()->updateWindowTitle(id, title);
}

void osf_window_set_geometry(const char *id, int x, int y, int w, int h) {
  auto it = window_map.find(id);
  if (it != window_map.end()) {
    it->second->setGeometry(x, y, w, h);
  }
}

void osf_window_destroy(OSFWindowC *window_c) {
  auto *window = reinterpret_cast<OSFWindow *>(window_c);
  osf_window_unregister(window->id().c_str());
}

// Window actions
void osf_window_focus(const char *id) {
  auto *desktop = OSFDesktop::shared();
  desktop->windowManager()->focusWindow(id);
}

void osf_window_minimize(const char *id) {
  auto *desktop = OSFDesktop::shared();
  desktop->windowManager()->minimizeWindow(id);
}

void osf_window_maximize(const char *id) {
  auto *desktop = OSFDesktop::shared();
  desktop->windowManager()->maximizeWindow(id);
}

void osf_window_close(const char *id) {
  auto *desktop = OSFDesktop::shared();
  desktop->windowManager()->closeWindow(id);
}

// Event publishing
void osf_event_publish(const char *event_type, const char *data) {
  auto *desktop = OSFDesktop::shared();
  OSFEvent event;
  event.set("data", std::string(data));
  desktop->eventBus()->publish(event_type, event);
}
