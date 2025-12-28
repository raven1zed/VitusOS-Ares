# VitusOS Ares

**Bringing Life to the Beautiful Future We Were Promised**

---

## openSEF Framework

**openSEF** = Open SeagrEnv (Seamless Integration Environment) Framework

A modern GUI framework for VitusOS, forked from GNUStep and modernized with:
- Hybrid Objective-C (core) + C++ (apps) architecture
- OS1-inspired minimalist aesthetic
- Hardware-adaptive rendering (Vulkan/OpenGL)
- Wayland-only (no X11 legacy)

---

## Quick Start

```bash
# Enter development environment
nix develop

# Build openSEF framework
cmake -B build -S .
cmake --build build
```

---

## Project Structure

```
VitusOS Ares/
├── opensef/                   # Core GUI framework
│   ├── opensef-base/          # Foundation (forked GNUStep-base)
│   ├── opensef-appkit/        # GUI widgets (forked GNUStep-gui)
│   ├── opensef-backend/       # Wayland + Vulkan
│   └── opensef-ui/            # OS1 design system
├── apps/
│   └── seadrop/               # Pre-installed clipboard sync
├── vitus-shell/               # Desktop compositor
└── nixos-config/              # System configuration
```

---

## Philosophy

- **Daily-driver first** - Built for the creator, shared with the world
- **If it works, don't fix it** - Hybrid ObjC/C++ architecture
- **Wayland-only** - No legacy baggage
- **OS1 + Aqua/Aero** - Beautiful FOSS future
- **macOS-style boot** - No visible logs, polished experience

---

## Pre-installed Apps

- **SeaDrop** - Clipboard sync (core to VitusOS, always running)
- **Filer** - File manager
- **Terminal** - OS1-styled terminal
- **Settings** - Unified system preferences

---

## License

GPL-3.0 (framework) | Components may vary
