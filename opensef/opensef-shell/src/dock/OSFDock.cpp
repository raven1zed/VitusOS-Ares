#include "OSFDock.h"
#include "OSFAresTheme.h"
#include <csignal>
#include <fcntl.h>
#include <iostream>
#include <linux/input-event-codes.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace opensef {

OSFDock::OSFDock() {
  // Prevent zombie processes by ignoring SIGCHLD
  signal(SIGCHLD, SIG_IGN);

  surface_ = std::make_unique<OSFSurface>("osf-dock");

  // Bottom Center Dock
  surface_->setLayer(Layer::Top);
  surface_->setAnchor(Anchor::Bottom);

  // Fixed width for now (V1), will implement dynamic sizing later
  int dockWidth = 600; // Wider to accommodate more spacing
  surface_->setSize(dockWidth, AresTheme::DockHeight);

  // Margin from bottom
  surface_->setMargin(0, 0, AresTheme::DockPadding, 0);

  // Exclusive zone 0 so wallpaper renders behind it (Floating Dock)
  surface_->setExclusiveZone(0);

  initWidgets();

  surface_->onDraw([this](cairo_t *cr, int w, int h) { this->draw(cr, w, h); });

  // Bind mouse click
  surface_->onMouseUp(
      [this](int x, int y, uint32_t button) { this->onMouseUp(x, y, button); });

  surface_->onMouseMove([this](int x, int y) { this->onMouseMove(x, y); });
  surface_->onMouseLeave([this]() { this->clearHover(); });

  // Framework Integration
  desktop_ = OpenSEF::OSFDesktop::shared();
  subscribeToEvents();
}

void OSFDock::subscribeToEvents() {
  auto *eventBus = desktop_->eventBus();

  eventBus->subscribe(
      "application.launched",
      [this](const OpenSEF::OSFEvent &event) { onAppLaunched(event); });

  eventBus->subscribe(
      "application.closed",
      [this](const OpenSEF::OSFEvent &event) { onAppClosed(event); });
}

void OSFDock::onAppLaunched(const OpenSEF::OSFEvent &event) {
  updateRunningApps();
  surface_->requestRedraw();
}

void OSFDock::onAppClosed(const OpenSEF::OSFEvent &event) {
  updateRunningApps();
  surface_->requestRedraw();
}

void OSFDock::updateRunningApps() {
  // Determine running status
  // For V1, we rely on event updates or polling StateManager
  // Currently StateManager tracks windows, not full app lifecycle yet
  // So we'll implement a basic check or just log for now
  std::cout << "[Dock] Updating running apps list..." << std::endl;
}

OSFDock::~OSFDock() = default;

void OSFDock::initWidgets() {
  struct AppDef {
    std::string name;
    std::string icon;
    std::string cmd;
    uint32_t color; // Fallback color
  };

  // Updated list per user design: Filer, Store, Discord, Facebook, Spotify,
  // Video
  std::vector<AppDef> apps = {
      {"Filer",
       "resources/icons/WhiteSur/alternative/apps/scalable/file-manager.svg",
       "./build/opensef/apps/osf-filer/osf-filer", 0xFF333333},
      {"Store", "resources/icons/WhiteSur/links/apps/scalable/Appstore.svg", "",
       0xFF333333},
      {"Discord", "resources/icons/WhiteSur/src/apps/scalable/discord.svg", "",
       0xFF333333},
      {"Facebook",
       "resources/icons/WhiteSur/src/apps/scalable/web-facebook.svg", "",
       0xFF333333},
      {"Spotify",
       "resources/icons/WhiteSur/src/apps/scalable/spotify-client.svg", "",
       0xFF333333},
      {"Video", "resources/icons/WhiteSur/src/apps/scalable/youtube.svg", "",
       0xFF333333},
      {"Terminal",
       "resources/icons/WhiteSur/alternative/apps/scalable/terminal.svg",
       "./build/opensef/apps/osf-terminal/osf-terminal", 0xFF333333},
      {"Settings",
       "resources/icons/WhiteSur/src/apps/scalable/preferences-system.svg",
       "./build/opensef/apps/osf-settings/osf-settings", 0xFF333333}};

  int colorIdx = 0;
  for (const auto &app : apps) {
    DockItem item;
    item.name = app.name;
    item.iconPath = app.icon;
    item.command = app.cmd;
    item.fallbackColor = app.color; // Custom field I need to add to DockItem
                                    // struct in header or handle here locally?
    // Wait, DockItem is private struct in OSFDock.h probably?
    // I will just use the color array in draw() but mapping is better.
    // Let's rely on the draw() fallback logic for now, but update the list.

    GError *error = nullptr;
    RsvgHandle *rawHandle =
        rsvg_handle_new_from_file(item.iconPath.c_str(), &error);
    if (rawHandle) {
      item.svgHandle = std::shared_ptr<RsvgHandle>(
          rawHandle, [](RsvgHandle *h) { g_object_unref(h); });
    }

    items_.push_back(item);
  }
}

