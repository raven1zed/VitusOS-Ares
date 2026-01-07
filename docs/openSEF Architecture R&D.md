# openSEF Architecture Reference

**The Unified Framework for VitusOS Ares**

*Last Updated: January 7, 2026*

---

## 1. High-Level Overview

openSEF (open System Environment Framework) is the backbone of VitusOS Ares. It is a **Unified Framework** that bridges the gap between the low-level Display Server (Wayland/wlroots) and the high-level User Interface (Panel, Dock, Apps).

Unlike traditional Linux desktops that glue together disparate components (compositor, panel, notification daemon) via ad-hoc protocols, openSEF connects them all through a single, coherent **Event Bus** and **State Manager**.

### The "Unified" Promise

> **"Everything is connected."**

If a window opens, the Compositor knows. The Panel knows (to update the menu). The Dock knows (to show a running dot). The Filer knows. They all share the same "Brain" — the `OSFDesktop` singleton.

---

## 2. Core Architecture

The architecture consists of three distinct layers, all connected by the **openSEF Framework**.

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                           VitusOS Ares Ecosystem                            │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│   Layer 3: The Shell & Apps (C++)                                           │
│   ┌──────────────┐  ┌──────────────┐  ┌──────────────┐                      │
│   │   OSFPanel   │  │   OSFDock    │  │  OSFSettings │                      │
│   └──────┬───────┘  └──────┬───────┘  └──────┬───────┘                      │
│          │                 │                 │                              │
│          ▼                 ▼                 ▼                              │
│   ┌──────────────────────────────────────────────────────┐                  │
│   │           openSEF Framework (Shared Library)         │                  │
│   │                                                      │                  │
│   │   • OSFEventBus (Pub/Sub)                            │                  │
│   │   • OSFStateManager (Windows, Apps, System)          │                  │
│   │   • OSFResourceCache (Icons, Themes, Fonts)          │                  │
│   │   • OSFDesktop (Singleton Access Point)              │                  │
│   └─────────────────────────▲────────────────────────────┘                  │
│                             │                                               │
│   Layer 2: The Bridge       │                                               │
│   ┌──────────────────────────────────────────────────────┐                  │
│   │          OSFFrameworkC (C Wrapper for Framework)     │                  │
│   └─────────────────────────▲────────────────────────────┘                  │
│                             │                                               │
│   Layer 1: The Compositor (C / wlroots)                     │                  │
│   ┌──────────────────────────────────────────────────────┐  │                  │
│   │                 opensef-compositor                   │  │                  │
│   │   • Window Management (XDG Shell)                    │  │                  │
│   │   • Input Handling                                   │  │                  │
│   │   • Hardware Rendering                               │  │                  │
│   └──────────────────────────────────────────────────────┘  │                  │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

### Layer 1: The Compositor (`opensef-compositor`)
- **Language**: Pure C (C11)
- **Role**: Validates hardware, manages the display, handles raw input, and composite pixels.
- **Framework Integration**: Uses `OSFFrameworkC` to publish events (`OSF_EVENT_WINDOW_CREATED`, `OSF_EVENT_WINDOW_FOCUSED`) and register window states.
- **Crucial Design Choice**: The compositor is *dumb*. It doesn't know *what* it's drawing, it just manages the surfaces. The logic lives in the Framework.

### Layer 2: The Framework (`opensef-framework`)
- **Language**: C++17 (with C bindings)
- **Role**: The "Brain". Holds the Source of Truth.
- **Components**:
    - `OSFEventBus`: A string-topic based generic event system.
    - `OSFStateManager`: standardized structs for `Window`, `Application`, `Monitor`.
    - `OSFDesktop`: The main entry point `OSFDesktop::shared()`.

### Layer 3: The Shell (`opensef-shell`)
- **Language**: C++17
- **Role**: The "Face". Renders the UI using Cairo.
- **Framework Integration**: Subscribes to events.
    - `OSFPanel` subscribes to `window.focused` to update the global menu title.
    - `OSFDock` subscribes to `application.launched` and `window.minimized` to manage dock items.

---

## 3. The Event-Driven Model

The core of openSEF is the **Event Bus**. We do not use polling.

### Example Flow: User Opens a Window (e.g., Firefox)

1.  **Wayland Event**: `opensef-compositor` receives a `map` request from the XDG Shell protocol (Firefox starts).
2.  **Compositor Action**:
    *   Creates an internal `osf_view`.
    *   Calls `OSFDesktop_RegisterWindow(id, title, "firefox")`.
    *   Calls `OSFEventBus_Publish("window.created", {id, title})`.
    *   Give focus -> Calls `OSFEventBus_Publish("window.focused", {id, title})`.
3.  **Framework Action**:
    *   Updates `OSFStateManager` (adds window to list, sets active window ID).
4.  **Shell Action (Panel)**:
    *   `OSFPanel` (listening to `window.focused`) receives the event.
    *   Calls `surface_->requestRedraw()`.
    *   **Next Frame**: Draws "Firefox" in the center of the top bar.
5.  **Shell Action (Dock)**:
    *   `OSFDock` (listening to `window.created`) receives the event.
    *   Checks if "Firefox" icon is already bouncing/present.
    *   Adds a "running indicator" dot below the Firefox icon.

This happens in milliseconds, with zero "asking" or polling.

---

## 4. Key Components Detail

### OSFDesktop (Singleton)
The primary access point.
```cpp
auto* desktop = OpenSEF::OSFDesktop::shared();
desktop->eventBus()->subscribe("topic", callback);
```

### OSFEventBus
A flexible Publish-Subscribe system.
- **Topics**: Hierarchical strings (`window.created`, `system.power.shutdown`).
- **Data**: Generic `OSFEvent` object holding a Key-Value map (`std::map<std::string, std::string>`).

### OSFFrameworkC
The C-compatible wrapper that allows the Pure C compositor to talk to the C++ framework.
```c
// In C compositor
void* framework = OSFDesktop_Get();
OSFEventBus_Publish(framework, "window.created", data);
```

---

## 5. Design Decisions & Rationale

### Why not just use Wayland Protocols for everything?
Wayland protocols are low-level and fragmentation-prone. Building a "Panel Protocol" or "Dock Protocol" for every feature is slow.
**openSEF** bypasses this for trusted internal components. The Panel acts like a part of the OS, not just a random client. It has direct access to the `OSFDesktop` state (in the same process space or shared memory/IPC in the future service model).

*Note: Currently, Shell and Compositor run as separate processes in Phase 4. The Event Bus currently uses IPC (mocked or pipe-based) or is planned to use a shared memory bus in Phase 6. For the implemented Prototype, they are tightly coupled via the build, but cleanly separated by API.*

### Why C++ for the Shell?
- **Cairo/Pango**: Excellent C++ bindings.
- **Productivity**: Classes, Vectors, Maps are superior for UI state management vs raw C structs.
- **Performance**: Zero-overhead abstractions compared to Python/JS bridges.

### Why C for the Compositor?
- **wlroots**: The gold standard library is C99.
- **ABI Stability**: C is stable.
- **Simplicity**: Keeps the crucial "display engine" simple, fast, and dumb.

---

## 6. Directory Structure

- `opensef/opensef-framework`: Core logic (C++).
- `opensef/opensef-compositor`: Display server (C).
- `opensef/opensef-shell`: UI components (C++).
- `opensef/opensef-base`: Shared types/utils.
- `opensef/opensef-appkit`: Widget library.

---

*This document supersedes all previous "R&D" research notes.*
