# openSEF - Open SeagrEnv Framework

## What is openSEF?

**openSEF is NOT a Linux Desktop Environment.**

openSEF is a **complete Desktop Experience framework** that provides seamless integration from the moment the machine boots until it shuts down. No systemd logs. No fragmented components. No Linux paradigms.

Think **macOS**, not GNOME/KDE/XFCE.

---

## The Vision

### What Users Experience

**Boot**:
- Machine powers on
- Beautiful boot animation (no systemd logs)
- Smooth transition to login screen
- No terminal, no text, no "Starting service..."

**Login**:
- Premium lockscreen with password field
- Smooth fade-in animation
- Instant transition to desktop

**Desktop**:
- Cohesive, integrated environment
- Global menu bar (like macOS)
- Dock with running applications
- Everything feels like ONE system, not separate apps

**Shutdown**:
- Smooth fade-out animation
- Clean shutdown (no systemd logs)
- Machine powers off gracefully

### What Developers Get

**Unified API**:
```cpp
auto* desktop = OSFDesktop::shared();
auto windows = desktop->windowManager()->allWindows();
desktop->eventBus()->subscribe("window.created", handler);
auto color = desktop->themeManager()->primaryColor();
```

**Single Framework**:
- One API for everything
- Centralized state management
- Event-driven architecture
- Shared resources (icons, fonts, themes)
- Service discovery

**No Fragmentation**:
- Not "compositor + shell + apps"
- ONE integrated system
- Components communicate through framework
- Shared state across all parts

---

## Architecture

### The Old Way (Linux DE Paradigm) ❌

```
Compositor (Wayland/X11)
    ↓ (protocols only)
Window Manager
    ↓ (D-Bus maybe?)
Panel
    ↓ (???)
Applications
    ↓ (each doing their own thing)
```

**Problems**:
- Fragmented components
- No shared state
- Manual IPC
- Each component loads own theme
- No unified experience

### The openSEF Way (Desktop Experience) ✅

```
┌─────────────────────────────────────────┐
│         openSEF Framework               │
│  ┌───────────────────────────────────┐  │
│  │      OSFDesktop (Singleton)       │  │
│  ├───────────────────────────────────┤  │
│  │  • Event Bus                      │  │
│  │  • State Manager                  │  │
│  │  • Window Manager                 │  │
│  │  • Service Registry               │  │
│  │  • Resource Cache                 │  │
│  │  • Theme Manager                  │  │
│  └───────────────────────────────────┘  │
└──────────┬──────────┬──────────┬────────┘
           │          │          │
      Compositor   Shell    Applications
```

**Benefits**:
- Single source of truth
- Unified API
- Shared resources
- Event-driven
- Feels like ONE system

---

## Core Principles

### 1. **Seamless Integration**
Everything works together. No visible seams between components.

### 2. **No Linux Paradigms**
- No systemd logs visible to users
- No fragmented "desktop environment"
- No manual component configuration
- No D-Bus spaghetti

### 3. **macOS-Style Experience**
- Boot animation, not boot logs
- Integrated UI, not separate apps
- Unified theme, not per-app theming
- Central API, not protocols

### 4. **Developer-Friendly**
- One API to learn
- Clear documentation
- Event-driven architecture
- Easy to extend

### 5. **User-Focused**
- Beautiful from boot to shutdown
- No technical details exposed
- Smooth animations
- Consistent experience

---

## Components

### opensef-framework
**The Core** - Unified Desktop Environment API

Provides:
- `OSFDesktop` - Central singleton
- `OSFEventBus` - Event system
- `OSFStateManager` - State management
- `OSFWindowManager` - Window lifecycle
- `OSFServiceRegistry` - Service discovery
- `OSFResourceCache` - Shared resources
- `OSFThemeManager` - Unified theming

### opensef-compositor
**Display Server** - Wayland compositor using wlroots

Responsibilities:
- Window rendering
- Input handling
- Output management
- Registers windows with framework

### opensef-shell
**Desktop UI** - Panel, dock, wallpaper

Components:
- Global menu bar
- Application dock
- System tray
- Wallpaper manager

Uses framework for:
- Window state queries
- Event subscriptions
- Theme consistency

### opensef-appkit
**Application Framework** - Widget library for apps

Provides:
- Buttons, labels, containers
- Window management
- Event handling
- Theme integration

### Applications
**Built-in Apps** - Lockscreen, settings, file manager, etc.

All apps:
- Use opensef-framework API
- Subscribe to desktop events
- Share resources
- Follow unified theme

---

## What Makes openSEF Different

### vs. GNOME/KDE/XFCE

| Aspect | Linux DEs | openSEF |
|--------|-----------|---------|
| Architecture | Fragmented components | Unified framework |
| State | Each component maintains own | Centralized state manager |
| Communication | D-Bus, protocols | Event bus + API |
| Resources | Each loads own | Shared cache |
| Theme | Per-app configuration | Unified theme manager |
| Boot | systemd logs visible | Smooth boot animation |
| Experience | Collection of apps | Integrated system |

### vs. macOS

| Aspect | macOS | openSEF |
|--------|-------|---------|
| Philosophy | ✅ Integrated experience | ✅ Integrated experience |
| API | Cocoa/AppKit | openSEF Framework |
| Open Source | ❌ Proprietary | ✅ Open Source |
| Platform | macOS only | Linux |

**openSEF is the open-source macOS-style experience for Linux.**

---

## Development Philosophy

### What We Build

✅ **Integrated systems** - Components work together seamlessly  
✅ **Unified APIs** - One way to do things  
✅ **Event-driven** - Loose coupling, high cohesion  
✅ **User-focused** - Beautiful, smooth, polished  
✅ **Developer-friendly** - Clear, documented, consistent  

### What We DON'T Build

❌ **Fragmented components** - No "compositor + shell + WM"  
❌ **Protocol spaghetti** - No manual D-Bus/Wayland IPC  
❌ **Per-app configuration** - No separate theme files  
❌ **Visible systemd** - No boot logs for users  
❌ **Linux DE paradigms** - We're building an experience, not a DE  

---

## Getting Started

### For Users

1. Boot VitusOS
2. See beautiful boot animation
3. Login at lockscreen
4. Enjoy integrated desktop
5. Shutdown smoothly

**No configuration needed. It just works.**

### For Developers

```cpp
#include <OSFDesktop.h>

int main() {
  auto* desktop = OSFDesktop::shared();
  desktop->initialize();
  
  // Subscribe to events
  desktop->eventBus()->subscribe("window.created", [](auto& e) {
    std::cout << "New window!" << std::endl;
  });
  
  // Query state
  auto windows = desktop->windowManager()->allWindows();
  
  // Use theme
  auto color = desktop->themeManager()->primaryColor();
  
  desktop->run();
}
```

**One API. One framework. One system.**

---

## The Bottom Line

**openSEF is not trying to be another Linux Desktop Environment.**

**openSEF is building a complete Desktop Experience that happens to run on Linux.**

The difference matters.

---

## Learn More

- [Developer Guide](DEVELOPER_GUIDE.md) - How to build with openSEF
- [API Reference](API.md) - Complete API documentation
- [Architecture](openSEF%20Architecture%20R%26D.md) - Technical deep dive

---

**Welcome to the future of Linux desktop computing.**
