# openSEF Framework API Reference

## Core Singleton

### OSFDesktop

**Purpose**: Central access point for all desktop functionality.

```cpp
#include <OSFDesktop.h>

// Get singleton instance
auto* desktop = OSFDesktop::shared();

// Initialize framework
desktop->initialize();

// Access services
auto* eventBus = desktop->eventBus();
auto* stateManager = desktop->stateManager();
auto* windowManager = desktop->windowManager();
auto* serviceRegistry = desktop->serviceRegistry();
auto* resourceCache = desktop->resourceCache();
auto* themeManager = desktop->themeManager();

// Run main loop (optional, handled by compositor)
desktop->run();

// Cleanup
desktop->terminate();
```

---

## Event System

### OSFEventBus

**Purpose**: Unified event communication across all components.

#### Subscribe to Events

```cpp
desktop->eventBus()->subscribe("window.created", [](const OSFEvent& e) {
  auto windowId = e.get<std::string>("window_id");
  auto title = e.get<std::string>("title");
  std::cout << "Window created: " << title << std::endl;
});
```

#### Publish Events

```cpp
OSFEvent event;
event.set("window_id", "window-123");
event.set("title", "My Window");
desktop->eventBus()->publish("window.created", event);
```

#### Standard Events

```cpp
// Window events
OSFEventBus::WINDOW_CREATED      // "window.created"
OSFEventBus::WINDOW_DESTROYED    // "window.destroyed"
OSFEventBus::WINDOW_FOCUSED      // "window.focused"
OSFEventBus::WINDOW_MINIMIZED    // "window.minimized"
OSFEventBus::WINDOW_MAXIMIZED    // "window.maximized"

// Application events
OSFEventBus::APP_LAUNCHED        // "app.launched"
OSFEventBus::APP_TERMINATED      // "app.terminated"

// Menu events
OSFEventBus::MENU_CLICKED        // "menu.clicked"

// System events
OSFEventBus::THEME_CHANGED       // "theme.changed"
OSFEventBus::WORKSPACE_CHANGED   // "workspace.changed"
```

---

## State Management

### OSFStateManager

**Purpose**: Single source of truth for desktop state.

#### Query Windows

```cpp
// Get all windows
auto windows = desktop->stateManager()->allWindows();
for (auto* window : windows) {
  std::cout << window->title() << std::endl;
}

// Get active window
auto* activeWin = desktop->stateManager()->activeWindow();
if (activeWin) {
  std::cout << "Active: " << activeWin->title() << std::endl;
}

// Get window by ID
auto* window = desktop->stateManager()->windowById("window-123");
```

#### Query Applications

```cpp
// Get running apps
auto apps = desktop->stateManager()->runningApps();

// Get app by ID
auto* app = desktop->stateManager()->appById("com.vitus.settings");
```

#### Query Workspaces

```cpp
// Get current workspace
int current = desktop->stateManager()->currentWorkspace();

// Get all workspaces
auto workspaces = desktop->stateManager()->allWorkspaces();
```

---

## Window Management

### OSFWindowManager

**Purpose**: High-level window operations and lifecycle management.

#### Window Queries

```cpp
// Same as stateManager (delegates to it)
auto windows = desktop->windowManager()->allWindows();
auto* activeWin = desktop->windowManager()->activeWindow();
auto* window = desktop->windowManager()->windowById("window-123");
```

#### Window Actions

```cpp
// Focus window
desktop->windowManager()->focusWindow("window-123");

// Minimize window
desktop->windowManager()->minimizeWindow("window-123");

// Maximize/restore window
desktop->windowManager()->maximizeWindow("window-123");

// Close window
desktop->windowManager()->closeWindow("window-123");

// Move window
desktop->windowManager()->moveWindow("window-123", 100, 100);

// Resize window
desktop->windowManager()->resizeWindow("window-123", 800, 600);
```

#### Window Callbacks

```cpp
// Called when window is created
desktop->windowManager()->onWindowCreated([](OSFWindow* window) {
  std::cout << "Created: " << window->title() << std::endl;
});

// Called when window is destroyed
desktop->windowManager()->onWindowDestroyed([](const std::string& id) {
  std::cout << "Destroyed: " << id << std::endl;
});

// Called when window is focused
desktop->windowManager()->onWindowFocused([](OSFWindow* window) {
  std::cout << "Focused: " << window->title() << std::endl;
});
```

#### Window Registration (Compositor Only)

```cpp
// Create window object
auto* window = new OSFWindow("window-123", "My Window", "com.app.id");

// Register with framework
desktop->windowManager()->registerWindow(window);
// Automatically publishes "window.created" event

// Update window title
desktop->windowManager()->updateWindowTitle("window-123", "New Title");

// Unregister window
desktop->windowManager()->unregisterWindow("window-123");
// Automatically publishes "window.destroyed" event
```

---

## Service Discovery

### OSFServiceRegistry

**Purpose**: Component registration and discovery.

#### Define a Service

```cpp
class PanelService : public OSFService {
public:
  std::string name() const override { return "panel"; }
  
  void start() override {
    // Initialize panel
  }
  
  void stop() override {
    // Cleanup panel
  }
  
  // Custom methods
  void showMenu(const std::string& menuId) {
    // Show menu
  }
};
```

#### Register Service

```cpp
auto* panel = new PanelService();
desktop->serviceRegistry()->registerService("panel", panel);
```

#### Discover Service

