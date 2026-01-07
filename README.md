# VitusOS Ares

<div align="center">

<img src="resources/vitusos-logo.png" alt="VitusOS Logo" width="120">

![VitusOS Ares](https://img.shields.io/badge/VitusOS-Ares-E85D04?style=for-the-badge)
![openSEF](https://img.shields.io/badge/Framework-openSEF-3D5A80?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Prototype-green?style=for-the-badge)

**"Bringing Life to the Beautiful Future We Were Promised"**

*A unified Linux desktop experience combining OS1 minimalism, macOS Aqua vitality, and Ares warmth*

</div>

---

## About

VitusOS Ares is a **complete linux distro with openSEF that's work both as a desktop environment and a framework**
- **Pure C Wayland compositor** using wlroots 0.19
- **C++ UI shell** with Cairo/Pango rendering
- **Mars-inspired "Ares" aesthetic** — warm, polished, human

### AI-Assisted Development

> **Transparency Notice**
> 
> This project is developed by **[@raven1zed](https://github.com/raven1zed)** (Human Architect) with significant AI assistance from **Claude** (Anthropic) and **Gemini** (Google DeepMind).
>
> - **Human (@raven1zed)**: Vision, design direction, architecture decisions, code review, testing
> - **AI (Claude/Gemini/ChatGPT)**: Code implementation, documentation, debugging, research
>
> We believe in transparency about AI's role in software development. The architectural decisions, design philosophy, and quality assurance are human-driven. AI accelerates implementation but doesn't replace human creativity and judgment.

### Sponsors

<a href="https://kilocode.ai">
  <img src="https://kilocode.ai/logo.png" alt="Kilo Code" width="120">
</a>

**VitusOS Ares is proudly sponsored by [Kilo Code](https://kilocode.ai)** through their Open Source Sponsorship Program.

- 🏆 **Seed Level Sponsor** — Kilo Enterprise (5 seats, $9,000/year value)
- 🔍 Unlimited AI-powered Code Reviews
- 🤝 Supporting open source innovation

*Thank you to Brian Turcotte and the Kilo team for believing in this project!*

---

## Architecture

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                           VitusOS Ares Desktop                              │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│   ┌─────────────────────────────────────────────────────────────────────┐   │
│   │                    Unified Framework (opensef-framework)            │   │
│   │                                                                     │   │
│   │   [ OSFEventBus ] ──────┬────── [ OSFStateManager ] ──── [ Cache ]  |   │
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
├─────────────────────────────────────────────────────────────────────────────┤
│   Key:   ─── Framework Communication (Events)    ... Wayland Protocol       │
└─────────────────────────────────────────────────────────────────────────────┘
```

### The Unified Architecture

VitusOS Ares uses a **Unified Framework Architecture** that bridges the gap between the C-based Compositor (wlroots) and the C++ UI Shell.

- **opensef-framework**: The central nervous system.
  - **OSFEventBus**: Decoupled communication. Compositor publishes `window.created`, Shell subscribes to `window.created`.
  - **OSFDesktop**: Singleton access to system state.
- **opensef-compositor**: A pure C Wayland compositor that "speaks" Framework. It publishes events and registers windows.
- **opensef-shell**: A C++ UI layer that consumes Framework events to update the Panel (active window title) and Dock (running apps).

### Why Hybrid C/C++?

**Problem:** wlroots uses C99-only syntax (`[static 4]`) that C++ compilers reject.

**Solution:** 
- Compositor core in **pure C** (8 files, ~1,300 lines)
- UI shell as **C++ Wayland clients** (Cairo rendering)

This is the same approach used by labwc, sway, and other production compositors.

---

## Current Status (January 7, 2026)

| Component | Status |
|-----------|--------|
| **Framework Foundation** | ✅ Phase 1 Complete |
| **Windowing Integration** | ✅ Phase 2 Complete |
| **Compositor Core** | ✅ Working (Unified Framework Integration) |
| **Panel** | ✅ Phase 4 Complete (Event-driven, active window tracking) |
| **Dock** | ✅ Phase 4 Complete (Event-driven, app lifecycle tracking) |
| **Wallpaper** | ✅ Working (image rendering) |
| **Widget Library** | ⚠️ Basic (Button, Label, TextField - Phase 5 incomplete) |
| **Layout + Responder** | 📋 Phase 3 Planned |
| **Theming System** | 📋 Phase 4 Extensions Planned |

### Recent Improvements (January 7, 2026)

- **Unified Framework Integration**: Compositor, Panel, and Dock now communicate entirely via the `OSFEventBus` (no direct coupling).
- **Event-Driven UI**: `OSFPanel` subscribes to `window.focused` to update title; `OSFDock` subscribes to `application.launch` for app tracking.
- **State Management**: Centralized `OSFStateManager` (via `OSFDesktop`) now holds the single source of truth for window/app state.
- **Performance**: Eliminated polling in favor of event subscriptions, reducing idle CPU usage.
- **Stability**: Fixed verify-build issues with `OSFEvent` API usage and include paths.

> **Development Note**: We have successfully refactored the core Shell components (Panel/Dock) to use the new Unified Framework. The next phase (Phase 5) will focus on applying this same framework pattern to standard applications (Filer, Settings, Terminal).

---

## Design System

### Ares Color Palette

| Color | Hex | Use |
|-------|-----|-----|
| **Space Orange** | `#E85D04` | Primary accent, close button |
| **Mars Gold** | `#D4A93E` | Secondary accent, minimize button |
| **Deep Space** | `#1A1A1A` | Dark backgrounds |
| **Star White** | `#FFFFFF` | Window backgrounds, text |
| **Lunar Gray** | `#F5F5F5` | Panels, title bars |

### UI Dimensions

| Element | Size |
|---------|------|
| Panel height | 28px |
| Dock height | 64px |
| Window corner radius | 8px |
| Dock corner radius | 16px |
| Traffic light buttons | 12px diameter |

---

## Getting Started

### Requirements
- NixOS (recommended) or Linux with Nix
- wlroots 0.19+, Wayland, Cairo, Pango

### Quick Start

```bash
# Clone repository
git clone https://github.com/raven1zed/vitusos-ares.git
cd vitusos-ares

# Enter development environment
nix develop

# Build compositor
cd opensef/opensef-compositor
mkdir -p build && cd build
cmake .. -G Ninja && ninja

# Build shell (from project root)
cd ../../opensef-shell
mkdir -p build && cd build
cmake .. -G Ninja && ninja

# Run compositor (nested in existing Wayland session)
WLR_BACKENDS=wayland ./opensef-compositor

# Run panel (in separate terminal)
./osf-panel

# Run dock (in separate terminal)
./osf-dock
```

### Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| `Alt+Escape` | Quit compositor |
| `Alt+F1` | Cycle window focus |

---

### File Structure

```
VitusOS Ares/
├── opensef/
│   ├── opensef-framework/      # Unified Event & Service Bus (The Core)
│   │   ├── include/
│   │   │   ├── OSFDesktop.h        # Main singleton entry point
│   │   │   ├── OSFEventBus.h       # Event subscription system
│   │   │   ├── OSFStateManager.h   # Central state repository
│   │   │   ├── OSFResourceCache.h  # Shared assets
│   │   │   └── OSFFrameworkC.h     # C bindings for Compositor
│   │   └── src/
│   │
│   ├── opensef-compositor/     # Pure C Wayland Compositor
│   │   ├── src/
│   │   │   ├── main.c              # Entry point & Framework init
│   │   │   ├── server.c            # wlroots backend setup
│   │   │   ├── view.c              # Window management (tracks windows)
│   │   │   ├── layer_shell.c       # Panel/Dock positioning
│   │   │   └── ...
│   │
│   ├── opensef-shell/          # Native C++ UI Shell
│   │   ├── src/
│   │   │   ├── panel/OSFPanel.cpp  # Event-driven top bar
│   │   │   ├── dock/OSFDock.cpp    # Event-driven dock
│   │   │   └── render/OSFSurface.cpp
│   │
│   ├── opensef-base/           # Foundation Classes
│   │   └── include/
│   │       ├── OSFApplication.h    # App lifecycle base
│   │       └── OSFNotification.h
│   │
│   ├── opensef-appkit/         # Widget Toolkit (Buttons, Labels)
│   ├── opensef-core/           # Animation & Render Layers
│   └── opensef-auth/           # Future Authentication (PAM)
│
├── apps/                       # Standard Applications
│   ├── osf-filer/
│   ├── osf-settings/
│   └── osf-terminal/
│
├── docs/                       # Documentation
│   ├── DEVELOPER_GUIDE.md      # Comprehensive dev docs
│   └── API.md                  # API reference
│
├── flake.nix                   # NixOS dev environment
└── README.md                   # This file
```

---

## Documentation

- **[Developer Guide](docs/DEVELOPER_GUIDE.md)** — Architecture, code walkthrough, contribution guide
- **[API Reference](docs/API.md)** — OSFSurface, widgets, theme constants
- **[Design Reference](openSEF%20Design%20Reference.md)** — UI/UX specifications

---

## Roadmap

**See [VitusOS Ares.md](VitusOS%20Ares.md) for the full 9-Phase Roadmap.**

| Phase | Focus | Status |
|-------|-------|--------|
| **1** | Framework Foundation | ✅ **Complete** |
| **2** | Windowing Integration | ✅ **Complete** |
| **3** | Layout & Input | ✅ **Complete** |
| **4** | Shell Integration (Refactor) | ✅ **Complete** |
| **5** | Application Integration | 📋 **Next Up** |
| **6** | Service System | 📋 **Planned** |
| **7** | Resource Management | 📋 **Planned** |
| **8** | Testing & Verification | 📋 **Planned** |
| **9** | Documentation | 📋 **Planned** |

> **Note**: Phase 4 involved refactoring the early prototype Shell (from Phase 7 original plan) to fully utilize the new Framework, ensuring a robust foundation before building more apps.

---

## Design Inspiration

- **OS1 (Her, 2013):** Warmth, minimalism, technology that recedes
- **macOS Aqua:** Polish, animations, "lickable" UI
- **The Martian:** Determination, "science the shit out of this"

---

## Contributing

We welcome contributions! Please see our [Developer Guide](docs/DEVELOPER_GUIDE.md) for:
- Code style guidelines
- Commit message format
- Pull request process

### Build & Test

```bash
nix develop
cd opensef && mkdir -p build && cd build
cmake .. -G Ninja -DBUILD_TESTING=ON
ninja
```

---

## License

MIT License © 2025-2026 VitusOS Project

---

<div align="center">

**VitusOS Ares** — *Reaching for Mars* 

---

<sub>
Developed by <a href="https://github.com/raven1zed">@raven1zed</a> (Human Architect) with AI assistance from Claude (Anthropic) & Gemini (Google DeepMind)
</sub>

</div>
