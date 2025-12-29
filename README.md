# VitusOS Ares

**A cohesive Linux desktop. One framework. One design system. One vision.**

<p align="center">
  <a href="https://vitusos.lovable.app"><img src="https://img.shields.io/badge/Website-vitusos.lovable.app-blue?style=flat-square" alt="Website"></a>
  <img src="https://img.shields.io/badge/Status-Pre--Alpha-orange?style=flat-square" alt="Status">
  <img src="https://img.shields.io/badge/Framework-openSEF-blue?style=flat-square" alt="Framework">
  <img src="https://img.shields.io/badge/Theme-Ares-red?style=flat-square" alt="Theme">
  <img src="https://img.shields.io/badge/License-GPL--3.0-green?style=flat-square" alt="License">
</p>

**Website:** [vitusos.lovable.app](https://vitusos.lovable.app)

---

## What is VitusOS?

VitusOS is an attempt to create a **unified, beautiful Linux experience** - inspired by macOS polish, OS1 (Her 2013) minimalism, and The Martian's Ares mission aesthetic.

**The Problem:** Linux GUIs are fragmented - GTK, Qt, Electron, X11 vs Wayland, inconsistent theming.

**The Solution:** Build one framework (**openSEF**), one design language (**Ares**), ship as complete system.

---

## Current Status

| Milestone | Status |
|-----------|--------|
| openSEF Framework (C++) | ✅ Complete |
| Wayland Backend | ✅ Working |
| XDG Shell Window | ✅ Working |
| First Visible Window | ✅ **Done!** |
| Text Rendering | ⏳ Next |
| Input Events | ⏳ Soon |

**Last verified:** December 29, 2024 - NixOS Plasma Wayland

---

## Built with AI

**Full transparency:** Human-AI collaboration.

- **Architect/Vision:** Human ([@raven1zed](https://github.com/raven1zed))
- **Implementation:** Claude Opus & Sonnet (Anthropic)
- **Methodology:** "Vibecoding" - human provides vision, AI writes code

---

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│  VitusOS Apps (SeaDrop, Terminal, Filer)         [C++17]    │
├─────────────────────────────────────────────────────────────┤
│  openSEF Framework                                [C++17]   │
│  ├── opensef-base      (Foundation)                         │
│  ├── opensef-appkit    (GUI Widgets)                        │
│  ├── opensef-backend   (Wayland + Vulkan)                   │
│  └── opensef-ui        (Ares Design System)                 │
├─────────────────────────────────────────────────────────────┤
│  NixOS (Declarative Linux)                                  │
└─────────────────────────────────────────────────────────────┘
```

---

## Ares Theme

Inspired by *The Martian* - **"I'm gonna have to science the shit out of this."**

| Color | Hex | Use |
|-------|-----|-----|
| **Space Orange** | `#E85D04` | Primary accent |
| **Lunar Gray** | `#F0F0F0` | Background |
| **Mission Blue** | `#3D5A80` | Secondary |
| **Space Charcoal** | `#1A1A2E` | Dark mode |

**Font:** Inter (UI), JetBrains Mono (Terminal)

---

## Quick Start

**Requires:** NixOS or Nix on Linux with Wayland session

```bash
# Clone
git clone --recursive https://github.com/raven1zed/vitusos-ares.git
cd vitusos-ares

# Enter dev environment
nix develop --extra-experimental-features "nix-command flakes"

# Build
cmake -B build -G Ninja
cmake --build build

# Test - should show orange window!
./build/opensef/test/hello-window
```

---

## Project Structure

```
vitusos-ares/
├── opensef/                 # GUI framework (C++)
│   ├── opensef-base/        # OSFObject, types
│   ├── opensef-appkit/      # OSFWindow, OSFButton, OSFView
│   ├── opensef-backend/     # Wayland, XDG shell
│   └── opensef-ui/          # Colors, spacing, typography
├── apps/
│   ├── seadrop/             # Clipboard sync
│   ├── vitus-terminal/      # Terminal emulator
│   ├── vitus-filer/         # File manager
│   └── vitus-installer/     # OS installer
├── boot/plymouth/           # Boot animation
└── flake.nix                # Nix dev environment
```

---

## Roadmap

| Phase | Target | Status |
|-------|--------|--------|
| **0** | Scaffolding | ✅ Complete |
| **1** | Build on Linux | ✅ Complete |
| **2** | Hello Window on Wayland | ✅ **Complete!** |
| **3** | Text & Input | 🔄 In Progress |
| **4** | OS1 Visual Effects | ⏳ |
| **5** | Daily Driver | ⏳ |

---

## License

GPL-3.0 - See [LICENSE](LICENSE)

---

<p align="center">
  <strong>Ad astra per aspera</strong><br>
  <em>"Through hardships to the stars"</em><br><br>
  🔴 Ares Mission - Bring it Home 🌍
</p>
