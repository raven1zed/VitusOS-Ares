# VitusOS Ares - openSEF Desktop Environment

> **Status: EXPERIMENTAL / IN DEVELOPMENT**  
> Pure C wlroots compositor with Cairo C++ UI layer

---

## 🚨 Honest Status

| Component | Status | Compiles? | Runs? |
|-----------|--------|-----------|-------|
| **C Compositor Core** | 70% | ⚠️ Untested | ⚠️ Untested |
| **C++ Shell (Cairo)** | 20% | ❌ Incomplete | ❌ No |
| **Old C++ Compositor** | ❌ Abandoned | ❌ No (C99 errors) | ❌ No |

### What Happened

The original C++ compositor hit a **fundamental incompatibility**: wlroots uses C99 `[static N]` array syntax that **cannot be compiled by any C++ compiler** (clang, GCC, MSVC all reject it).

After multiple failed attempts to work around this, we pivoted to:
1. **Pure C compositor core** (7 new files)
2. **C++ UI as separate Wayland clients** (Cairo-based)

---

## Architecture

```
┌─────────────────────────────────────────────┐
│           C++ UI Layer (Cairo)              │
│  osf-panel / osf-dock / osf-launcher        │
│         (Wayland clients via layer-shell)   │
├─────────────────────────────────────────────┤
│           Pure C Compositor                 │
│  main.c → server.c → output.c → view.c      │
│  input.c → layer_shell.c → decorations.c    │
│            (wlroots integration)            │
└─────────────────────────────────────────────┘
```

---

## File Inventory

### New Pure C Compositor (Phase 8) ✅
```
opensef/opensef-compositor/
├── include/server.h          # Data structures
└── src/
    ├── main.c                # Entry point (2.9KB)
    ├── server.c              # Init + scene graph (8.2KB)
    ├── output.c              # Monitor handling (3.7KB)
    ├── view.c                # Window mgmt (9.4KB)
    ├── input.c               # Keyboard + mouse (11.3KB)
    ├── layer_shell.c         # Dock/panel (5.9KB)
    └── decorations.c         # SSD (2.2KB)
```
**Total: ~44KB of C code**

### C++ Shell Foundation (In Progress)
```
opensef/opensef-shell/
├── CMakeLists.txt
├── include/
│   ├── OSFSurface.h          # Cairo layer surface
│   └── OSFAresTheme.h        # Theme constants
└── protocols/
    └── wlr-layer-shell-unstable-v1.xml
```
**Missing:** All `.cpp` implementations

### Old C++ Code (Abandoned)
```
opensef/opensef-compositor/src/*.cpp  # 25 files, ~120KB
```
These do NOT compile due to wlroots C99 incompatibility.

---

## To Make This Work

### Step 1: Test C Compositor
```bash
cd ~/vitusos-ares
nix develop
cd opensef/opensef-compositor
rm -rf build && mkdir build && cd build
cmake .. -G Ninja && ninja
WLR_BACKENDS=wayland ./opensef-compositor
```

### Step 2: If It Works
- Add C++ shell implementations
- Connect shell to compositor via layer-shell
- Port animation framework from old code

### Step 3: If It Fails
- Debug C compilation errors
- May need more wlroots-specific fixes

---

## Realistic Assessment

| Task | Effort | Risk |
|------|--------|------|
| Get C compositor running | 2-4 hours | Medium |
| Implement Cairo shell | 8-16 hours | Medium |
| Port animations | 4-8 hours | Low |
| Full desktop experience | 40+ hours | High |

**Total to MVP:** ~60-80 hours of focused work

---

## Dependencies (NixOS)

```nix
wlroots
wayland, wayland-protocols
cairo, pango, librsvg
libxkbcommon, libdrm, libinput
```

---

## License

MIT License - VitusOS Project
