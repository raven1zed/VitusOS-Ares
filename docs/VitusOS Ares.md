# VitusOS Ares - openSEF Unified Architecture

**openSEF is NOT a Linux Desktop Environment.**

openSEF is a **complete Desktop Experience framework** - from boot to shutdown, ONE unified system like macOS.

---

## The Fundamental Difference

### Linux Paradigm (What We REJECT) ❌

```
Separate components talking via protocols:
- Compositor (Wayland)
- Panel (listening to D-Bus)
- Dock (reading .desktop files)
- Apps (doing their own thing)
- SystemD (boot logs visible)
```

**Problems**:
- Fragmented
- Each component loads own config/theme
- Manual IPC via D-Bus
- User sees boot logs
- Feels like separate apps, not ONE system

### openSEF Paradigm (What We ARE) ✅

```
┌────────────────────────────────────────────────────┐
│            openSEF Framework (Cocoa-like)          │
│  ┌──────────────────────────────────────────────┐  │
│  │         OSFDesktop (Singleton API)           │  │
│  │   • EventBus  • StateManager  • WindowMgr   │  │
│  │   • Theme  • Resources  • Services           │  │
│  └──────────────────────────────────────────────┘  │
│                       ↕                            │
│  ALL COMPONENTS USE THE SAME FRAMEWORK:            │
│                                                    │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐        │
│  │Compositor│  │  Shell   │  │   Apps   │        │
│  │(wlroots +│  │(Qt Quick)│  │ (GNUstep)│        │
│  │ Vulkan)  │  │          │  │          │        │
│  └──────────┘  └──────────┘  └──────────┘        │
│                                                    │
│  Boot Animation → Login → Desktop → Shutdown      │
│         (NO systemd logs visible)                  │
└────────────────────────────────────────────────────┘
```

**Result**:
- ONE system (feels like macOS)
- Unified API for everything
- Shared state across all components
- Smooth experience from boot to shutdown
- No fragmentation

---

## The Stack (As It Should Be)

### Layer 1: Rendering (Qt Quick + Vulkan)
**Purpose**: High-performance GPU rendering

```
QML (declarative UI)
  ↓
Qt Quick Scene Graph
  ↓
QRhi (abstraction)
  ↓
Vulkan (600+ FPS)
```

**This is ONLY for rendering** - not the widget logic.

### Layer 2: Widget Framework (GNUstep AppKit - Forked)
**Purpose**: macOS-like widget library (NOT for rendering!)

```
GNUstep AppKit (forked from GitHub)
  • NSButton, NSMenu, NSWindow
  • Layout engine (constraints, stack views)
  • Responder chain
  • MVC pattern
  • Delegates

NO CAIRO BACKEND - We strip out GNUstep's rendering code
```

**Bridge**: Objective-C++ connects AppKit widgets to Qt Quick for rendering

### Layer 3: openSEF Desktop Framework
**Purpose**: Unified system API (boot to shutdown)

```cpp
// Single API for EVERYTHING
auto* desktop = OSFDesktop::shared();

// Window management
desktop->windowManager()->allWindows();

//Events
desktop->eventBus()->subscribe("window.created", handler);

// Theme
desktop->themeManager()->primaryColor();

// Boot sequence
desktop->bootManager()->showBootAnimation();

// Session
desktop->sessionManager()->logout();
```

### Layer 4: System Integration
**Purpose**: Boot, login, services

- **Boot sequence** (custom, NO systemd logs shown)
- **Login screen** (smooth animation)
- **Session management** (logout/shutdown)
- **Service registry** (like launchd, not systemd)

---

## What We're Actually Building

### Phase 1: Fork GNUstep AppKit ✅

```bash
cd opensef/
git clone https://github.com/gnustep/libs-gui.git opensef-gnustep
cd opensef-gnustep

# Keep ONLY:
# - Foundation/ (NSString, NSArray)
# - AppKit/NSView.m NSButton.m NSMenu.m NSWindow.m
# - AppKit/Layout/

# REMOVE:
# - All Cairo/X11 rendering code
# - Back.framework (graphics backend)
# - Art/ (drawing code)
```

### Phase 2: Qt/AppKit Bridge ✅

```
opensef-shell-qt/
├── AppKit/              🆕 AppKit widget wrappers
│   ├── AresButton.m     (NSButton-based)
│   ├── AresMenu.m       (NSMenu-based)
│   └── AresWindow.m
│
├── src/
│   ├── AppKitBridge.mm  🆕 Objective-C++ bridge
│   └── PanelController.cpp
│
└── qml/
    ├── Panel.qml        (Uses AresButton via bridge)
    └── Dock.qml
```

**Bridge Example**:
```cpp
// AppKitBridge.mm
class AresButtonQML : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(QString label READ label WRITE setLabel)
  
private:
  AresButton* ns_button_; // GNUstep widget (logic only)
  
public:
  // Button click logic = AppKit
  // Rendering = Qt Quick
  void paint(QPainter *painter) override {
    // Read state from ns_button_, render with Qt
  }
};
```

