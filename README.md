# VitusOS Ares

<div align="center">

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
> - **AI (Claude/Gemini)**: Code implementation, documentation, debugging, research
>
> We believe in transparency about AI's role in software development. The architectural decisions, design philosophy, and quality assurance are human-driven. AI accelerates implementation but doesn't replace human creativity and judgment.

---

## Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                        VitusOS Ares Desktop                         │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│   ┌─────────────────────────────────────────────────────────────┐   │
│   │           C++ UI Shell (opensef-shell)                      │   │
│   │                                                             │   │
│   │   osf-panel  ─── Global menu bar (Filer Menu Settings Help) │   │
│   │   osf-dock   ─── Bottom dock with app icons                 │   │
│   │                                                             │   │
│   │   • Cairo/Pango rendering                                   │   │
│   │   • Connects via wlr-layer-shell protocol                   │   │
│   │   • AresTheme design system                                 │   │
│   └──────────────────────────┬──────────────────────────────────┘   │
│                              │ Wayland Protocol                     │
│   ┌──────────────────────────┴──────────────────────────────────┐   │
│   │           Pure C Compositor (opensef-compositor)            │   │
│   │                                                             │   │
│   │   server.c    ─── wlroots init, scene graph                 │   │
│   │   output.c    ─── Monitor handling, background              │   │
│   │   view.c      ─── Window management                         │   │
│   │   input.c     ─── Keyboard/mouse handling                   │   │
│   │   layer_shell.c ─ Dock/panel integration                    │   │
│   │   decorations.c ─ Server-side window decorations            │   │
│   │                                                             │   │
│   │   • Direct wlroots 0.19 integration                         │   │
│   │   • Scene graph rendering                                   │   │
│   │   • XDG shell for client windows                            │   │
│   └─────────────────────────────────────────────────────────────┘   │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

### Why Hybrid C/C++?

**Problem:** wlroots uses C99-only syntax (`[static 4]`) that C++ compilers reject.

**Solution:** 
- Compositor core in **pure C** (8 files, ~1,300 lines)
- UI shell as **C++ Wayland clients** (Cairo rendering)

This is the same approach used by labwc, sway, and other production compositors.

---

## Current Status (January 2026)

| Component | Status |
|-----------|--------|
| **Compositor Core** | ✅ Working (windows, input, layer-shell) |
| **Panel** | ✅ Working (menu bar with clock) |
| **Dock** | ✅ Working (placeholder icons) |
| **Widget Library** | ✅ Button, Label, TextField, GlassPanel |
| **Window Tiling** | 🔄 In Progress |
| **Interactivity** | 🔄 In Progress |
| **Wallpaper** | 📋 Planned |

---

## Design System

### Ares Color Palette

| Color | Hex | Use |
|-------|-----|-----|
| **Space Orange** | `#E57C3A` | Primary accent, close button |
| **Mars Gold** | `#D4A93E` | Secondary accent, minimize button |
| **Star White** | `#1A1A1A` | Primary text |
| **Deep Space** | `#FFFFFF` | Window backgrounds |
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

## File Structure

```
VitusOS Ares/
├── opensef/
│   ├── opensef-compositor/     # Pure C Wayland compositor
│   │   ├── src/
│   │   │   ├── main.c          # Entry point
│   │   │   ├── server.c        # Core initialization
│   │   │   ├── output.c        # Monitor handling
│   │   │   ├── view.c          # Window management
│   │   │   ├── input.c         # Keyboard/mouse
│   │   │   ├── layer_shell.c   # Dock/panel support
│   │   │   └── decorations.c   # Window decorations
│   │   ├── include/
│   │   │   └── server.h        # Core data structures
│   │   └── CMakeLists.txt
│   │
│   ├── opensef-shell/          # C++ UI components
│   │   ├── src/
│   │   │   ├── render/OSFSurface.cpp    # Cairo→Wayland bridge
│   │   │   ├── panel/OSFPanel.cpp       # Global menu bar
│   │   │   └── dock/OSFDock.cpp         # Bottom dock
│   │   ├── include/
│   │   │   ├── OSFSurface.h             # Surface API
│   │   │   └── OSFAresTheme.h           # Colors, dimensions
│   │   └── CMakeLists.txt
│   │
│   ├── opensef-appkit/         # Widget library
│   │   └── src/                # OSFButton, OSFLabel, etc.
│   │
│   └── opensef-core/           # Animation framework
│
├── docs/
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

| Phase | Status | Timeline |
|-------|--------|----------|
| **1. Core Compositor** | ✅ Complete | Dec 2025 |
| **2. Cairo Shell** | ✅ Complete | Dec 2025 |
| **3. First Build Test** | ✅ Complete | Jan 2026 |
| **4. Window Management** | 🔄 In Progress | Jan 2026 |
| **5. SeaDrop Integration** | 📋 Planned | Q2 2026 |
| **6. Native Apps** | 📋 Planned | Q3-Q4 2026 |

### Planned Native Apps

| App | Purpose | Priority |
|-----|---------|----------|
| **SeaDrop** | File sharing (AirDrop-style) | P1 — Thesis project |
| **Filer** | File manager | P2 |
| **Terminow** | Terminal emulator | P3 |
| **Settings** | System preferences | P4 |

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
