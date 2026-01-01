#include "OSFDock.h"
#include "OSFAresTheme.h"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <csignal>
#include <fcntl.h>
#include <linux/input-event-codes.h>

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

  // Exclusive zone so windows don't maximize over it
  surface_->setExclusiveZone(AresTheme::DockHeight + AresTheme::DockPadding);

  initWidgets();

  surface_->onDraw([this](cairo_t *cr, int w, int h) { this->draw(cr, w, h); });

  // Bind mouse click
  surface_->onMouseUp([this](int x, int y, uint32_t button) {
      this->onMouseUp(x, y, button);
  });

  surface_->onMouseMove([this](int x, int y) { this->onMouseMove(x, y); });
  surface_->onMouseLeave([this]() { this->clearHover(); });
}

OSFDock::~OSFDock() = default;

void OSFDock::initWidgets() {
    // Add default dock items
    // In a real implementation, we would look for .desktop files or a config.
    // For now, hardcode critical apps.

    struct AppDef {
        std::string name;
        std::string icon;
        std::string cmd;
    };

    std::vector<AppDef> apps = {
        {"Filer", "/usr/share/icons/Adwaita/scalable/apps/system-file-manager-symbolic.svg", "nautilus"},
        {"Terminal", "/usr/share/icons/Adwaita/scalable/apps/utilities-terminal-symbolic.svg", "gnome-terminal"},
        {"Browser", "/usr/share/icons/Adwaita/scalable/apps/web-browser-symbolic.svg", "firefox"},
        {"Settings", "/usr/share/icons/Adwaita/scalable/apps/preferences-system-symbolic.svg", "gnome-control-center"}
    };

    // Note: Icons paths are examples. In the Nix environment, we rely on XDG_DATA_DIRS or specific paths.
    // Since we don't have SVG files guaranteed in a known location, we might fail to load them.
    // If SVG fails, we fallback to colored rects.

    for (const auto& app : apps) {
        DockItem item;
        item.name = app.name;
        item.iconPath = app.icon;
        item.command = app.cmd;

        GError* error = nullptr;
        RsvgHandle* rawHandle = rsvg_handle_new_from_file(item.iconPath.c_str(), &error);
        if (rawHandle) {
             item.svgHandle = std::shared_ptr<RsvgHandle>(rawHandle, [](RsvgHandle* h) {
                 g_object_unref(h);
             });
        } else {
             if (error) {
                 std::cerr << "Warning: Could not load icon for " << app.name << ": " << error->message << std::endl;
                 g_error_free(error);
             }
        }

        items_.push_back(item);
    }
}

void OSFDock::onMouseUp(int x, int y, uint32_t button) {
    if (button != BTN_LEFT) {
        return;
    }

    // Basic hit testing
    for (const auto& item : items_) {
        if (x >= item.x && x <= item.x + item.size &&
            y >= item.y && y <= item.y + item.size) {

            std::cout << "Launching " << item.name << "..." << std::endl;

            pid_t pid = fork();
            if (pid == 0) {
                // Child
                // Redirect standard streams to /dev/null to avoid cluttering the shell output
                // or hanging if the parent process dies.
                setsid(); // Create a new session
                int devnull = open("/dev/null", O_RDWR);
                if (devnull != -1) {
                    dup2(devnull, STDIN_FILENO);
                    dup2(devnull, STDOUT_FILENO);
                    dup2(devnull, STDERR_FILENO);
                    if (devnull > 2) close(devnull);
                }
                execl("/bin/sh", "/bin/sh", "-c", item.command.c_str(), nullptr);
                _exit(1);
            } else if (pid > 0) {
                // Parent: Clean up zombies
                // In a real shell we might want to track PIDs, but here we just want to avoid zombies.
                // Using double-fork is one way, or signal handling.
                // Since we are in a simple loop, let's use a signal handler approach or simpler double fork.
                // Actually, waitpid with WNOHANG is safer here.
                // But since we don't have a main loop here (it's in OSFSurface), we can't poll waitpid easily.
                // Let's rely on signal(SIGCHLD, SIG_IGN) in the constructor.
            }
            return;
        }
    }
}

void OSFDock::onMouseMove(int x, int y) {
    int newHover = -1;
    for (size_t i = 0; i < items_.size(); ++i) {
        const auto& item = items_[i];
        if (x >= item.x && x <= item.x + item.size &&
            y >= item.y && y <= item.y + item.size) {
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

  // 2. Dock Background (Rounded Rect)
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  AresTheme::setCairoColor(cr, AresTheme::DockBackground);
  AresTheme::roundedRect(cr, 0, 0, width, height, AresTheme::DockCornerRadius);
  cairo_fill(cr);

  // 3. Draw Icons
  cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

  int iconSize = 48;
  int spacing = AresTheme::DockItemSpacing;
  int count = items_.size();

  if (count == 0) return;

  double startX = (width - (count * iconSize + (count - 1) * spacing)) / 2.0;
  double drawY = (height - iconSize) / 2.0;

  for (size_t i = 0; i < items_.size(); ++i) {
      auto& item = items_[i];

      // Update hit rect
      item.x = startX;
      item.y = drawY;
      item.size = iconSize;

      if (static_cast<int>(i) == hoveredIndex_) {
          AresTheme::setCairoColor(cr, AresTheme::Nebula);
          AresTheme::roundedRect(cr, startX - 4, drawY - 4, iconSize + 8, iconSize + 8, 14);
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
          // Use a deterministic color based on index
          uint32_t colors[] = {0xFF4A9FD4, 0xFFE57C3A, 0xFF2D2D2D, 0xFF888888, 0xFFD44A4A};
          AresTheme::setCairoColor(cr, colors[i % 5]);
          AresTheme::roundedRect(cr, startX, drawY, iconSize, iconSize, 12);
          cairo_fill(cr);

          // Draw first letter of name
          cairo_set_source_rgb(cr, 1, 1, 1);
          cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
          cairo_set_font_size(cr, 24);

          cairo_text_extents_t ext;
          std::string letter = item.name.substr(0, 1);
          cairo_text_extents(cr, letter.c_str(), &ext);
          cairo_move_to(cr, startX + (iconSize - ext.width)/2 - ext.x_bearing, drawY + (iconSize - ext.height)/2 - ext.y_bearing);
          cairo_show_text(cr, letter.c_str());
      }

      startX += iconSize + spacing;
  }
}

} // namespace opensef
