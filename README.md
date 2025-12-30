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

**VitusOS** (or **vOS** for short) is an attempt to create a **unified, beautiful Linux experience** - inspired by macOS polish, OS1 (Her 2013) minimalism, and The Martian's Ares mission aesthetic.

**The Problem:** Linux GUIs are fragmented - GTK, Qt, Electron, X11 vs Wayland, inconsistent theming.

**The Solution:** Build **openSEF** - both a **Desktop Environment** AND a **Framework**.

### Naming

| Name | What it is |
|------|------------|
| **VitusOS / vOS** | The complete operating system |
| **openSEF** | Desktop Environment + GUI Framework |
| **Ares** | First design language & codename |

### openSEF = Desktop Environment + Framework

```
┌─────────────────────────────────────────────────────────────┐
│  openSEF Desktop Environment                                │
│  ├── opensef-compositor   (wlroots Wayland compositor)      │
│  ├── Dock, Menu Bar       (Desktop components)              │
│  └── Workspaces           (Window management)               │
├─────────────────────────────────────────────────────────────┤
│  openSEF Framework                                          │
│  ├── opensef-appkit       (GUI widgets, windows)            │
│  ├── opensef-backend      (Wayland, rendering)              │
│  └── opensef-ui           (Ares design system)              │
└─────────────────────────────────────────────────────────────┘
```

> **Inspired by:** NeXTSTEP and GNUStep - reimagined for modern Wayland.

---

## 🎉 Latest Progress

**December 30, 2024** - Welcome screen working on NixOS Wayland!

| Milestone | Status |
|-----------|--------|
| openSEF Framework (C++) | ✅ Complete |
| Wayland Backend | ✅ Working |
| XDG Shell Window | ✅ Working |
| Text Rendering (FreeType) | ✅ **Done!** |
| Mouse/Keyboard Input | ✅ **Done!** |
| 60fps Animations | ✅ **Done!** |
| Ares Theme Colors | ✅ **Done!** |
| wlroots Compositor | ✅ Code complete |
| Desktop Components | 🔄 Next |

**Verified on:** NixOS + Plasma Wayland (VM)

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
│  ├── opensef-base        (Foundation)                       │
│  ├── opensef-appkit      (GUI Widgets)                      │
│  ├── opensef-backend     (Wayland Client)                   │
│  ├── opensef-ui          (Ares Design System)               │
│  └── opensef-compositor  (wlroots Compositor) ← NEW!        │
├─────────────────────────────────────────────────────────────┤
│  NixOS (Declarative Linux)                                  │
└─────────────────────────────────────────────────────────────┘
```

---

## Ares Theme

Inspired by *The Martian* - **"I'm gonna have to science the shit out of this."**

| Color | Hex | Use |
|-------|-----|-----|
| **Space Orange** | `#E85D04` | Close button, primary |
| **Warm Gold** | `#C3BC19` | Minimize button |
| **Mission Blue** | `#3D5A80` | Maximize button |
| **Lunar Gray** | `#F0F0F0` | Title bar |
| **Soft Cream** | `#FBFBFB` | Background |

**Font:** Inter (UI), JetBrains Mono (Terminal)

---

## Quick Start

**Requires:** NixOS or Nix on Linux with Wayland session

```bash
# Clone
git clone --recursive https://github.com/raven1zed/VitusOS-Ares.git
cd VitusOS-Ares

# Enter dev environment
nix develop

# Build
cmake -B build -G Ninja
cmake --build build

# Test - Welcome screen with Ares theme!
./build/opensef/test/welcome-screen
```

---

## Project Structure

```
VitusOS-Ares/
├── opensef/                    # GUI framework (C++)
│   ├── opensef-base/           # OSFObject, types
│   ├── opensef-appkit/         # OSFWindow, OSFButton
│   ├── opensef-backend/        # Wayland client, text rendering
│   ├── opensef-ui/             # Ares colors, typography
│   └── opensef-compositor/     # wlroots compositor (NEW!)
├── apps/
│   ├── seadrop/                # Clipboard sync
│   ├── vitus-terminal/         # Terminal emulator
│   └── vitus-filer/            # File manager
└── flake.nix                   # Nix dev environment
```

---

## Roadmap

| Phase | Target | Status |
|-------|--------|--------|
| **0-2** | Scaffolding + Hello Window | ✅ Complete |
| **3** | Text & Input | ✅ **Complete!** |
| **4** | Ares Theme Alignment | ✅ **Complete!** |
| **4.5** | wlroots Compositor | ✅ Code complete |
| **5** | Desktop Components (Dock, Menu) | 🔄 **In Progress** |
| **6** | Core Apps | ⏳ Planned |
| **7** | Daily Driver | ⏳ Future |

---

## License

GPL-3.0 - See [LICENSE](LICENSE)

---

<p align="center">
  <strong>Ad astra per aspera</strong><br>
  <em>"Through hardships to the stars"</em><br><br>
  🔴 Ares Mission - Bring it Home 🌍
</p>
