# VitusOS Ares

**openSEF**: Open Seamless Environment Framework  
**Current Release**: Upstream Color (uptc) - Development Channel  
**Next Release**: Upstream One (up1) - Stable

---

## What is VitusOS Ares?

VitusOS Ares is a **unified Desktop Experience** from boot to shutdown - like macOS, not like traditional Linux DEs.

**Not**: GNOME/KDE/XFCE (fragmented components)  
**Is**: Complete integrated system with ONE framework controlling everything

---

## Quick Start

### Prerequisites
- NixOS or Linux with Nix
- WSL2 (for development) or native Wayland

### Build & Run

```bash
# Enter dev environment
nix develop

# Build everything
cd opensef && cmake -B build -G Ninja && cmake --build build

# Run (in WSL2)
bash scripts/run_vitus_ares.sh
```

---

## Architecture

```
┌────────────────────────────────────────────────────┐
│            openSEF Framework (Unified)             │
│  ┌──────────────────────────────────────────────┐  │
│  │         OSFDesktop (Cocoa-like API)          │  │
│  │   EventBus │ StateManager │ WindowManager    │  │
│  └──────────────────────────────────────────────┘  │
│                       ↕                            │
│  ┌──────────────────────────────────────────────┐  │
│  │  Compositor (wlroots + Vulkan)               │  │
│  │  Shell (Qt Quick + Vulkan)                   │  │
│  │  Apps (GNUstep AppKit + Qt Quick)            │  │
│  └──────────────────────────────────────────────┘  │
│                                                    │
│  Boot Animation → Login → Desktop → Shutdown       │
│         (NO systemd logs visible)                  │
└────────────────────────────────────────────────────┘
```

**Key Principle**: Everything uses `OSFDesktop::shared()` API - ONE unified system.

---

## Project Structure

```
VitusOS-Ares/
├── opensef/                    # Core framework
│   ├── opensef-framework/      # Unified API (OSFDesktop, EventBus)
│   ├── opensef-compositor/     # wlroots Wayland compositor
│   ├── opensef-shell-qt/       # Qt Quick shell (Panel, Dock)
│   ├── opensef-base/           # Foundation (geometry, views, layers)
│   ├── opensef-core/           # Animation system
│   └── opensef-appkit/         # Widget library (buttons, windows)
│
├── apps/                       # Native applications
├── scripts/                    # Build & run scripts
├── assets/                     # Icons, fonts, wallpapers
└── docs/                       # Documentation
```

---

## Current Phase

**We are in Phase 4**: Controls & Integration (uptc release)

| Phase | Status | Description |
|-------|--------|-------------|
| 1. Framework Foundation | ✅ Complete | OSFDesktop, EventBus, StateManager |
| 2. Windowing Model | ✅ Complete | Compositor, window management |
| 3. Shell & Layout | ✅ Complete | Panel, Dock, Wallpaper |
| **4. Controls & Integration** | **🚧 Current** | **uptc release - Qt Quick shell functional** |
| 5. System Services | 📋 Next | Boot manager, session, services |
| 6. GNUstep AppKit Fork | 📋 Planned | Widget library (up1 goal) |
| 7. Final Polish | 📋 Planned | Animations, accessibility |

---

## Tech Stack

| Component | Technology | Why |
|-----------|-----------|-----|
| **Framework** | C++ (openSEF) | Unified API (like Cocoa) |
| **Compositor** | C + wlroots + Vulkan | Wayland protocol, GPU rendering |
| **Shell** | Qt Quick + QML | 600+ FPS, modern GPU pipeline |
| **Widget Library** | C++ (transitioning to GNUstep) | macOS-like widgets |
| **IPC** | C++ Event Bus (internal) | Unified pub/sub system |
| **Future** | GNUstep AppKit fork | Native app framework (up1) |

---

## Release Channels

### uptc (Upstream Color) - Current
**Status**: Development / Early Access  
**Goal**: Functional desktop with Qt Quick shell  
**Paradigm**: Uses Linux protocols (DBusMenu, StatusNotifier) for compatibility  
**Timeline**: Active development

### up1 (Upstream One) - Future Stable
**Status**: Planned  
**Goal**: Pure openSEF unified framework  
**Paradigm**: No Linux protocols, native openSEF APIs only  
**Timeline**: After GNUstep AppKit integration

---

## What Makes This Different

### Traditional Linux DE (GNOME/KDE)
```
❌ Fragmented components
❌ D-Bus spaghetti
❌ Each app loads own theme
❌ Manual component configuration
❌ Visible systemd logs on boot
```

### openSEF (VitusOS Ares)
```
✅ ONE unified framework
✅ Central OSFDesktop API
✅ Shared state & resources
✅ Event-driven architecture
✅ Smooth boot animation (no logs)
✅ macOS-like experience
```

---

## Documentation

- **[Architecture Overview](docs/VitusOS%20Ares.md)** - Complete technical architecture
- **[Design Reference](docs/openSEF%20Design%20Reference.md)** - UI/UX guidelines
- **[API Documentation](docs/API.md)** - Framework API reference
- **[GNUstep Plan](docs/gnustep_architecture_plan.md.resolved)** - GNUstep AppKit integration

---

## Development Status (uptc)

### ✅ Working
- [x] Compositor (wlroots + Vulkan)
- [x] Framework (OSFDesktop, EventBus)
- [x] Shell rendering (Qt Quick + Vulkan)
- [x] Panel (global menu stubs)
- [x] Dock (app launcher)
- [x] Window focus events

### 🚧 In Progress
- [ ] Global menu (DBusMenu connection)
- [ ] System tray (StatusNotifier functional)
- [ ] Window title updates (just fixed!)

### 📋 Planned (up1)
- [ ] GNUstep AppKit fork
- [ ] Native openSEF menu/tray APIs
- [ ] Boot splash (hide systemd)
- [ ] Session management

---

## Contributing

See [DEVELOPER_GUIDE.md](docs/DEVELOPER_GUIDE.md) for:
- Build instructions
- Architecture deep-dive
- Coding standards
- Component communication patterns

---

## License

MIT License - See [LICENSE](LICENSE) file

---

## Vision

**"A desktop environment should feel like ONE system, not a collection of apps"**

From the moment you power on until you shutdown, every component speaks the same language through openSEF. No fragmentation, no protocols - just a seamless experience.

Inspired by: macOS Cocoa, NeXTSTEP, Her (OS1)

---

**Current Focus**: Making uptc release functional and polished  
**Next Milestone**: Fork GNUstep AppKit for up1 unified framework
