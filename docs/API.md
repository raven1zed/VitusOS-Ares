# openSEF API Reference

**Version:** 0.2.0  
**Language:** C++ (Shell) / C (Compositor)  
**Theme:** Ares (The Martian)

---

## Table of Contents

- [Layer Surface API (OSFSurface)](#layer-surface-api-osfsurface)
- [UI Widgets (opensef-appkit)](#ui-widgets-opensef-appkit)
- [Design System (AresTheme)](#design-system-arestheme)
- [Compositor Structures](#compositor-structures)

---

## Layer Surface API (OSFSurface)

The core class for creating Cairo-rendered Wayland surfaces.

### Header

```cpp
#include "OSFSurface.h"

namespace opensef {

// Layer positioning (matches wlr-layer-shell)
enum class Layer { Background = 0, Bottom = 1, Top = 2, Overlay = 3 };

// Anchor edges
enum class Anchor { None = 0, Top = 1, Bottom = 2, Left = 4, Right = 8 };

class OSFSurface {
public:
    explicit OSFSurface(const std::string& namespace_name);
    ~OSFSurface();
    
    // Connection
    bool connect(const char* display = nullptr);
    void disconnect();
    
    // Configuration (call before connect)
    void setLayer(Layer layer);
    void setAnchor(Anchor anchor);
    void setSize(int width, int height);
    void setExclusiveZone(int zone);
    void setMargin(int top, int right, int bottom, int left);
    
    // Rendering
    cairo_t* beginPaint();      // Returns Cairo context for drawing
    void endPaint();
    void damage(int x, int y, int width, int height);
    void commit();
    
    // Event loop
    void run();                 // Blocking event dispatch
    void stop();
    bool isRunning() const;
    
    // Callbacks
    using ConfigureCallback = std::function<void(int width, int height)>;
    using DrawCallback = std::function<void(cairo_t* cr, int width, int height)>;
    using CloseCallback = std::function<void()>;
    
    void onConfigure(ConfigureCallback cb);
    void onDraw(DrawCallback cb);     // Called automatically on configure
    void onClose(CloseCallback cb);
    
    // Getters
    int width() const;
    int height() const;
    struct wl_display* display() const;
};

}
```

### Usage Example

```cpp
#include "OSFSurface.h"
#include "OSFAresTheme.h"

using namespace opensef;

int main() {
    OSFSurface surface("my-app");
    
    // Configure as bottom dock
    surface.setLayer(Layer::Top);
    surface.setAnchor(Anchor::Bottom);
    surface.setSize(400, 64);
    surface.setExclusiveZone(64);
    
    // Set draw callback
    surface.onDraw([](cairo_t* cr, int w, int h) {
        // Background
        AresTheme::setCairoColor(cr, AresTheme::DockBackground);
        AresTheme::roundedRect(cr, 0, 0, w, h, 16);
        cairo_fill(cr);
    });
    
    if (surface.connect()) {
        surface.run();
    }
    return 0;
}
```

---

## UI Widgets (opensef-appkit)

Reusable Cairo-rendered widgets for shell applications.

### Base Widget (OSFView)

```cpp
#include <opensef/OSFView.h>

class OSFView {
public:
    void setFrame(const OSFRect& frame);
    OSFRect frame() const;
    
    virtual void render(cairo_t* cr);
    virtual void layout();
};
```

### OSFButton

Interactive button with hover/press states.

```cpp
#include <opensef/OpenSEFAppKit.h>

auto button = OSFButton::create("Click Me");
button->setFrame(OSFRect(10, 10, 120, 32));
button->setCallback([]() {
    std::cout << "Button clicked!" << std::endl;
});
button->render(cairoContext);
```

### OSFLabel

Text display with alignment options.

```cpp
auto label = OSFLabel::create("Status: Connected");
label->setFrame(OSFRect(10, 10, 200, 20));
label->setAlignment(TextAlignment::Right);
label->setFont(AresTheme::FontFamily, AresTheme::FontSizeNormal);
label->setText("Updated text");
label->render(cairoContext);
```

### OSFTextField

Text input field (requires input event integration).

```cpp
auto field = OSFTextField::create();
field->setFrame(OSFRect(10, 50, 200, 28));
field->setPlaceholder("Enter name...");
field->onTextChanged([](const std::string& text) {
    std::cout << "Text: " << text << std::endl;
});
```

### OSFGlassPanel

Frosted glass effect container.

```cpp
auto panel = OSFGlassPanel::create();
panel->setFrame(OSFRect(0, 0, width, height));
panel->setBlurRadius(16);  // Future: requires compositor support
panel->render(cairoContext);
```

---

## Design System (AresTheme)

All visual constants in `OSFAresTheme.h`.

### Colors (ARGB Format)

```cpp
namespace opensef::AresTheme {
    // Mars-inspired primaries
    constexpr uint32_t MarsOrange = 0xFFE57C3A;   // Primary accent
    constexpr uint32_t MarsGold   = 0xFFD4A93E;   // Secondary accent
    constexpr uint32_t MarsSand   = 0xFFB5651D;   // Desert sand
    constexpr uint32_t MarsRed    = 0xFFC44536;   // Rust red
    
    // Neutral (Light Mode)
    constexpr uint32_t LunarGray  = 0xFFF5F5F5;   // Panel backgrounds
    constexpr uint32_t DeepSpace  = 0xFFFFFFFF;   // Window backgrounds
    constexpr uint32_t StarWhite  = 0xFF1A1A1A;   // Primary text
    constexpr uint32_t MoonGray   = 0xFF555555;   // Secondary text
    
    // Traffic light buttons
    constexpr uint32_t ButtonClose    = 0xFFE57C3A;   // Orange
    constexpr uint32_t ButtonMinimize = 0xFFD4A93E;   // Gold
    constexpr uint32_t ButtonMaximize = 0xFF4A9FD4;   // Blue
    
    // Semi-transparent
    constexpr uint32_t GlassBackground = 0xD9FFFFFF;  // 85% white
    constexpr uint32_t PanelBackground = 0xF2F5F5F5;  // 95% gray
    constexpr uint32_t DockBackground  = 0xB3FFFFFF;  // 70% white
}
```

### Dimensions

```cpp
namespace opensef::AresTheme {
    constexpr int PanelHeight = 28;
    constexpr int DockHeight = 64;
    constexpr int DockItemSpacing = 8;
    constexpr int DockPadding = 8;
    constexpr int DockCornerRadius = 16;
    
    constexpr int WindowCornerRadius = 8;
    constexpr int TitleBarHeight = 30;
    constexpr int TrafficLightSize = 12;
    constexpr int TrafficLightSpacing = 8;
    constexpr int TrafficLightMargin = 10;
}
```

### Typography

```cpp
namespace opensef::AresTheme {
    constexpr const char* FontFamily = "Inter";
    constexpr const char* FallbackFont = "sans-serif";
    
    constexpr int FontSizeSmall  = 11;
    constexpr int FontSizeNormal = 13;
    constexpr int FontSizeLarge  = 16;
    constexpr int FontSizeTitle  = 20;
    constexpr int FontSizeHero   = 32;
}
```

### Helper Functions

```cpp
// Set Cairo source color from ARGB
void setCairoColor(cairo_t* cr, uint32_t color);

// Draw a rounded rectangle path
void roundedRect(cairo_t* cr, double x, double y, double w, double h, double r);

// Draw a circle
void circle(cairo_t* cr, double cx, double cy, double r);

// Animation easing (t: 0.0 to 1.0)
double easeOut(double t);
double easeInOut(double t);
double lerp(double a, double b, double t);
```

---

## Compositor Structures

For contributors working on `opensef-compositor` (Pure C).

### osf_server

```c
struct osf_server {
    struct wl_display *wl_display;
    struct wlr_backend *backend;
    struct wlr_renderer *renderer;
    struct wlr_allocator *allocator;
    struct wlr_compositor *compositor;
    
    // Scene graph layers (z-order)
    struct wlr_scene *scene;
    struct wlr_scene_tree *layer_background;  // Wallpaper
    struct wlr_scene_tree *layer_bottom;      // Desktop widgets
    struct wlr_scene_tree *layer_views;       // Normal windows
    struct wlr_scene_tree *layer_top;         // Panel, dock
    struct wlr_scene_tree *layer_overlay;     // Notifications
    
    // Shell protocols
    struct wlr_xdg_shell *xdg_shell;
    struct wlr_layer_shell_v1 *layer_shell;
    
    // Lists
    struct wl_list outputs;         // osf_output
    struct wl_list views;           // osf_view
    struct wl_list layer_surfaces;  // osf_layer_surface
    struct wl_list keyboards;       // osf_keyboard
};
```

### osf_view (Window)

```c
struct osf_view {
    struct wl_list link;
    struct osf_server *server;
    struct wlr_xdg_toplevel *xdg_toplevel;
    struct wlr_scene_tree *scene_tree;
    
    bool mapped;
    int x, y;
    
    // Event listeners
    struct wl_listener map, unmap, commit, destroy;
    struct wl_listener request_move, request_resize;
    struct wl_listener request_maximize, request_fullscreen;
};
```

### Key Functions

```c
// View management
struct osf_view* osf_view_at(struct osf_server *server, 
                              double lx, double ly,
                              struct wlr_surface **surface, 
                              double *sx, double *sy);

void osf_focus_view(struct osf_view *view, struct wlr_surface *surface);
void osf_reset_cursor_mode(struct osf_server *server);

// Server lifecycle
bool osf_server_init(struct osf_server *server);
void osf_server_run(struct osf_server *server);
void osf_server_finish(struct osf_server *server);
```

---

*API Reference - VitusOS Ares v0.2.0*
