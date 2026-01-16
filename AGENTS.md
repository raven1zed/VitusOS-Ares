# AGENTS.md - Guide for AI Assistants Working on VitusOS-Ares

**Last Updated**: January 16, 2026  
**Purpose**: Prevent AI instances from breaking this project with incorrect assumptions

---

## ⚠️ CRITICAL: Read This First

**This project has been broken multiple times by AI assistants who:**
1. Assumed it's a traditional Linux DE (GNOME/KDE paradigm)
2. Added Cairo/Pango dependencies when we use Qt Quick + Vulkan
3. Implemented D-Bus for internal communication (wrong - only for legacy app compat)
4. Treated components as independent services (wrong - unified framework)
5. Assumed server-side window decorations (wrong - custom QML client-side)
6. Made up APIs that don't exist
7. Didn't read the vision document

**If you don't understand something, ASK THE USER. Don't assume. Don't hallucinate.**

---

## 1. What VitusOS-Ares Actually Is

### The Vision

VitusOS-Ares is a **unified desktop environment** inspired by macOS, NOT a collection of independent Linux components.

**Think**: macOS (unified, cohesive)  
**NOT**: GNOME/KDE (fragmented, protocol soup)

### Core Philosophy

```
ONE unified system (openSEF)
    ↓
Controls everything through OSFDesktop::shared()
    ↓
All communication via EventBus
    ↓
Consistent behavior across all components
```

