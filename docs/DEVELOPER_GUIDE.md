# VitusOS Ares Developer Guide

**A comprehensive guide to understanding and contributing to VitusOS Ares**

*Last Updated: January 7, 2026*

---

## Table of Contents

1. [Project Overview](#project-overview)
2. [Unified Architecture](#unified-architecture)
3. [The Framework (openSEF)](#the-framework-opensef)
4. [Shell Development (C++)](#shell-development-c)
5. [Compositor Development (C)](#compositor-development-c)
6. [Building & Testing](#building--testing)
7. [Contributing](#contributing)
8. [Troubleshooting](#troubleshooting)

---

## Project Overview

VitusOS Ares is a custom Linux desktop environment designed to be:
- **Unified**: All components (Compositor, Panel, Dock, Apps) share a single brain (`opensef-framework`).
- **Polished**: Pixel-perfect UI inspired by macOS Aqua and "Her" (OS1).
- **Responsive**: Event-driven architecture (no polling).

### What is openSEF?
openSEF (open System Environment Framework) is the core library that powers everything. It provides the **Event Bus**, **State Management**, and **UI Widgets**.

> **Think of openSEF as the "Cocoa" of VitusOS.**

---

## Unified Architecture

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                           VitusOS Ares Desktop                              │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│   ┌─────────────────────────────────────────────────────────────────────┐   │
│   │                    Unified Framework (opensef-framework)             │   │
│   │   [ OSFEventBus ] ──────┬────── [ OSFStateManager ] ──── [ Cache ]  │   │
│   │           ▲             │               ▲                           │   │
│   └───────────┼─────────────┼───────────────┼───────────────────────────┘   │
│               │             │               │                               │
│      Events   │             │ States        │ Queries                       │
│  (Pub/Sub)    │             ▼               │                               │
│               │    ┌───────────────────┐    │                               │
│   ┌───────────┴────┴─┐               ┌──┴────┴────────────┐                 │
│   │  Compositor (C)  │               │   Shell (C++)      │                 │
│   │                  │               │                    │                 │
│   │ • Window Mgmt    │◄─────────────►│ • OSFPanel         │                 │
│   │ • Input          │    Wayland    │ • OSFDock          │                 │
│   │ • Hardware       │  (Rendering)  │ • OSFAppKit        │                 │
│   └──────────────────┘               └────────────────────┘                 │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## The Framework (openSEF)

The Framework is the most important component. It allows disparate parts of the OS to talk to each other without tight coupling.

### 1. The Desktop Singleton
Access the framework from anywhere in the C++ Shell:
```cpp
#include "OSFDesktop.h"

auto* desktop = OpenSEF::OSFDesktop::shared();
```

### 2. The Event Bus
We use a **Topic-Based Publish/Subscribe** model.

**Subscribing to Events:**
```cpp
// In your Shell Component (e.g., OSFPanel)
void OSFPanel::init() {
    auto* bus = OpenSEF::OSFDesktop::shared()->eventBus();
    
    bus->subscribe("window.focused", [this](const OpenSEF::OSFEvent& event) {
        std::string title = event.get<std::string>("title");
        this->updateTitle(title);
    });
}
```

**Publishing Events (from C++):**
```cpp
OpenSEF::OSFEvent event;
event.set("title", "Firefox");
event.set("app_id", "firefox");
bus->publish("window.focused", event);
```

### 3. State Manager
The State Manager holds the "Truth".
```cpp
auto* state = desktop->stateManager();
std::string activeWin = state->getActiveWindowId();
```

---

## Shell Development (C++)

The Shell (`opensef-shell`) includes the Panel, Dock, and other system UI. It uses **Cairo** for rendering and **Wayland Layer Shell** for positioning.

### Key Class: OSFSurface
Wraps Cairo/Wayland boilerplate.
```cpp
opensef::OSFSurface surface("osf-panel");
surface.setLayer(Layer::Top);
surface.setAnchor(Anchor::Top | Anchor::Left | Anchor::Right);

surface.onDraw([](cairo_t* cr, int w, int h) {
    // Draw your UI here using Cairo
    cairo_set_source_rgb(cr, 1, 0, 0); // Red
    cairo_paint(cr);
});
```

### Key Class: OSFAppKit
Re-usable widgets (`OSFButton`, `OSFLabel`, `OSFTextField`).
```cpp
auto btn = OSFButton::create("Start Menu");
btn->setFrame(OSFRect(10, 10, 100, 30));
btn->render(cr);
```

---

## Compositor Development (C)

The Compositor (`opensef-compositor`) is written in **Pure C** to interact directly with `wlroots`.

### Integration with Framework (C Bridge)
The compositor uses the **C Bridge** to talk to the C++ Framework.

```c
#include "OSFFrameworkC.h"

// In view.c (when a window is mapped)
void handle_map(struct wl_listener *listener, void *data) {
    // ... logic ...
    
    // Notify Framework
    void* framework = OSFDesktop_Get(); // C wrapper for singleton
    OSFEventBus_Publish(framework, "window.created", eventData);
}
```

---

## Building & Testing

### Prerequisites
- **NixOS** (Recommended) or Linux with Nix.
- **Wayland Session** (Required).

### Build Procedure
```bash
# 1. Enter Dev Environment
nix develop

# 2. Build Framework & Compositor
cd opensef/opensef-compositor
mkdir -p build && cd build
cmake .. -G Ninja
ninja

# 3. Build Shell
cd ../../opensef-shell
mkdir -p build && cd build
cmake .. -G Ninja
ninja
```

### Running
```bash
# Terminal 1: Compositor (runs nested)
WLR_BACKENDS=wayland ./opensef/opensef-compositor/build/opensef-compositor

# Terminal 2: Panel (connects to compositor)
./opensef/opensef-shell/build/osf-panel

# Terminal 3: Dock
./opensef/opensef-shell/build/osf-dock
```

---

## Troubleshooting

- **"OSFDesktop.h not found"**: Ensure `opensef-framework/include` is in your `CMakeLists.txt` include path.
- **"wl_display_connect failed"**: You are likely running from X11 or without `WAYLAND_DISPLAY` set. Use `WLR_BACKENDS=wayland` when running nested.
- **Linker Errors**: Ensure you are linking against `opensef-framework` in CMake.

---
