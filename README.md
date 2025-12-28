# VitusOS Ares

**A cohesive Linux desktop. One framework. One design system. One vision.**

<p align="center">
  <a href="https://vitusos.lovable.app"><img src="https://img.shields.io/badge/Website-vitusos.lovable.app-blue?style=flat-square" alt="Website"></a>
  <img src="https://img.shields.io/badge/Status-Pre--Alpha-orange?style=flat-square" alt="Status">
  <img src="https://img.shields.io/badge/Framework-openSEF-blue?style=flat-square" alt="Framework">
  <img src="https://img.shields.io/badge/Theme-Ares-red?style=flat-square" alt="Theme">
  <img src="https://img.shields.io/badge/License-GPL--3.0-green?style=flat-square" alt="License">
</p>

🌐 **Website:** [vitusos.lovable.app](https://vitusos.lovable.app) | **Coming soon:** vitusos.qzz.io

---

## 🌋 What is VitusOS?

VitusOS is an attempt to create a **unified, beautiful Linux experience** - inspired by macOS polish, OS1 minimalism, and The Martian's Ares mission aesthetic.

**The Problem:** Linux GUIs are fragmented - GTK2/3/4, Qt5/6, Electron, X11 vs Wayland, inconsistent theming everywhere.

**The Solution:** Build one framework (**openSEF**), one design language (**Ares**), and ship it as a complete system.

---

## 🤖 Built with AI

**Full transparency:** This project is developed through human-AI collaboration.

- **Architect/Vision:** Human ([@raven1zed](https://github.com/raven1zed))
- **Implementation:** Claude Opus & Claude Sonnet (Anthropic)
- **Methodology:** "Vibecoding" - human provides vision, AI writes code

The human acts as **Architect/CEO/CTO** - making design decisions, approving directions, and providing creative vision. The AI acts as **Implementation Engineer** - writing code, solving technical problems, and scaffolding the architecture.

This is an experiment in what one person + AI can build.

---

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────────────────┐
│  VitusOS Apps (SeaDrop, Installer, Terminal)    [C++]      │
├─────────────────────────────────────────────────────────────┤
│  openSEF Framework                               [ObjC/C++] │
│  ├── opensef-base      (Foundation)                        │
│  ├── opensef-appkit    (GUI Widgets)                       │
│  ├── opensef-backend   (Wayland + Vulkan)                  │
│  └── opensef-ui        (Ares Design System)                │
├─────────────────────────────────────────────────────────────┤
│  NixOS (Declarative Linux)                                 │
├─────────────────────────────────────────────────────────────┤
│  Linux Kernel                                              │
└─────────────────────────────────────────────────────────────┘
```

---

## 🎨 Ares Theme

Inspired by *The Martian* (2015) - **"I'm gonna have to science the shit out of this."**

| Color | Hex | Use |
|-------|-----|-----|
| **Space Orange** | `#E85D04` | Primary accent |
| **Lunar Gray** | `#F0F0F0` | Background |
| **Mission Blue** | `#3D5A80` | Secondary |
| **Mars Dust** | `#FB8500` | Highlights |

---

## 🚀 Quick Start

```bash
# Clone with submodules (includes SeaDrop/libseadrop)
git clone --recursive https://github.com/raven1zed/vitusos-ares.git
cd vitusos-ares

# Or if already cloned:
git submodule update --init --recursive

# Enter dev environment (requires Nix)
nix develop

# Build
cmake -B build -G Ninja
cmake --build build

# Test openSEF
./build/opensef/test/hello-window
```

### Building with SeaDrop

```bash
# Enable apps in build
cmake -B build -G Ninja -DBUILD_APPS=ON
cmake --build build

# Run SeaDrop
./build/apps/seadrop/seadrop
```

---

## 📁 Project Structure

```
vitusos-ares/
├── opensef/                 # GUI framework
│   ├── opensef-base/        # Foundation (objects, strings)
│   ├── opensef-appkit/      # Widgets (window, button, view)
│   ├── opensef-backend/     # Wayland + Vulkan rendering
│   └── opensef-ui/          # Ares design system
├── apps/
│   ├── seadrop/             # Clipboard sync (pre-installed)
│   └── vitus-installer/     # OS installer
├── boot/plymouth/           # Boot animation
├── flake.nix                # Nix dev environment
└── CMakeLists.txt           # Build configuration
```

---

## 🎯 Roadmap

| Phase | Target | Status |
|-------|--------|--------|
| **0** | Scaffolding | ✅ Complete |
| **1** | Build on Linux | 🔄 In Progress |
| **2** | Hello Window on Wayland | ⏳ |
| **3** | OS1 Visual Effects | ⏳ |
| **4** | Daily Driver | ⏳ |

**Goal:** Daily-drivable system in 12 months.

---

## 🤝 Contributing

This is currently a solo + AI experiment. If you're interested in contributing:

1. Star the repo to show interest
2. Open issues for feedback
3. PRs welcome after Phase 2

---

## 📜 License

GPL-3.0 - See [LICENSE](LICENSE) for details.

---

## 💭 Philosophy

> *"Building for users, not developers - but welcoming both."*

VitusOS exists because we were promised beautiful, seamless computing and instead got fragmentation. This is one person's attempt (with AI help) to build the future we deserve.

---

<p align="center">
  <strong>Ad astra per aspera</strong><br>
  <em>"Through hardships to the stars"</em><br><br>
  🔴 Ares Mission - Bring it Home 🌍
</p>
