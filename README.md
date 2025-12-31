# VitusOS Ares

<div align="center">

![VitusOS Ares](https://img.shields.io/badge/VitusOS-Ares-E85D04?style=for-the-badge)
![openSEF](https://img.shields.io/badge/Framework-openSEF-3D5A80?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Prototype-green?style=for-the-badge)

**"Bringing Life to the Beautiful Future We Were Promised"**

*A unified Linux desktop experience combining OS1 minimalism, macOS Aqua vitality, and Ares warmth*

</div>

---

## What This Is

VitusOS Ares is a **custom Linux desktop environment** built from scratch:
- **Pure C Wayland compositor** using wlroots
- **C++ UI shell** with Cairo/Pango rendering
- **Mars-inspired "Ares" aesthetic** — warm, polished, human

**Current Status (January 2026):** Prototype ready for first build test.

---

## Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                        VitusOS Ares Desktop                          │
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
- Compositor core in **pure C** (7 files, ~44KB)
- UI shell as **C++ Wayland clients** (Cairo rendering)

This is the same approach used by labwc, sway, and other production compositors.

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
│   └── opensef-shell/          # C++ UI components
│       ├── src/
│       │   ├── render/
│       │   │   └── OSFSurface.cpp    # Cairo→Wayland bridge
│       │   ├── panel/
│       │   │   ├── OSFPanel.cpp      # Global menu bar
│       │   │   └── main.cpp          # Panel entry point
│       │   └── core/
│       │       └── OSFAresTheme.cpp  # Theme implementation
│       ├── include/
│       │   ├── OSFSurface.h          # Surface API
│       │   └── OSFAresTheme.h        # Colors, dimensions
│       ├── protocols/
│       │   └── wlr-layer-shell-unstable-v1.xml
│       └── CMakeLists.txt
│
├── flake.nix                   # NixOS development environment
├── README.md                   # This file
└── ui-design/                  # UI mockups
```

---

## Design System

### Ares Color Palette

| Color | Hex | Use |
|-------|-----|-----|
| **Space Orange** | `#E57C3A` | Primary accent, traffic light close |
| **Mars Gold** | `#D4A93E` | Secondary accent, minimize button |
| **Star White** | `#F5F5F5` | Primary text |
| **Deep Space** | `#1A1A1A` | Backgrounds |
| **Lunar Gray** | `#2D2D2D` | Panels, title bars |

### UI Dimensions

| Element | Size |
|---------|------|
| Panel height | 28px |
| Dock height | 64px |
| Window corner radius | 8px |
| Dock corner radius | 16px |
| Traffic light buttons | 12px diameter |

---

## Building

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
mkdir build && cd build
cmake .. -G Ninja && ninja

# Build shell (in new terminal)
cd opensef/opensef-shell
mkdir build && cd build
cmake .. -G Ninja && ninja

# Run compositor
WLR_BACKENDS=wayland ./opensef-compositor

# Run panel (in separate terminal, same Wayland session)
./osf-panel
```

---

## Roadmap

| Phase | Status | Timeline |
|-------|--------|----------|
| **1. Core Compositor** | ✅ Complete | Dec 2025 |
| **2. Cairo Shell** | ✅ Complete | Dec 2025 |
| **3. First Build Test** | 🔄 Next | Jan 2026 |
| **4. Dock Implementation** | 📋 Planned | Q1 2026 |
| **5. SeaDrop Integration** | 📋 Planned | Q2 2026 |
| **6. Native Apps** | 📋 Planned | Q3-Q4 2026 |

---

## Progress Log

### January 1, 2026 — Prototype Ready
- All source files audited and verified
- CMake configurations cleaned up
- Ready for first NixOS build test

### December 31, 2025 — Architecture Pivot
- Migrated from pure C++ to hybrid C/C++
- Created 7-file pure C compositor core
- Implemented OSFSurface Cairo→Wayland bridge
- OSFPanel with global menu items

### December 30, 2025 — Initial Research
- Completed design system analysis
- Established Ares color palette
- Created OSFAresTheme.h

---

## Native Apps (Planned)

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

## License

MIT License © 2025-2026 VitusOS Project

---

<div align="center">

*The interface recedes when not needed (OS1)*  
*Delights when interacted with (Aqua)*  
*Feels warm and human (Ares)*

**VitusOS Ares** — *Reaching for Mars* 🚀

</div>