### Phase 3: Unified Framework API ✅

**Boot Sequence**:
```cpp
// Instead of systemd showing logs:
OSFDesktop::shared()->bootManager()->showSplash();
OSFDesktop::shared()->bootManager()->initServices();
OSFDesktop::shared()->bootManager()->transitionToLogin();
```

**All Components Use Same API**:
```cpp
// Compositor
osf_framework_init(); // C wrapper

// Shell
OSFDesktop::shared()->eventBus()->subscribe("window.focused");

// Apps
OSFDesktop::shared()->windowManager()->createWindow();
```

---

## What Gemini Got COMPLETELY Wrong

### ❌ WRONG: "Use DBusMenu protocol"
**Correct**: Apps use openSEF native menu API, NOT D-Bus

```cpp
// Native openSEF app:
auto* menu = OSFMenu::create();
menu->addItem("File", @selector(openFile:));
desktop->menuManager()->registerMenu(menu, windowId);

// Shell automatically shows it - NO D-Bus
```

### ❌ WRONG: "Use StatusNotifier for tray"
**Correct**: Apps register with openSEF tray API

```cpp
// Native app:
auto* tray = OSF TrayIcon::create("app-icon");
desktop->systemTray()->registerIcon(tray);

// Shell shows it - NO D-Bus
```

### ❌ WRONG: "Separate stack for native apps (Cairo)"
**Correct**: ONE rendering stack (Qt Quick), AppKit provides widget LOGIC

```
Native App:
  AppKit widgets (NSButton logic)
    ↓
  openSEF framework
    ↓
  Qt Quick rendering (Vulkan)
```

### ❌ WRONG: "Two parallel stacks"
**Correct**: ONE unified system

```
Everything goes through openSEF framework:
Boot → Compositor → Shell → Apps → Shutdown

All use OSFDesktop API
All render via Qt Quick + Vulkan
AppKit provides widget architecture (not rendering)
```

---

## The Experience (Boot to Shutdown)

### 1. Boot
```
Hardware ON
  ↓
OSFBootManager::showSplash()
  ↓
Load openSEF framework
  ↓
Init compositor (wlroots + Vulkan)
  ↓
Transition to login (smooth fade)

NO SYSTEMD LOGS VISIBLE
```

### 2. Login
```
OSFLockscreen appears
  ↓
User enters password
  ↓
OSFAuthenticator validates
  ↓
Fade to desktop

ONE SMOOTH ANIMATION
```

### 3. Desktop
```
Panel (Qt Quick rendering, AppKit logic)
Dock (Qt Quick rendering, AppKit logic)
Apps (Qt Quick rendering, AppKit logic)

All using: OSFDesktop::shared()
```

### 4. Shutdown
```
User clicks Shutdown
  ↓
OSFSessionManager::shutdown()
  ↓
Save state
  ↓
Fade out animation
  ↓
Hardware OFF

NO SYSTEMD LOGS
```

---

## Current Status (What's Done vs What's Needed)

### ✅ Done
- Compositor (wlroots + Vulkan)
- Shell UI (Qt Quick rendering)
- Framework skeleton (OSFDesktop stub)
- Event bus (basic pub/sub)

### 🚧 In Progress
- Qt shell works butuses QML components, not AppKit widgets
- Framework exists but not fully wired
- Boot sequence shows systemd logs (need custom boot)

### ❌ Not Started
- Fork GNUstep AppKit
- Qt/AppKit bridge (Objective-C++)
- Native app support (using AppKit widgets)
- Boot manager (hide systemd)
- Session manager (smooth logout/shutdown)

---

## Next Steps (The Real Plan)

### Week 1: Fork GNUstep
1. Clone GNUstep libs-gui
2. Strip out Cairo rendering code
3. Keep only AppKit widget logic
4. CMake build for static library

### Week 2: AppKit Bridge
1. Create Objective-C++ bridge layer
2. Expose NSButton, NSMenu to QML
3. Test: Button rendered via Qt, logic via AppKit

### Week 3: Native App Support
1. Create native app template using AppKit
2. Wire up to framework API
3. Test: Full native app running

### Week 4: Boot/Session Management
1. Custom boot splash (hide systemd)
2. Smooth login transition
3. Clean shutdown sequence

---

## The Unified Vision

**What openSEF IS**:
- macOS Cocoa for Linux
- Boot-to-shutdown integrated system
- ONE framework API
- AppKit widgets + Qt Quick rendering
- Seamless user experience

**What openSEF IS NOT**:
- Another Linux DE (GNOME/KDE)
- Collection of separate tools
- D-Bus spaghetti
- Fragmented components

---

**This document is the source of truth.**  
**Everything else that contradicts this is outdated.**

Last Updated: 2026-01-11
