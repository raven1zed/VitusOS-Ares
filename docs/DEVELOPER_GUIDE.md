# VitusOS Ares Developer Guide

**A comprehensive guide to understanding and contributing to VitusOS Ares**

*Last Updated: January 1, 2026*

---

## Table of Contents

1. [Project Overview](#project-overview)
2. [Architecture](#architecture)
3. [Getting Started](#getting-started)
4. [Compositor (opensef-compositor)](#compositor-opensef-compositor)
5. [UI Shell (opensef-shell)](#ui-shell-opensef-shell)
6. [Widget Library (opensef-appkit)](#widget-library-opensef-appkit)
7. [Design System (Ares Theme)](#design-system-ares-theme)
8. [Building & Testing](#building--testing)
9. [Contributing](#contributing)
10. [Troubleshooting](#troubleshooting)

---

## Project Overview

VitusOS Ares is a custom Linux desktop environment with a unique **hybrid C/C++ architecture**:

- **Purpose**: Create a warm, polished desktop inspired by OS1 (Her), macOS Aqua, and Mars aesthetics
- **Display Server**: Wayland (via wlroots 0.19)
- **Rendering**: Cairo/Pango for 2D UI, scene graph for compositing
- **Target**: NixOS (primary), other Linux distributions (secondary)

### ⚠️ Critical Context: What is openSEF?

> **openSEF is to VitusOS what Cocoa is to macOS.**

openSEF serves **two purposes**:

1. **GUI Framework** — Like Cocoa, Qt, or GTK. Native apps are built with openSEF widgets.
2. **Desktop Environment** — Compositor, panel, dock, greeter, installer, system apps.

**The End Goal:**
- Users **never see** GRUB, kernel messages, systemd logs, or terminal commands
- From power button: Boot animation → Lock screen → Desktop → Shutdown animation
- systemd still exists, but openSEF makes it invisible
- The UI design is iterating, but the foundation must be solid and maintainable

**This is not about competing with Apple.** It's about giving Linux the polish it deserves.

### Why Hybrid C/C++?

The wlroots library uses C99-only syntax (`[static 4]` array parameters) that **cannot compile as C++**. After extensive research, we adopted the same pattern used by production compositors:

| Component | Language | Reason |
|-----------|----------|--------|
| Compositor Core | Pure C | wlroots compatibility |
| UI Shell | C++ | Modern features, Cairo bindings |
| Widget Library | C++ | OOP design patterns |

---

## Architecture

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                           VitusOS Ares Desktop                               │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                              │
│  ┌──────────────────────────────────────────────────────────────────────┐   │
│  │                    C++ Shell Layer                                    │   │
│  │                                                                       │   │
│  │   opensef-appkit      opensef-core        opensef-shell              │   │
│  │   ├─ OSFButton        ├─ Animation        ├─ OSFSurface (render)     │   │
│  │   ├─ OSFLabel         └─ OSFLayer         ├─ OSFPanel   (top bar)    │   │
│  │   ├─ OSFTextField                         └─ OSFDock    (bottom)     │   │
│  │   └─ OSFGlassPanel                                                    │   │
│  │                                                                       │   │
│  │   Connects via: wlr-layer-shell-unstable-v1 protocol                 │   │
│  └────────────────────────────────┬─────────────────────────────────────┘   │
│                                   │ Wayland IPC                              │
│  ┌────────────────────────────────┴─────────────────────────────────────┐   │
│  │                    Pure C Compositor Core                             │   │
│  │                                                                       │   │
│  │   opensef-compositor                                                  │   │
│  │   ├─ main.c         Entry point, CLI args                           │   │
│  │   ├─ server.c       wlroots init, scene graph setup                  │   │
│  │   ├─ output.c       Monitor handling, frame callbacks                │   │
│  │   ├─ view.c         Window (XDG toplevel) management                 │   │
│  │   ├─ input.c        Keyboard/pointer event handling                  │   │
│  │   ├─ layer_shell.c  Panel/dock integration                           │   │
│  │   └─ decorations.c  Server-side window decorations                   │   │
│  │                                                                       │   │
│  │   Integrates: wlroots 0.19, XDG shell, layer-shell                   │   │
│  └──────────────────────────────────────────────────────────────────────┘   │
│                                                                              │
└─────────────────────────────────────────────────────────────────────────────┘
```

### Data Flow

1. **Compositor** creates Wayland display, listens for clients
2. **Shell clients** (panel, dock) connect via `wl_display_connect()`
3. Shell clients request layer surfaces via `zwlr_layer_shell_v1`
4. Shell clients render UI with Cairo → shared memory buffer → Wayland surface
5. Compositor places layer surfaces on scene graph layers
6. Frame callback triggers scene render to output

---

## Getting Started

### Prerequisites

- **NixOS** or Linux with Nix package manager
- **Wayland session** (X11 won't work)
- Git

### Quick Setup

```bash
# Clone the repository
git clone https://github.com/raven1zed/vitusos-ares.git
cd vitusos-ares

# Enter the Nix development shell (installs all dependencies)
nix develop       # or: nix-shell (if not using flakes)

# Verify dependencies
pkg-config --modversion wlroots    # Should show 0.19.x
pkg-config --modversion cairo      # Should show 1.x
```

### Building

```bash
# Build compositor
cd opensef/opensef-compositor
mkdir -p build && cd build
cmake .. -G Ninja && ninja

# Build shell (from project root)
cd ../../opensef-shell
mkdir -p build && cd build
cmake .. -G Ninja && ninja
```

### Running

```bash
# Terminal 1: Start compositor (nested in another Wayland session)
WLR_BACKENDS=wayland ./opensef-compositor

# Terminal 2: Start panel (set WAYLAND_DISPLAY if needed)
./osf-panel

# Terminal 3: Start dock
./osf-dock
```

**Tip**: Use `Alt+Escape` to quit the compositor during development.

---

## Compositor (opensef-compositor)

The compositor is **pure C11** for wlroots compatibility.

### Core Files

| File | Lines | Purpose |
|------|-------|---------|
| `server.h` | ~220 | All data structures and forward declarations |
| `main.c` | ~100 | Entry point, argument parsing |
| `server.c` | ~210 | wlroots initialization, scene graph layers |
| `output.c` | ~120 | Monitor handling, frame rendering |
| `view.c` | ~285 | XDG toplevel window management |
| `input.c` | ~335 | Keyboard/pointer event handling |
| `layer_shell.c` | ~200 | Dock/panel surface arrangement |
| `decorations.c` | ~70 | Server-side decoration requests |

### Key Data Structures

```c
// server.h

struct osf_server {
    struct wl_display *wl_display;       // Wayland display
    struct wlr_backend *backend;         // Hardware abstraction
    struct wlr_scene *scene;             // Scene graph root
    
    // Desktop layers (bottom to top)
    struct wlr_scene_tree *layer_background;
    struct wlr_scene_tree *layer_bottom;
    struct wlr_scene_tree *layer_views;    // Normal windows
    struct wlr_scene_tree *layer_top;      // Panel, dock
    struct wlr_scene_tree *layer_overlay;  // Notifications
    
    struct wl_list views;          // osf_view linked list
    struct wl_list layer_surfaces; // osf_layer_surface linked list
    // ...
};

struct osf_view {
    struct osf_server *server;
    struct wlr_xdg_toplevel *xdg_toplevel;
    struct wlr_scene_tree *scene_tree;
    bool mapped;
    // Event listeners
    struct wl_listener map, unmap, commit, destroy;
    struct wl_listener request_move, request_resize;
    // ...
};
```

### Adding a New Feature

Example: Adding a **minimize all windows** keybinding:

1. Open `input.c`
2. Find `handle_compositor_keybinding()`
3. Add a new case:

```c
case XKB_KEY_d:
    if (wlr_keyboard_get_modifiers(...) & WLR_MODIFIER_ALT) {
        struct osf_view *view;
        wl_list_for_each(view, &server->views, link) {
            wlr_xdg_toplevel_set_minimized(view->xdg_toplevel, true);
        }
        return true;
    }
    break;
```

---

## UI Shell (opensef-shell)

The shell is **C++17** and uses Cairo for rendering.

### Core Classes

#### OSFSurface

The bridge between Cairo rendering and Wayland layer-shell:

```cpp
// include/OSFSurface.h
namespace opensef {

class OSFSurface {
public:
    OSFSurface(const std::string& namespace_name);
    
    bool connect(const char* display = nullptr);
    void disconnect();
    
    // Configuration
    void setLayer(Layer layer);      // Background, Bottom, Top, Overlay
    void setAnchor(Anchor anchor);   // Top, Bottom, Left, Right
    void setSize(int width, int height);
    void setExclusiveZone(int zone);
    
    // Rendering
    cairo_t* beginPaint();
    void endPaint();
    void commit();
    
    // Event loop
    void run();
    
    // Callbacks
    void onDraw(DrawCallback cb);
    void onConfigure(ConfigureCallback cb);
};

}
```

#### Creating a New Shell Client

Example: Creating a notification overlay:

```cpp
// src/notifications/main.cpp
#include "OSFSurface.h"
#include "OSFAresTheme.h"

int main() {
    opensef::OSFSurface surface("osf-notifications");
    
    surface.setLayer(opensef::Layer::Overlay);
    surface.setAnchor(opensef::Anchor::Top | opensef::Anchor::Right);
    surface.setSize(300, 100);
    surface.setMargin(32, 16, 0, 0); // top, right, bottom, left
    
    surface.onDraw([](cairo_t* cr, int w, int h) {
        // Draw notification background
        opensef::AresTheme::setCairoColor(cr, AresTheme::GlassBackground);
        opensef::AresTheme::roundedRect(cr, 0, 0, w, h, 12);
        cairo_fill(cr);
        
        // Draw text
        opensef::AresTheme::setCairoColor(cr, AresTheme::StarWhite);
        cairo_select_font_face(cr, AresTheme::FontFamily, 
                               CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, AresTheme::FontSizeNormal);
        cairo_move_to(cr, 16, 50);
        cairo_show_text(cr, "Hello from VitusOS!");
    });
    
    if (surface.connect()) {
        surface.run();
    }
    return 0;
}
```

---

## Widget Library (opensef-appkit)

Reusable Cairo-rendered widgets.

### Available Widgets

| Widget | File | Purpose |
|--------|------|---------|
| `OSFView` | View.cpp | Base class for all widgets |
| `OSFButton` | Button.cpp | Clickable button with states |
| `OSFLabel` | Label.cpp | Text display with alignment |
| `OSFTextField` | TextField.cpp | Text input field |
| `OSFGlassPanel` | GlassPanel.cpp | Frosted glass effect container |

### Widget Usage

```cpp
#include <opensef/OpenSEFAppKit.h>

// Create a button
auto button = OSFButton::create("Click Me");
button->setFrame(OSFRect(10, 10, 100, 32));
button->render(cairoContext);

// Create a label
auto label = OSFLabel::create("Status: Ready");
label->setAlignment(TextAlignment::Right);
label->render(cairoContext);
```

---

## Design System (Ares Theme)

All visual constants are defined in `OSFAresTheme.h`.

### Color Palette

```cpp
namespace AresTheme {
    // Primary Mars colors
    constexpr uint32_t MarsOrange = 0xFFE57C3A;   // #E57C3A - Primary accent
    constexpr uint32_t MarsGold   = 0xFFD4A93E;   // #D4A93E - Secondary
    
    // Traffic light buttons
    constexpr uint32_t ButtonClose    = 0xFFE57C3A; // Orange
    constexpr uint32_t ButtonMinimize = 0xFFD4A93E; // Gold
    constexpr uint32_t ButtonMaximize = 0xFF4A9FD4; // Blue
    
    // Text
    constexpr uint32_t StarWhite = 0xFF1A1A1A;    // Primary text (dark)
    constexpr uint32_t MoonGray  = 0xFF555555;    // Secondary text
    
    // Backgrounds  
    constexpr uint32_t LunarGray = 0xFFF5F5F5;    // Light surfaces
    constexpr uint32_t DeepSpace = 0xFFFFFFFF;    // Window backgrounds
    
    // Semi-transparent
    constexpr uint32_t PanelBackground = 0xF2F5F5F5; // 95% opacity
    constexpr uint32_t DockBackground  = 0xB3FFFFFF; // 70% opacity
}
```

### Dimensions

```cpp
namespace AresTheme {
    constexpr int PanelHeight = 28;           // Global menu bar
    constexpr int DockHeight = 64;            // Dock icon height
    constexpr int WindowCornerRadius = 8;     // Window rounding
    constexpr int DockCornerRadius = 16;      // Dock rounding
    constexpr int TrafficLightSize = 12;      // Window button diameter
}
```

### Helper Functions

```cpp
// Set Cairo color from ARGB
AresTheme::setCairoColor(cr, AresTheme::MarsOrange);

// Draw rounded rectangle
AresTheme::roundedRect(cr, x, y, width, height, radius);
cairo_fill(cr);

// Draw circle (for traffic lights)
AresTheme::circle(cr, centerX, centerY, radius);
cairo_fill(cr);

// Animation easing
double t = AresTheme::easeOut(progress);  // 0.0 to 1.0
```

---

## Building & Testing

### Full Build

```bash
nix develop
cd opensef
mkdir -p build && cd build
cmake .. -G Ninja -DBUILD_TESTING=ON
ninja
```

### Test Programs

Located in `opensef/test/`:

- `hello-window.cpp` - Basic layer surface test
- `welcome-screen.cpp` - Full welcome screen UI

### Debug Build

```bash
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug
ninja
```

### Running in Nested Wayland

```bash
WLR_BACKENDS=wayland ./build/opensef/opensef-compositor/opensef-compositor
```

---

## Contributing

### Code Style

- **C code**: K&R style, 2-space indentation
- **C++ code**: LLVM style, 2-space indentation
- **Naming**:
  - Functions: `osf_function_name` (C), `functionName` (C++)
  - Types: `osf_type_name` (C structs), `OSFTypeName` (C++ classes)
  - Constants: `UPPER_SNAKE_CASE`

### Commit Messages

```
component: Short description

Longer explanation if needed.
```

Examples:
- `compositor: Add focus-follows-mouse option`
- `shell/dock: Implement hover scale animation`
- `appkit: Add OSFSlider widget`

### Pull Request Process

1. Create a feature branch: `git checkout -b feature/my-feature`
2. Make changes and test on NixOS
3. Update documentation if needed
4. Submit PR with clear description

---

## Troubleshooting

### "wlroots not found"

```bash
# Check PKG_CONFIG_PATH
echo $PKG_CONFIG_PATH

# Ensure you're in nix shell
nix develop

# Try versioned package name
pkg-config --exists wlroots-0.19 && echo "Found"
```

### "Failed to connect to Wayland display"

- Ensure you're running inside a Wayland session (not X11)
- Check `WAYLAND_DISPLAY` environment variable
- If running nested: `export WAYLAND_DISPLAY=wayland-1`

### Compositor crashes on startup

Check wlroots log output:
```bash
WLR_DEBUG=1 ./opensef-compositor
```

### Panel/Dock not appearing

1. Verify compositor supports layer-shell:
   ```bash
   wayland-info | grep layer_shell
   ```
2. Check for errors in shell client output
3. Ensure exclusive zone is set correctly

---

## Resources

- [wlroots Documentation](https://gitlab.freedesktop.org/wlroots/wlroots)
- [Wayland Protocol Docs](https://wayland.freedesktop.org/docs/html/)
- [Cairo Documentation](https://www.cairographics.org/documentation/)
- [Pango Documentation](https://docs.gtk.org/Pango/)

---

*This documentation is maintained by the VitusOS team with AI assistance from Claude (Anthropic).*
