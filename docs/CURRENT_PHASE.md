# VitusOS Ares - Current Phase

**Last Updated**: 2026-01-11  
**Release Channel**: uptc (Upstream Color) - Development  
**Phase**: 4 - Controls & Integration

---

## Phase 4: Controls & Integration (CURRENT)

**Goal**: Functional desktop environment with Qt Quick shell

**Status**: 🚧 **In Progress** - Building uptc release

### Completed in Phase 4
- [x] Qt Quick shell renders correctly (Vulkan)
- [x] Framework initialization (OSFDesktop::shared())
- [x] Event bus integration (WINDOW_FOCUSED events)
- [x] Panel with clock, global menu stubs
- [x] Dock with app launcher
- [x] Window focus events wired up (2026-01-11)

### In Progress
- [ ] Global menu functional (DBusMenu connection)
- [ ] System tray live icons (StatusNotifier watcher)
- [ ] Testing with real Qt/GTK apps

### This Phase Delivers
✅ **uptc release** - Functional desktop using Qt Quick + Linux protocols (DBusMenu, StatusNotifier)

---

## Previous Phases (Completed)

### Phase 1: Framework Foundation ✅
- OSFDesktop singleton
- OSFEventBus (pub/sub)
- OSFStateManager
- C/C++ bridge for compositor

### Phase 2: Windowing Model ✅
- wlroots compositor
- Wayland protocol integration
- Window management
- Input handling

### Phase 3: Shell & Layout ✅
- Panel (top bar)
- Dock (bottom, floating)
- Wallpaper manager
- Layer shell integration

---

## Next Phases (Planned)

### Phase 5: System Services
**Goal**: Boot-to-shutdown integration

**Work**:
- [ ] Boot splash (hide systemd logs)
- [ ] Session manager (login/logout)
- [ ] Service registry (like launchd)
- [ ] IPC across processes

**Delivers**: Complete desktop experience (boot animation → login → desktop → shutdown)

### Phase 6: GNUstep AppKit Fork
**Goal**: Native widget library (up1 release)

**Work**:
- [ ] Fork GNUstep libs-gui
- [ ] Strip Cairo rendering backend
- [ ] Create AppKitBridge.mm (Objective-C++)
- [ ] Integrate NSButton, NSMenu, NSWindow

**Delivers**: **up1 release** - Pure openSEF native APIs (no D-Bus protocols)

### Phase 7: Animation & Polish
**Goal**: Smooth, beautiful experience

**Work**:
- [ ] Implicit animations (CALayer-style)
- [ ] Transaction model (batch updates)
- [ ] Accessibility audit
- [ ] Performance optimization

**Delivers**: Production-ready v1.0

---

## Timeline

| Milestone | Target | Status |
|-----------|--------|--------|
| **uptc release** | **2026-01 (current)** | **🚧 In progress** |
| up1 alpha | 2026-02 | Planned (GNUstep fork) |
| up1 beta | 2026-03 | Planned (AppKit integration) |
| v1.0 stable | 2026-Q2 | Goal |

---

## What Each Phase Means

### uptc (Upstream Color)
**Philosophy**: "Make it work first"
- Uses Linux protocols (DBusMenu, StatusNotifier)
- Qt Quick only (no AppKit yet)
- Functional desktop for developers/early adopters

### up1 (Upstream One)  
**Philosophy**: "Pure unified framework"
- Native openSEF APIs (no D-Bus protocols)
- GNUstep AppKit + Qt Quick rendering
- Complete boot-to-shutdown integration
- Stable release for users

---

## Current Focus (Phase 4 Tasks)

### This Week
1. ✅ Fix window focus events (DONE)
2. 🚧 Wire up StatusNotifier watcher (system tray)
3. 🚧 Connect DBusMenu to real apps (global menu)
4. 🚧 Test with Firefox, Kate, Gedit

### Next Week
1. Polish UI (squircle icons, animations)
2. Performance testing (600 FPS target)
3. Documentation for uptc release
4. Tag uptc-v0.1.0

---

## How to Know What Phase You're In

**Simple rule**: Look at `docs/COCOA_ALIGNMENT_ROADMAP.md`

The roadmap lists all 7 phases with clear ✅/🚧/📋 status markers.

**Current marker**: Phase 4 is marked 🚧 **In Progress**

---

## Phase Completion Criteria

A phase is **only complete** when:
1. ✅ All deliverables implemented
2. ✅ Tested and functional
3. ✅ Documented
4. ✅ Roadmap updated with ✅ marker

**Don't move to next phase** until current phase is truly done.

---

**Reference**: This document is the single source of truth for "what phase are we in?"