**Key Document**: [`vitusos_complete_vision.md`](file:///C:/Users/hp/Documents/VitusOS-Ares/docs/vitusos_complete_vision.md)  
**READ IT COMPLETELY** before making any changes.

---

## 2. Architecture - The Truth

### What openSEF Is

openSEF is **THREE things in ONE**:

1. **Desktop Environment** - The visual shell users interact with
2. **API Layer** - Unified C++ framework (OSFDesktop, EventBus, etc.)
3. **Application Framework** - GNUstep C++ fork for native apps

### Technology Stack

```
┌─────────────────────────────────────────┐
│  Shell (Qt Quick + Vulkan)              │  ← GPU-accelerated UI
│  - Panel, Dock, SystemTray, Wallpaper   │
│  - QML components, NO Cairo/Pango       │
└─────────────────────────────────────────┘
              ↓ uses ↓
┌─────────────────────────────────────────┐
│  Framework (Pure C++)                   │  ← NO external deps
│  - OSFDesktop::shared()                 │     except pthread
│  - EventBus, StateManager, etc.         │
│  - NO Cairo, NO Pango, NO xkbcommon     │
└─────────────────────────────────────────┘
              ↓ controls ↓
┌─────────────────────────────────────────┐
│  Compositor (wlroots + Vulkan)          │  ← Pure C
│  - Window management                    │
│  - Wayland protocol                     │
└─────────────────────────────────────────┘
```

### What We DON'T Use

❌ **Cairo/Pango** - We use Qt Quick for rendering  
❌ **D-Bus for internal comms** - We use EventBus (D-Bus only for legacy GTK/Qt/Electron apps)  
❌ **Server-side decorations** - We use custom QML client-side decorations  
❌ **systemd for desktop** - openSEF dominates systemd  
❌ **Traditional Linux DE patterns** - We follow macOS patterns

---

## 3. The Unified Framework (OSFDesktop)

### The Single Source of Truth

**EVERY component** must use:

```cpp
auto* desktop = OpenSEF::OSFDesktop::shared();
```

This is the **ONLY** way to access framework services.

### Available Services

```cpp
// Event system (ALL internal communication)
desktop->eventBus()->subscribe(OSFEventBus::WINDOW_FOCUSED, handler);
desktop->eventBus()->publish(OSFEventBus::APP_LAUNCHED, event);

// State management
desktop->stateManager()->setState("key", value);
desktop->stateManager()->getState("key");

// Window management
desktop->windowManager()->allWindows();
desktop->windowManager()->focusWindow(id);

// Other services
desktop->serviceRegistry()  // Service discovery
desktop->resourceCache()    // Resource caching
desktop->themeManager()     // Theme/colors
desktop->clipboardManager() // Universal clipboard
desktop->shortcutManager()  // Keyboard shortcuts
```

### Event Types (Pre-Defined)

**Use these constants, don't make up your own:**

```cpp
// Window events
OSFEventBus::WINDOW_CREATED
OSFEventBus::WINDOW_DESTROYED
OSFEventBus::WINDOW_FOCUSED
OSFEventBus::WINDOW_MINIMIZED
OSFEventBus::WINDOW_MAXIMIZED

// App events
OSFEventBus::APP_LAUNCHED
OSFEventBus::APP_TERMINATED

// UI events
OSFEventBus::MULTITASK_TOGGLE
OSFEventBus::THEME_CHANGED
OSFEventBus::WORKSPACE_CHANGED

// Clipboard events
OSFEventBus::CLIPBOARD_COPY
OSFEventBus::CLIPBOARD_PASTE
OSFEventBus::CLIPBOARD_CHANGED
```

**Location**: [`opensef-framework/include/OSFEventBus.h`](file:///C:/Users/hp/Documents/VitusOS-Ares/opensef/opensef-framework/include/OSFEventBus.h)

---

## 4. Component Details

### Panel (Global Menu Bar)

**File**: [`opensef-shell-qt/qml/AresPanel.qml`](file:///C:/Users/hp/Documents/VitusOS-Ares/opensef/opensef-shell-qt/qml/AresPanel.qml)  
**Controller**: [`opensef-shell-qt/src/PanelController.cpp`](file:///C:/Users/hp/Documents/VitusOS-Ares/opensef/opensef-shell-qt/src/PanelController.cpp)

**Behavior**:
- Shows **Filer menu** when desktop is idle (no windows focused)
- Dynamically switches to focused app's menu
- Like macOS menu bar (NOT static like GNOME)

**WRONG Assumptions**:
- ❌ "Panel should always show the same menu"
- ❌ "Use DBusMenu for native apps" (only for legacy apps in up1)
- ❌ "Hardcode menu items" (use initializeDefaultMenus() for Filer, load dynamically for others)

### Dock

**File**: [`opensef-shell-qt/qml/Dock.qml`](file:///C:/Users/hp/Documents/VitusOS-Ares/opensef/opensef-shell-qt/qml/Dock.qml)  
**Controller**: [`opensef-shell-qt/src/DockController.cpp`](file:///C:/Users/hp/Documents/VitusOS-Ares/opensef/opensef-shell-qt/src/DockController.cpp)

**Behavior**:
- **Filer ALWAYS shows orange dot** (like macOS Finder - always running)
- Other apps show dots when running
- Auto-hides when windows overlap
- Subscribes to `APP_LAUNCHED` and `APP_TERMINATED` events

**WRONG Assumptions**:
- ❌ "Filer is just another app" (NO - it's always running, provides default menu)
- ❌ "Dots are just visual indicators" (they reflect actual running state via EventBus)

### SystemTray

**File**: [`opensef-shell-qt/qml/SystemTray.qml`](file:///C:/Users/hp/Documents/VitusOS-Ares/opensef/opensef-shell-qt/qml/SystemTray.qml)  
**Dropdown**: [`opensef-shell-qt/qml/components/SystemTrayDropdown.qml`](file:///C:/Users/hp/Documents/VitusOS-Ares/opensef/opensef-shell-qt/qml/components/SystemTrayDropdown.qml)

**Behavior**:
- Procedural icons (QML shapes, NO image assets)
- Click opens dropdown with Network/Sound/Battery controls
- Connects to OSFDesktop services (NOT D-Bus directly)

### Filer (File Manager)

**File**: [`opensef/apps/osf-filer-native/Filer.qml`](file:///C:/Users/hp/Documents/VitusOS-Ares/opensef/apps/osf-filer-native/Filer.qml)  
**Main**: [`opensef/apps/osf-filer-native/main.cpp`](file:///C:/Users/hp/Documents/VitusOS-Ares/opensef/apps/osf-filer-native/main.cpp)

**Behavior**:
- Always running (like macOS Finder)
- Uses GNUstep C++ AppKit components (NSButton, NSView, etc.)
- Custom QML window decorations (traffic lights in titlebar)
- Publishes `APP_LAUNCHED` on start

**WRONG Assumptions**:
- ❌ "Use Qt widgets" (NO - use GNUstep AppKit + Qt Quick bridge)
- ❌ "Filer can be quit" (NO - it's always running)

---

## 5. Dependencies - What's Actually Used

### Framework (opensef-framework)

**CMakeLists.txt should have**:
```cmake
# NO external dependencies except pthread
target_link_libraries(opensef-framework PUBLIC pthread)
```

**DO NOT ADD**:
- ❌ Cairo
- ❌ Pango
- ❌ xkbcommon (compositor uses this, not framework)
- ❌ Any GUI libraries

**Framework is PURE C++** - just data structures and event handling.

### Shell (opensef-shell-qt)

**Dependencies**:
- Qt6 (qtbase, qtwayland, qtdeclarative, qtsvg, qt5compat)
- Vulkan (for GPU acceleration)
- wlroots (for Wayland compositor integration)

**Rendering**: Qt Quick QML + Vulkan, **NOT** Cairo/Pango

### Compositor (opensef-compositor)

**Dependencies**:
- wlroots
- Wayland
- Vulkan
- libxkbcommon (for keyboard)
- libinput (for input devices)

**Language**: Pure C (for wlroots compatibility)

### GNUstep AppKit (opensef-gnustep)

**Purpose**: macOS-like AppKit API in C++

**Components**:
- NSObject, NSView, NSControl, NSButton, NSWindow
- Qt Quick bridges (QQuickNSButton, etc.)

**Usage**: Native apps use this, NOT Qt widgets

---

## 6. Build System

### Environment

**NixOS WSL2** with packages from `flake.nix`

**DO NOT**:
- ❌ Assume tools are in PATH without nix shell
- ❌ Add Cairo/Pango to system packages
- ❌ Use `apt` or other package managers

### Build Order (CRITICAL)

```bash
1. opensef-framework    # Pure C++, no deps
2. opensef-compositor   # wlroots + Vulkan
3. opensef-gnustep      # GNUstep C++ AppKit
4. opensef-shell-qt     # Qt Quick shell (needs framework)
5. apps/*               # Native apps (need framework + gnustep)
```

**Build each with**:
```bash
mkdir -p build && cd build
cmake .. -G Ninja
ninja
```

### Scripts

**Location**: [`scripts/`](file:///C:/Users/hp/Documents/VitusOS-Ares/scripts/)

- `setup.sh` - One-time environment setup
- `build.sh` - Build all components in order
- `run.sh` - Launch compositor + shell
- `test.sh` - Verify everything built correctly

---

## 7. Common Mistakes & How to Avoid Them

### Mistake #1: Adding Cairo/Pango

**WRONG**:
```cmake
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(PANGO REQUIRED pango)
```

**RIGHT**:
```cmake
# Framework has NO external dependencies
target_link_libraries(opensef-framework PUBLIC pthread)
```

**Why**: We use Qt Quick for all rendering, not Cairo/Pango.

### Mistake #2: Using D-Bus for Internal Communication

**WRONG**:
```cpp
QDBusConnection::sessionBus().send(message);
```

**RIGHT**:
```cpp
auto* desktop = OSFDesktop::shared();
desktop->eventBus()->publish(OSFEventBus::APP_LAUNCHED, event);
```

**Why**: D-Bus is ONLY for legacy app compatibility (GTK/Qt/Electron), not internal comms.

### Mistake #3: Treating Components as Independent

**WRONG**:
```cpp
// DockController managing its own state
m_runningApps.append(appId);
```

**RIGHT**:
```cpp
// DockController subscribing to framework events
desktop->eventBus()->subscribe(
    OSFEventBus::APP_LAUNCHED,
    [this](const OSFEvent& e) {
        updateDockState();
    }
);
```

**Why**: Unified framework - all state managed centrally.

### Mistake #4: Server-Side Window Decorations

**WRONG**:
```c
// In compositor
draw_titlebar(view);
draw_traffic_lights(view);
```

**RIGHT**:
```qml
// In Filer.qml (client-side)
Rectangle {
    // Custom titlebar
    Row {
        // Traffic light buttons
        Rectangle { color: "#E85D04" } // Close
        Rectangle { color: "#D4A93E" } // Minimize
        Rectangle { color: "#3D5A80" } // Maximize
    }
}
```

**Why**: Apps control their own decorations for flexibility.

### Mistake #5: Assuming APIs Exist

**WRONG**:
```cpp
// Making up an API
desktop->menuManager()->registerMenu(menu);
```

**RIGHT**:
```cpp
// Check OSFDesktop.h first
// If it doesn't exist, ASK THE USER
```

**Why**: Don't hallucinate APIs. Use what exists or ask user to add it.

---

## 8. Design System (Ares)

### Colors

```cpp
// Primary (Mars Mission)
#define SPACE_ORANGE  0xE85D04  // Close button, accents
#define MARS_DUST     0xFB8500
#define MARS_GOLD     0xD4A93E  // Minimize button
#define MISSION_BLUE  0x3D5A80  // Maximize button border
#define LUNAR_GRAY    0xF0F0F0  // Backgrounds

// Warm Ceramic
#define WARM_CERAMIC  0xFAFAF9  // Panel, tray background
#define SOFT_CREAM    0xFBFBFB

// Text
#define TEXT_PRIMARY    0x2D2D2D
#define TEXT_SECONDARY  0x6B6B6B
```

### Typography

**Font**: Inter (for everything except Terminow)

```qml
Text {
    font.family: "Inter"
    font.pixelSize: 13  // Standard
    font.weight: Font.Normal
}
```

### Animations

**Timings**:
- 150ms - Micro-interactions (hover, click)
- 250ms - Standard transitions (fade, slide)
- 350ms - Page transitions
- 500ms - Emphasis animations

**Easing**: `Easing.OutCubic` (most common)

```qml
Behavior on opacity {
    NumberAnimation {
        duration: 250
        easing.type: Easing.OutCubic
    }
}
```

### Dimensions

```qml
// Panel
height: 28

// Dock
height: 90  // Visible
height: 4   // Hidden (auto-hide)

// Corner radius
radius: 8   // Windows
radius: 12  // UI elements
radius: 16  // Dropdowns

// Shadows
blur: 24
offset: 8
```

---

## 9. Release Channels

### uptc (Current Development)

**What it is**: Hybrid approach using existing protocols

**Characteristics**:
- Qt Quick shell
- GNUstep C++ AppKit (started, not complete)
- D-Bus for legacy app compatibility
- Custom protocols being developed
- **NOT production-ready**

### up1 (Future Stable)

**What it is**: Fully native, daily-drivable OS

**Characteristics**:
- Complete GNUstep C++ AppKit
- All native apps
- Custom protocols (no D-Bus for native apps)
- Boot-to-shutdown openSEF experience
- .iso distribution
- **Production target**

**Current work is for uptc, building towards up1.**

---

## 10. Critical Rules

### Rule #1: Read the Vision Document

**Before making ANY changes**, read:
- [`vitusos_complete_vision.md`](file:///C:/Users/hp/Documents/VitusOS-Ares/vitusos_complete_vision.md)

If you don't understand something, **ASK THE USER**.

### Rule #2: Use OSFDesktop::shared()

**EVERY component** must use the unified framework:

```cpp
auto* desktop = OpenSEF::OSFDesktop::shared();
desktop->eventBus()->...
desktop->stateManager()->...
desktop->windowManager()->...
```

**NO exceptions**.

### Rule #3: No Cairo/Pango in Framework

Framework is **pure C++**. Only dependency: `pthread`.

If you see Cairo/Pango in framework code, **it's a bug from previous AI instances**.

### Rule #4: EventBus for All Internal Communication

**Use**:
```cpp
desktop->eventBus()->publish(OSFEventBus::APP_LAUNCHED, event);
```

**NOT**:
```cpp
QDBusConnection::sessionBus().send(...);
```

D-Bus is ONLY for legacy app compatibility.

### Rule #5: Don't Hallucinate

If an API doesn't exist:
1. Check the header files
2. If not there, **ASK THE USER**
3. Don't make it up

### Rule #6: Test Your Changes

Before claiming something works:
1. Actually build it in WSL2
2. Run it
3. Verify windows appear
4. Check for errors

**Don't assume it works**.

### Rule #7: Custom Window Decorations

Apps draw their own decorations in QML.

**NOT** server-side from compositor.

### Rule #8: Filer is Special

- Always running (can't be quit)
- Always shows orange dot in Dock
- Provides default menu when desktop idle
- Like macOS Finder

### Rule #9: Inter Font Everywhere

```qml
font.family: "Inter"
```

Exception: Terminow uses monospace.

### Rule #10: When in Doubt, Ask

**Better to ask** than to break the project with wrong assumptions.

---

## 11. How Previous AIs Broke This Project

### Instance #1: Linux DE Paradigm

**What they did**:
- Implemented D-Bus for all communication
- Made components independent services
- Added systemd units for each component

**Result**: Fragmented mess, not unified system

**Fix**: Removed D-Bus, implemented EventBus, unified via OSFDesktop

### Instance #2: Cairo/Pango Dependencies

**What they did**:
- Added Cairo/Pango to framework CMakeLists
- Wrote rendering code using Cairo
- Added includes to header files

**Result**: Framework depends on GUI libraries (wrong)

**Fix**: Remove all Cairo/Pango, use Qt Quick in shell

### Instance #3: Server-Side Decorations

**What they did**:
- Implemented titlebar rendering in compositor
- Drew traffic lights server-side

**Result**: Inflexible, not app-controlled

**Fix**: Custom QML decorations in each app

### Instance #4: Made Up APIs

**What they did**:
- Created `OSFMenuManager` (doesn't exist)
- Implemented `OSFAnimationEngine` incorrectly
- Added methods to OSFDesktop that weren't there

**Result**: Code doesn't compile

**Fix**: Use actual APIs, ask user before adding new ones

---

## 12. Quick Reference

### File Locations

```
VitusOS-Ares/
├── vitusos_complete_vision.md  ← READ THIS FIRST
├── flake.nix                   ← Nix environment
├── scripts/                    ← Build scripts
│   ├── setup.sh
│   ├── build.sh
│   ├── run.sh
│   └── test.sh
└── opensef/
    ├── opensef-framework/      ← Pure C++ (NO GUI deps)
    │   ├── include/
    │   │   ├── OSFDesktop.h
    │   │   ├── OSFEventBus.h
    │   │   └── ...
    │   └── src/
    ├── opensef-compositor/     ← Pure C, wlroots
    ├── opensef-gnustep/        ← GNUstep C++ AppKit
    ├── opensef-shell-qt/       ← Qt Quick shell
    │   ├── qml/
    │   │   ├── AresPanel.qml
    │   │   ├── Dock.qml
    │   │   ├── SystemTray.qml
    │   │   └── components/
    │   └── src/
    │       ├── PanelController.cpp
    │       └── DockController.cpp
    └── apps/
        ├── osf-filer-native/   ← File manager
        ├── osf-terminal/       ← Terminal
        └── osf-settings/       ← Settings
```

### Key Headers

```cpp
#include <OSFDesktop.h>      // Main framework entry
#include <OSFEventBus.h>     // Event system
#include <OSFStateManager.h> // State management
#include <OSFWindowManager.h> // Window management
```

### Build Commands

```bash
# Enter nix shell (in WSL2)
cd /mnt/c/Users/hp/Documents/VitusOS-Ares
nix develop

# Build all
bash scripts/build.sh

# Run
bash scripts/run.sh

# Test
bash scripts/test.sh
```

---

## 13. When You're Stuck

1. **Read the vision document** again
2. **Check existing code** in similar components
3. **Look at header files** to see what APIs exist
4. **Ask the user** - don't assume or hallucinate
5. **Test your changes** before claiming they work

---

## 14. Success Criteria

You've succeeded if:

✅ Code compiles without errors  
✅ Uses OSFDesktop::shared() for all framework access  
✅ No Cairo/Pango in framework  
✅ EventBus for internal communication  
✅ Custom QML window decorations  
✅ Follows Ares design system  
✅ Actually tested and verified working  
✅ Didn't break existing functionality  

---

## Final Words

**This project is not a traditional Linux DE.**

It's a unified system like macOS. If you approach it with GNOME/KDE assumptions, you WILL break it.

**Read the vision document. Use the existing APIs. Ask when unsure. Test your changes.**

**Good luck, and please don't break this again.**

---

**Document Version**: 1.0  
**Maintained by**: Human architect + AI assistants  
**Last Broken by AI**: January 2026 (multiple instances)  
**Last Fixed**: January 16, 2026