void OSFDock::onMouseUp(int x, int y, uint32_t button) {
  if (button != BTN_LEFT) {
    return;
  }

  // Basic hit testing
  for (const auto &item : items_) {
    if (x >= item.x && x <= item.x + item.size && y >= item.y &&
        y <= item.y + item.size) {

      std::cout << "Launching " << item.name << "..." << std::endl;

      pid_t pid = fork();
      if (pid == 0) {
        // Child
        // Redirect standard streams to /dev/null to avoid cluttering the shell
        // output or hanging if the parent process dies.
        setsid(); // Create a new session
        int devnull = open("/dev/null", O_RDWR);
        if (devnull != -1) {
          dup2(devnull, STDIN_FILENO);
          dup2(devnull, STDOUT_FILENO);
          dup2(devnull, STDERR_FILENO);
          if (devnull > 2)
            close(devnull);
        }
        execl("/bin/sh", "/bin/sh", "-c", item.command.c_str(), nullptr);
        _exit(1);
      } else if (pid > 0) {
        // Parent: Clean up zombies
        // In a real shell we might want to track PIDs, but here we just want to
        // avoid zombies. Using double-fork is one way, or signal handling.
        // Since we are in a simple loop, let's use a signal handler approach or
        // simpler double fork. Actually, waitpid with WNOHANG is safer here.
        // But since we don't have a main loop here (it's in OSFSurface), we
        // can't poll waitpid easily. Let's rely on signal(SIGCHLD, SIG_IGN) in
        // the constructor.
      }
      return;
    }
  }
}

void OSFDock::onMouseMove(int x, int y) {
  int newHover = -1;
  for (size_t i = 0; i < items_.size(); ++i) {
    const auto &item = items_[i];
    if (x >= item.x && x <= item.x + item.size && y >= item.y &&
        y <= item.y + item.size) {
      newHover = static_cast<int>(i);
      break;
    }
  }

  if (newHover != hoveredIndex_) {
    hoveredIndex_ = newHover;
    surface_->requestRedraw();
  }
}

void OSFDock::clearHover() {
  if (hoveredIndex_ != -1) {
    hoveredIndex_ = -1;
    surface_->requestRedraw();
  }
}

void OSFDock::run() {
  if (surface_->connect()) {
    surface_->run();
  } else {
    std::cerr << "Failed to connect OSFDock to display." << std::endl;
  }
}

void OSFDock::draw(cairo_t *cr, int width, int height) {
  // 1. Clear transparent
  cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
  cairo_paint(cr);

  // 2. Dock Background (Rounded Rect - White Translucent per Desktop.png)
  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0,
                        0.95); // Nearly opaque for visibility check
  AresTheme::roundedRect(cr, 0, 0, width, height, AresTheme::DockCornerRadius);
  cairo_fill(cr);

  // 3. Draw Icons
  cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

  int iconSize = 48;
  int spacing = AresTheme::DockItemSpacing;
  int count = items_.size();

  if (count == 0)
    return;

  double startX = (width - (count * iconSize + (count - 1) * spacing)) / 2.0;
  double drawY = (height - iconSize) / 2.0;

  for (size_t i = 0; i < items_.size(); ++i) {
    auto &item = items_[i];

    // Update hit rect
    item.x = startX;
    item.y = drawY;
    item.size = iconSize;

    if (static_cast<int>(i) == hoveredIndex_) {
      AresTheme::setCairoColor(cr, AresTheme::Nebula);
      AresTheme::roundedRect(cr, startX - 4, drawY - 4, iconSize + 8,
                             iconSize + 8, 14);
      cairo_fill(cr);
    }

    if (item.svgHandle) {
      // Render SVG
      cairo_save(cr);
      cairo_translate(cr, startX, drawY);

      // Scale SVG to fit iconSize
      RsvgDimensionData dim;
      rsvg_handle_get_dimensions(item.svgHandle.get(), &dim);
      double scaleX = (double)iconSize / dim.width;
      double scaleY = (double)iconSize / dim.height;
      double scale = std::min(scaleX, scaleY);

      cairo_scale(cr, scale, scale);
      rsvg_handle_render_cairo(item.svgHandle.get(), cr);
      cairo_restore(cr);
    } else {
      // Fallback to colored rect if no icon
      AresTheme::setCairoColor(cr, item.fallbackColor);
      AresTheme::roundedRect(cr, startX, drawY, iconSize, iconSize, 12);
      cairo_fill(cr);

      // Draw first letter of name
      cairo_set_source_rgb(cr, 1, 1, 1);
      cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                             CAIRO_FONT_WEIGHT_BOLD);
      cairo_set_font_size(cr, 24);

      cairo_text_extents_t ext;
      std::string letter = item.name.substr(0, 1);
      cairo_text_extents(cr, letter.c_str(), &ext);
      cairo_move_to(cr, startX + (iconSize - ext.width) / 2 - ext.x_bearing,
                    drawY + (iconSize - ext.height) / 2 - ext.y_bearing);
      cairo_show_text(cr, letter.c_str());
    }

    startX += iconSize + spacing;
  }
}

} // namespace opensef