```cpp
auto* panel = desktop->serviceRegistry()->getService("panel");
if (panel) {
  auto* panelService = static_cast<PanelService*>(panel);
  panelService->showMenu("applications");
}

// Check if service exists
if (desktop->serviceRegistry()->hasService("panel")) {
  // Service is available
}

// Get all services
auto services = desktop->serviceRegistry()->allServices();
```

---

## Resource Management

### OSFResourceCache

**Purpose**: Shared cache for icons, images, and fonts.

#### Icons

```cpp
// Get icon (cached automatically)
auto* icon = desktop->resourceCache()->getIcon("application-menu", 24);

// Preload icon in background
desktop->resourceCache()->preloadIcon("application-menu");

// Clear icon cache
desktop->resourceCache()->clearIconCache();
```

#### Images

```cpp
// Get image (cached automatically)
auto* image = desktop->resourceCache()->getImage("/path/to/wallpaper.png");

// Clear image cache
desktop->resourceCache()->clearImageCache();
```

#### Cache Management

```cpp
// Clear all caches
desktop->resourceCache()->clearAllCaches();

// Get cache size
size_t size = desktop->resourceCache()->cacheSize();
```

---

## Theme Management

### OSFThemeManager

**Purpose**: Unified theme system for all components.

#### Colors

```cpp
auto primary = desktop->themeManager()->primaryColor();
auto background = desktop->themeManager()->backgroundColor();
auto text = desktop->themeManager()->textColor();
auto accent = desktop->themeManager()->accentColor();
auto border = desktop->themeManager()->borderColor();
auto highlight = desktop->themeManager()->highlightColor();

// Use with Cairo
cairo_set_source_rgba(cr, primary.r, primary.g, primary.b, primary.a);
```

#### Dimensions

```cpp
int panelHeight = desktop->themeManager()->panelHeight();
int dockIconSize = desktop->themeManager()->dockIconSize();
int borderWidth = desktop->themeManager()->windowBorderWidth();
int radius = desktop->themeManager()->cornerRadius();
```

#### Fonts

```cpp
std::string fontFamily = desktop->themeManager()->systemFontFamily();
int fontSize = desktop->themeManager()->systemFontSize();

std::string titleFont = desktop->themeManager()->titleFontFamily();
int titleSize = desktop->themeManager()->titleFontSize();
```

#### Theme Switching

```cpp
// Load theme
desktop->themeManager()->loadTheme("Ares");

// Get current theme
std::string current = desktop->themeManager()->currentTheme();
```

---

## Complete Example

### Creating a Panel Application

```cpp
#include <OSFDesktop.h>
#include <OSFServiceRegistry.h>

class MyPanel : public OSFService {
public:
  MyPanel() {
    auto* desktop = OSFDesktop::shared();
    
    // Subscribe to window events
    desktop->eventBus()->subscribe("window.created", [this](auto& e) {
      updateWindowList();
    });
    
    desktop->eventBus()->subscribe("window.destroyed", [this](auto& e) {
      updateWindowList();
    });
  }
  
  std::string name() const override { return "my-panel"; }
  
  void start() override {
    render();
  }
  
  void stop() override {
    // Cleanup
  }
  
private:
  void updateWindowList() {
    auto* desktop = OSFDesktop::shared();
    auto windows = desktop->windowManager()->allWindows();
    
    // Update UI with window list
    for (auto* window : windows) {
      std::cout << "Window: " << window->title() << std::endl;
    }
    
    render();
  }
  
  void render() {
    auto* desktop = OSFDesktop::shared();
    auto theme = desktop->themeManager();
    
    // Use theme colors
    auto bgColor = theme->backgroundColor();
    auto textColor = theme->textColor();
    int height = theme->panelHeight();
    
    // Render panel...
  }
};

int main() {
  auto* desktop = OSFDesktop::shared();
  desktop->initialize();
  
  auto* panel = new MyPanel();
  desktop->serviceRegistry()->registerService("my-panel", panel);
  panel->start();
  
  desktop->run();
  
  return 0;
}
```

---

## Best Practices

### 1. Always Use the Framework

❌ **Don't**: Create your own state management  
✅ **Do**: Query state from `OSFStateManager`

❌ **Don't**: Implement manual IPC  
✅ **Do**: Use `OSFEventBus` for communication

❌ **Don't**: Load your own theme  
✅ **Do**: Use `OSFThemeManager` for colors/fonts

### 2. Subscribe to Events

Components should react to events, not poll for changes.

```cpp
// Good
desktop->eventBus()->subscribe("window.created", handler);

// Bad
while (true) {
  checkForNewWindows();
  sleep(1);
}
```

### 3. Register as a Service

If your component provides functionality others might use, register it.

```cpp
desktop->serviceRegistry()->registerService("my-service", this);
```

### 4. Use Shared Resources

Don't load duplicate icons/images.

```cpp
// Good
auto* icon = desktop->resourceCache()->getIcon("app-icon", 48);

// Bad
auto* icon = loadIconFromFile("/usr/share/icons/app-icon.png");
```

---

## Thread Safety

All framework components are thread-safe:
- `OSFEventBus` - Thread-safe event publishing
- `OSFStateManager` - Thread-safe state queries
- `OSFServiceRegistry` - Thread-safe service access
- `OSFResourceCache` - Thread-safe resource loading

You can safely call framework methods from any thread.

---

## Performance Tips

1. **Cache event handlers**: Don't create lambdas in hot paths
2. **Batch state queries**: Get all windows once, not per-window
3. **Preload resources**: Use `preloadIcon()` for frequently used icons
4. **Use async events**: `publishAsync()` for non-critical events

---

**This is the complete openSEF Framework API.**

**One API. One system. One experience.**
