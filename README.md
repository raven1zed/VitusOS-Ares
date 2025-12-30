# VitusOS Ares

<div align="center">

![VitusOS Ares](https://img.shields.io/badge/VitusOS-Ares-E85D04?style=for-the-badge)
![openSEF](https://img.shields.io/badge/Framework-openSEF-3D5A80?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Pre--Alpha-yellow?style=for-the-badge)

**"Bringing Life to the Beautiful Future We Were Promised"**

*A unified Linux desktop experience combining OS1 minimalism, macOS Aqua vitality, and Ares warmth*

</div>

---

## Vision

VitusOS Ares is not just another Linux desktop. It's the computing experience we were promised:
- **Technology that recedes** into the background (OS1 from *Her*, 2013)
- **Interfaces that delight** with polish and animation (macOS Aqua)
- **Design that feels warm** and human (*The Martian* aesthetic)

### The Ares Experience

When you use VitusOS, you feel:
| Feeling | Source |
|---------|--------|
| **Warmth** | Not cold corporate tech — OS1 + Ares colors |
| **Polish** | Not rough/unfinished — Aqua vitality |
| **Simplicity** | Not overwhelming — OS1 minimalism |
| **Consistency** | Not fragmented chaos — unified design |
| **Purpose** | "Science the shit out of this" — Ares determination |

---

## Design Philosophy

```
┌─────────────────────────────────────────────────────────────┐
│                    VitusOS Ares UI                          │
├─────────────────────────────────────────────────────────────┤
│   OS1 (Her 2013)     +    macOS Aqua    +    Ares Theme     │
│   • Warm colors          • Traffic lights  • Space Orange   │
│   • Minimal chrome       • Animations      • Mars Mission   │
│   • Content-first        • Polish          • Determination  │
│   • Recedes when         • Delightful      • "Science it"   │
│     not needed           • Responsive                       │
└─────────────────────────────────────────────────────────────┘
```

### Color Palette

| Color | Hex | Use |
|-------|-----|-----|
| **Space Orange** | `#E85D04` | Primary accent |
| **Mars Dust** | `#FB8500` | Secondary accent |
| **Mission Blue** | `#3D5A80` | Tertiary accent |
| **Deep Burgundy** | `#621212` | OS1 warmth |
| **Warm Gold** | `#C3BC19` | Highlights |
| **Lunar Gray** | `#F0F0F0` | Backgrounds |

---

## openSEF Framework

**openSEF** (Open Sea Environment Framework) is VitusOS's native GUI framework.

### Architecture (Current)

```
┌─────────────────────────────────────────────┐
│      C++ UI Shell (Cairo/Pango)             │
│   Panel • Dock • Launcher • Settings        │
│        (Wayland clients via layer-shell)    │
├─────────────────────────────────────────────┤
│         Pure C Compositor Core              │
│   wlroots • XDG shell • Layer shell         │
│        (Window management + rendering)      │
└─────────────────────────────────────────────┘
```

### Components

| Component | Description | Status |
|-----------|-------------|--------|
| `opensef-compositor` | Wayland compositor (C) | 🔄 In Progress |
| `opensef-shell` | Desktop UI (C++/Cairo) | 🔄 In Progress |
| `opensef-base` | Foundation classes | 📋 Planned |
| `opensef-appkit` | Widget library | 📋 Planned |

---

## SeaDrop

**SeaDrop** is VitusOS's native AirDrop-style file sharing.

*Priority #1 proof-of-concept application for openSEF framework.*

| Feature | Description | Status |
|---------|-------------|--------|
| Local Discovery | Find nearby devices | 📋 Planned |
| Encrypted Transfer | Secure file sharing | 📋 Planned |
| Cross-Device | Works across VitusOS ecosystem | 📋 Planned |

---

## Roadmap

### Phase 1-3: GUI Framework + SeaDrop *(Current)*
- [x] Architecture research and design
- [x] wlroots compositor structure
- [x] Pure C compositor core
- [ ] Cairo rendering pipeline
- [ ] Global menu panel
- [ ] Dock with app icons
- [ ] SeaDrop v1.0

**Target:** Working demo by mid-2026

### Phase 4-5: Native Apps
- [ ] Filer (file manager)
- [ ] Terminal (OS1-styled)
- [ ] Text Editor
- [ ] System Settings

**Target:** Daily-drivable alpha by Q4 2026

### Phase 6+: Security & Polish
- [ ] AbuSE security layer
- [ ] sue mode (security hardening)
- [ ] Full Sea* suite

**Target:** 2027+

---

## Progress Log

### December 31, 2024 — Architecture Pivot

**Change:** Migrated from pure C++ to hybrid C/C++ architecture.

**Reason:** wlroots (compositor library) uses C99-only syntax incompatible with C++ compilers. After multiple workaround attempts, we implemented:

1. **Pure C compositor core** (7 files, ~44KB)
   - `main.c`, `server.c`, `output.c`, `view.c`
   - `input.c`, `layer_shell.c`, `decorations.c`

2. **C++ UI as Wayland clients** (Cairo/Pango)
   - `OSFSurface.h` — Layer-shell integration
   - `OSFAresTheme.h` — Complete color/dimension system

**Next:** Test compositor build, implement Cairo rendering.

---

### December 30, 2024 — Initial Research

- Completed openSEF Design Reference document
- Analyzed OS1, macOS Aqua, and Gershwin desktop
- Established color palette and design system

---

## Building

### Requirements
- NixOS (recommended) or Linux with Nix
- wlroots, Wayland, Cairo, Pango

### Quick Start

```bash
git clone https://github.com/raven1zed/vitusos-ares.git
cd vitusos-ares
nix develop

# Build compositor
cd opensef/opensef-compositor
mkdir build && cd build
cmake .. -G Ninja && ninja

# Run
WLR_BACKENDS=wayland ./opensef-compositor
```

---

## References

### Design Inspiration
- **OS1** — [Her (2013) UI Analysis](https://scifiinterfaces.com/category/her-2013/)
- **Aqua** — [512pixels Aqua Screenshot Library](https://512pixels.net/projects/aqua-screenshot-library/)
- **Gershwin** — [GhostBSD Gershwin Desktop](https://github.com/gershwin-desktop/gershwin-desktop)

### UI Mockups
Located in `ui-design/` folder:
- `Desktop.png` — Full desktop with dock, windows
- `Onboarding-Welcome Screen.png` — Setup experience
- `Lockscreen.png` — Lock screen
- `Bootscreen.png` — Plymouth boot splash
- `Shutdown Screen.png` — Power off view

---

## Contributing

VitusOS is in early development. Contributions welcome!

- **Code:** C (compositor), C++ (UI shell)
- **Design:** UI/UX mockups and feedback
- **Testing:** NixOS builds and bug reports

---

## License

MIT License © 2024-2025 VitusOS Project

---

<div align="center">

*The interface recedes when not needed (OS1)*  
*Delights when interacted with (Aqua)*  
*Feels warm and human (Ares)*

**VitusOS Ares** — *Reaching for Mars*

</div>
