# openSEF Architecture R&D

**The Complete Research Foundation for Building a Premium Desktop Environment**

*Last Updated: December 31, 2025*

---

## Table of Contents

1. [Architecture Pivot - December 2024](#architecture-pivot---december-2024)
2. [How Other DEs Solved These Problems](#how-other-des-solved-these-problems)
3. [Rendering Pipeline Analysis](#rendering-pipeline-analysis)
4. [GNUStep Deep Dive](#gnustep-deep-dive)
5. [macOS Cocoa & Core Animation](#macos-cocoa--core-animation)
6. [GhostBSD Gershwin Study](#ghostbsd-gershwin-study)
7. [Implementation Roadmap](#implementation-roadmap)
8. [Boot, Lock Screen & Shutdown UI](#boot-lock-screen--shutdown-ui)

---

# Architecture Pivot - December 2024

## The Problem We Hit

```
┌─────────────────────────────────────────────────────────────────────┐
│  wlroots C99 Syntax Incompatible with C++ Compilers                 │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  wlroots uses C99-only syntax:                                      │
│    void wlr_renderer_clear(const float color[static 4]);            │
│                                          ^^^^^^                     │
│  This syntax is:                                                    │
│  ❌ Valid C99                                                       │
│  ❌ Invalid C++11/14/17/20 (parse error, not just warning)          │
│  ❌ Cannot be fixed with -fpermissive or -Wno-* flags               │
│                                                                     │
│  Affected versions: ALL (0.16, 0.17, 0.18, 0.19)                   │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

## Attempts Made (All Failed)

| Attempt | Result |
|---------|--------|
| `-fpermissive` flag | ❌ Parse error, not warning |
| `-Wno-c99-extensions` | ❌ Clang only, doesn't help parser |
| Switch GCC ↔ Clang | ❌ Both reject syntax |
| Older wlroots (0.16, 0.17) | ❌ Same syntax present |
| `extern "C"` wrapping | ❌ Doesn't fix array syntax |
| C wrapper layer | ⚠️ Partial, but too complex |

## Solution Chosen: Hybrid C/C++ Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│  NEW ARCHITECTURE (Post-Pivot)                                      │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌─────────────────────────────────────────────────────────────┐   │
│  │              C++ UI Layer (Cairo/Pango)                      │   │
│  │   osf-panel • osf-dock • osf-launcher • osf-settings         │   │
│  │        (Wayland clients via wlr-layer-shell)                 │   │
│  └──────────────────────────┬──────────────────────────────────┘   │
│                              │                                      │
│                         layer-shell protocol                        │
│                              │                                      │
│  ┌──────────────────────────┴──────────────────────────────────┐   │
│  │              Pure C Compositor Core                          │   │
│  │   main.c → server.c → output.c → view.c                     │   │
│  │   input.c → layer_shell.c → decorations.c                   │   │
│  │              (Direct wlroots integration)                    │   │
│  └─────────────────────────────────────────────────────────────┘   │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

---

# How Other DEs Solved These Problems

## Research: December 31, 2024

### 1. Hyprland — The Nuclear Option

**Problem:** Same C99 compatibility issues, memory bugs, lack of control.

**Solution:** **Completely dropped wlroots** and built custom C++ backend.

```
Timeline:
- 2022-2023: Based on wlroots
- July 2024 (v0.42.0): Fully independent, 100% C++

What they built:
┌─────────────────────────────────────────────────────────────────────┐
│  aquamarine (C++ library)                                           │
│  • Lightweight abstraction layer                                    │
│  • Handles: KMS, DRM, libinput (low-level hardware)                │
│  • All protocol implementations rewritten in C++                    │
│                                                                     │
│  Result: No more wlroots dependency at all                         │
└─────────────────────────────────────────────────────────────────────┘
```

**Why they did it:**
- Reduce memory issues from C implementations
- Better documentation control  
- Faster feature development (explicit sync, HDR)
- Full C++ memory safety

**Lesson for openSEF:** Long-term, we could build our own C++ backend like `aquamarine`. Short-term, hybrid C/C++ works.

---

### 2. Wayfire — The Practical Approach

**Problem:** C++ compositor using C library.

**Solution:** **Use wlroots as-is**, write C++ around it.

```
Architecture:
- 97.2% C++ codebase
- Builds wlroots as submodule
- Core and plugins in C++
- Direct wlroots calls where needed

Reality: Code sometimes resembles "C with classes"
```

**How they avoid the syntax issue:**
- Their C++ code calls wlroots functions but doesn't #include headers with problematic syntax in .cpp files
- Headers are included in .c translation units, not .cpp

**Lesson for openSEF:** Keep wlroots includes isolated in pure C files.

---

### 3. Louvre — The C++ Wrapper

**Problem:** Developers want pure C++ API.

**Solution:** Full C++ library wrapping wlroots concepts.

**Lesson for openSEF:** If we need C++ wlroots interaction, consider building a clean C wrapper library.

---

### 4. qwlroots — Qt Bindings

**Problem:** Qt developers want wlroots integration.

**Solution:** Qt/C++ bindings for wlroots.

**Lesson for openSEF:** We chose Cairo, so qwlroots doesn't apply, but the binding pattern is instructive.

---

## Summary: Industry Solutions

| Project | Approach | Language | Result |
|---------|----------|----------|--------|
| **Hyprland** | Drop wlroots, build `aquamarine` | 100% C++ | Full control |
| **Wayfire** | Use wlroots, write C++ around it | 97% C++ | Works but "C with classes" |
| **Louvre** | C++ wrapper library | C++ | Clean API |
| **Sway** | Stay pure C | 100% C | Rock solid |
| **labwc** | Pure C + external clients | C core | Simple, works |
| **openSEF** | Hybrid C core + C++ UI | C + C++ | Best of both worlds |

---

## openSEF Decision: Why Hybrid?

| Option | Pros | Cons | Verdict |
|--------|------|------|---------|
| **Pure C (like Sway)** | No compat issues | C++ animation code wasted | ❌ |
| **Drop wlroots (like Hyprland)** | Full control | Months of work | ❌ (future) |
| **C++ wrapper (like Louvre)** | Clean API | Still hit C99 syntax | ❌ |
| **Hybrid C/C++ (our choice)** | Works now, preserves C++ UI | Two languages | ✅ |

---

# Current Implementation Status

## Pure C Compositor Core ✅

| File | Lines | Purpose |
|------|-------|---------|
| `server.h` | ~170 | All data structures |
| `main.c` | ~100 | Entry point, CLI |
| `server.c` | ~180 | wlroots init, scene graph |
| `output.c` | ~100 | Monitor handling |
| `view.c` | ~250 | Window management |
| `input.c` | ~300 | Keyboard, mouse |
| `layer_shell.c` | ~170 | Dock/panel support |
| `decorations.c` | ~70 | Server-side decorations |

**Total: ~1,340 lines of pure C**

## C++ Shell Foundation 🔄

| File | Purpose | Status |
|------|---------|--------|
| `OSFSurface.h` | Cairo layer surface | ✅ Header |
| `OSFAresTheme.h` | Theme constants | ✅ Complete |
| `OSFSurface.cpp` | Implementation | ❌ Needed |
| `OSFPanel.cpp` | Global menu | ❌ Needed |
| `OSFDock.cpp` | Dock | ❌ Needed |

---

# Lessons Learned (December 2024)

## 1. Don't Fight Language Boundaries

> wlroots is C. Accept it. Build around it.

Attempting to compile C99 code as C++ is a dead end. The hybrid approach respects language boundaries.

## 2. The Hyprland Path is Ambitious

Building a custom backend like `aquamarine` gives full control but requires:
- Deep understanding of DRM/KMS/libinput
- Months of development
- Ongoing maintenance burden

For openSEF v1.0, hybrid is pragmatic. For v2.0+, custom backend may be worth considering.

## 3. labwc/Sway Architecture Works

These compositors prove that:
- Pure C core is stable and maintainable
- UI can be external Wayland clients
- layer-shell protocol is sufficient for desktop shells

## 4. Cairo for Custom UI

Choosing Cairo over GTK4/Qt gives:
- Full control over rendering
- Matches Ares design system exactly
- No toolkit-imposed design constraints
- Smaller dependency footprint

---

# Next Steps

1. **Test C Compositor Build** — Verify it compiles on NixOS
2. **Implement OSFSurface.cpp** — Cairo rendering to layer-shell
3. **Create OSFPanel** — Global menu bar
4. **Port Animation Framework** — OSFLayer, OSFTransaction to C++
5. **Build OSFDock** — App launcher

---

# Boot, Lock Screen & Shutdown UI

## The Golden Rule

> **openSEF is to VitusOS what Cocoa is to macOS**
>
> Cocoa doesn't replace Darwin/XNU — it makes them invisible.
> openSEF doesn't replace systemd — it makes it invisible.

## Boot-to-Shutdown Flow

```
┌─────────────────────────────────────────────────────────────────┐
│  Power On                                                        │
│  └── BIOS/UEFI → Silent (no POST spam)                          │
│      └── GRUB → Invisible (timeout=0, no menu)                  │
│          └── Kernel → "quiet splash loglevel=0" (no text)       │
│              └── Plymouth → VitusOS boot animation              │
│                  └── systemd → Silent (ShowStatus=no)           │
│                      └── osf-greeter → Lock screen (FIRST UI)   │
│                          └── PAM authenticates                   │
│                              └── osf-compositor launches         │
│                                  └── osf-panel + osf-dock        │
│                                      └── Desktop ready           │
└─────────────────────────────────────────────────────────────────┘
```

## Required Components

| Component | Purpose | Technology |
|-----------|---------|------------|
| Plymouth theme | Boot animation | Plymouth + script |
| osf-greeter | Lock screen / login | C++ Cairo, PAM |
| osf-wallpaper | Desktop background | C++ Cairo, layer-shell |
| osf-shutdown | Shutdown overlay | C++ Cairo, logind D-Bus |

## Kernel & systemd Configuration

```bash
# /etc/default/grub
GRUB_TIMEOUT=0
GRUB_CMDLINE_LINUX_DEFAULT="quiet splash loglevel=0 vt.global_cursor_default=0"

# /etc/systemd/system.conf
ShowStatus=no
```

## Session Entry

```desktop
# /usr/share/xsessions/opensef.desktop
[Desktop Entry]
Name=openSEF
Comment=VitusOS Ares Desktop
Exec=/usr/bin/start-opensef
Type=Application
```

## Success Criteria

User experience from power button:
1. **See**: VitusOS logo + progress bar
2. **See**: Lock screen
3. **Enter password**: Desktop appears instantly
4. **Click shutdown**: Confirmation → "Shutting down..." → off

User **never sees**: GRUB, kernel text, systemd output, terminal.

---

*This document is the foundation for building openSEF. Updated January 1, 2026.*

