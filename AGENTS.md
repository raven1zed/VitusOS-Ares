# AGENTS.md — AI Assistant Context & Guidelines
**VitusOS Ares Development**

---

## Project Overview

**VitusOS Ares** is a premium Linux desktop environment built on:
- **openSEF Framework**: Unified event bus and state management
- **wlroots 0.19**: Wayland compositor (Pure C)
- **Qt Quick + Vulkan**: Shell UI (C++)
- **Ares Design Language**: Mars-inspired warm aesthetic

**Current Status**: Phase 3 Complete (Window Decorations & Polish)  
**Next Phase**: Phase 4 — Qt-based Shell UI + Cross-Toolkit Support  
**Target**: v1.0 Stable Release in Q2 2026 (12-16 weeks)

---

## Architecture at a Glance

```
┌─────────────────────────────────────────────────┐
│  Shell UI (Qt Quick)                            │
│  • Panel (global menu + system tray)            │
│  • Dock (app launcher, autohide)                │
│  • DBusMenu integration (Qt/GTK apps)           │
├─────────────────────────────────────────────────┤
│  Compositor (wlroots + Vulkan)                  │
│  • Window management                            │
│  • xdg-decoration (SSD for Electron/GTK)        │
│  • Direct DRM for games                         │
├─────────────────────────────────────────────────┤
│  openSEF Framework                              │
│  • OSFEventBus (pub/sub)                        │
│  • OSFStateManager (single source of truth)     │
│  • OSFDesktop (singleton access)                │
└─────────────────────────────────────────────────┘
```

---

## Current Codebase State

### ✅ Completed (Phase 1-3)

**Compositor** (`opensef-compositor/`):
- Pure C Wayland compositor using wlroots 0.19
- Window management, input handling, layer shell
- Framework integration via C bindings (`OSFFrameworkC.h`)
- Window geometry tracking for spatial features

**Framework** (`opensef-framework/`):
- `OSFEventBus`: Decoupled pub/sub communication
- `OSFStateManager`: Centralized window/app state
- `OSFDesktop`: Singleton access to services
- C/C++ bridge for compositor integration

**Shell** (`opensef-shell/`):
- **Panel**: Global menu bar (28px height, light theme)
  - Multitask button (currently 20px wide — needs to be 80px)
  - Active window title display
  - Clock widget
  - Dropdown menus with rounded bottom corners
  
- **Dock**: Dynamic app launcher (120px surface, 64px pill)
  - SVG icon rendering
  - Dynamic width based on running apps
  - 3px active indicator dots
  - **Autohide**: Implemented but NOT WORKING (needs fix)
  
- **Wallpaper**: Full-screen image rendering

**Window System** (`opensef-base/`):
- Native CSD with 9px rounded corners
- Ares-palette traffic lights:
  - Mars Orange (#D4622A) — Close
  - Mars Gold (#D4A93E) — Minimize
  - Vitus Blue (#4A9FD4) — Maximize
- Interactive hover symbols (×, −, +)

### ⚠️ Known Issues

1. **Dock autohide not working**: Window geometry events not triggering `checkOverlap()`
2. **Multitask button too narrow**: 20px instead of 80px
3. **Multitask view**: Shows orange strip instead of Windows 8-style tiles
4. **Deprecated librsvg functions**: Using `rsvg_handle_render_cairo` instead of `rsvg_handle_render_document`
5. **Lint errors**: Missing includes, undeclared types (systematic cleanup needed)

### 📋 Missing for v1

**Critical (Blockers)**:
- File Manager (`osf-filer`)
- Settings App (`osf-settings`)
- Notification system
- System tray (StatusNotifierItem)
- Application launcher

**Important (Quality)**:
- Terminal emulator
- Keyboard layout switcher
- Audio/brightness controls
- Session management

---

## Active Implementation Plan

**Location**: `docs/implementation_plan.md` (in artifacts folder)

**Current Plan**: Qt-based Shell UI + Cross-Toolkit Support

**Key Decisions**:
1. **Rendering**: Qt Quick with Vulkan backend (via QRhi)
   - NOT Skia (mobile-focused, hard to build)
   - NOT Cairo (maintenance-only, poor performance)
   - Qt chosen for: 600+ FPS, desktop ecosystem, gaming support

2. **Cross-Toolkit Support**:
   - Qt apps: Native DBusMenu
   - GTK apps: `appmenu-gtk-module` + `gmenu-dbusmenu-proxy`
   - Electron apps: Wrapper script with `--enable-features=WaylandWindowDecorations`

3. **System Tray**: macOS-style minimalist (icon-only, contextual)

4. **Gaming**: Direct DRM scanout for Proton/Steam games

---

## Development Guidelines

### When Starting a New Session

1. **Read this file first** to understand current state
2. **Check `docs/V1_READINESS.md`** for progress tracking
3. **Review `docs/implementation_plan.md`** for active plan
4. **Check `README.md`** for latest status updates

### Before Making Changes

1. **Understand the architecture**: Don't break the unified framework pattern
2. **Check existing code**: Reuse `OSFAresTheme.h` constants, don't hardcode
3. **Follow naming conventions**: 
   - Classes: `OSFClassName`
   - Files: `OSFClassName.cpp/h`
   - Events: `category.action` (e.g., `window.focused`)

### Code Style

**C++ (Shell/Framework)**:
- Use `std::` prefix for STL
- Smart pointers: `std::unique_ptr`, `std::shared_ptr`
- Events: Publish via `OSFDesktop::instance()->eventBus()->publish()`
- State: Query via `OSFDesktop::instance()->stateManager()`

**C (Compositor)**:
- Pure C99, no C++ features
- wlroots naming: `osf_server`, `osf_view`, `osf_output`
- Framework integration: Use `OSFFrameworkC.h` C bindings

**Qt Quick (Future Shell)**:
- QML for UI, C++ for controllers
- Use `Qt6::Quick`, `Qt6::DBus`
- Follow Qt naming: `camelCase` for properties, `PascalCase` for types

### Testing

**Before committing**:
```bash
# Build everything
cd opensef
rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Run desktop
bash scripts/run_vitus_ares.sh
```

**Verify**:
- Compositor starts (check `logs/compositor.log`)
- Shell renders (check `logs/shell.log`)
- No crashes in first 30 seconds
- Window decorations visible
- Dock appears at bottom

---

## Common Pitfalls

### ❌ Don't Do This

1. **Don't bypass the framework**: Always use `OSFEventBus` for communication
2. **Don't hardcode colors**: Use `AresTheme::MarsOrange`, not `#D4622A`
3. **Don't use polling**: Subscribe to events instead
4. **Don't break Cairo rendering yet**: Wait for Qt migration plan approval
5. **Don't assume 1080p**: Query actual screen dimensions

### ✅ Do This Instead

1. **Publish events**: `osf_event_publish("window.moved", window_id)`
2. **Use theme constants**: `AresTheme::PanelHeight`, `AresTheme::DockCornerRadius`
3. **Subscribe to events**: `eventBus->subscribe("window.focused", callback)`
4. **Plan migrations**: Create implementation plan, get user approval
5. **Query screen size**: Use `wlr_output_layout_get_box()` or Qt `Screen.width`

---

## File Locations

**Key Files**:
- Compositor: `opensef/opensef-compositor/src/`
- Framework: `opensef/opensef-framework/include/` and `src/`
- Shell: `opensef/opensef-shell/src/`
- Theme: `opensef/opensef-base/include/opensef/OSFAresTheme.h`
- Docs: `docs/`
- Scripts: `scripts/`
- Logs: `logs/` (all logs go here, NOT repo root)

**Important Docs**:
- `README.md`: Project overview, quick start
- `docs/DEVELOPER_GUIDE.md`: Architecture walkthrough
- `docs/V1_READINESS.md`: Progress tracking, timeline
- `docs/implementation_plan.md`: Active implementation plan (in artifacts)

---

## Git Workflow

**Commit Message Format**:
```
type: brief description

- Detailed change 1
- Detailed change 2

Phase X Complete: [milestone name]
```

**Types**: `feat`, `fix`, `docs`, `refactor`, `test`, `chore`

**Example**:
```
feat: implement dock autohide functionality

- Subscribe dock to window.moved and window.resized events
- Add checkOverlap() method to detect window proximity
- Fix hardcoded 1080p screen height assumption

Phase 3 Progress: Dock autohide working
```

---

## Communication with User

### When to Ask for Approval

1. **Architecture changes**: Switching rendering libraries, major refactors
2. **Breaking changes**: Removing features, changing APIs
3. **Uncertain decisions**: Multiple valid approaches, need user preference

### When to Proceed Autonomously

1. **Bug fixes**: Fixing obvious bugs (lint errors, crashes)
2. **Implementation of approved plans**: Following `implementation_plan.md`
3. **Minor improvements**: Code cleanup, performance optimizations

### How to Report Progress

1. **Use task boundaries**: Update `TaskSummary` with accomplishments
2. **Create walkthroughs**: Document major milestones
3. **Update README**: Reflect completed phases
4. **Commit regularly**: Small, focused commits

---

## Current Priorities (January 2026)

### Immediate (This Week)
1. Fix dock autohide (window geometry events)
2. Fix multitask button width (80px)
3. Enable Vulkan renderer in compositor

### Short-term (Next 2 Weeks)
1. Create Qt Quick panel prototype
2. Implement DBusMenu integration
3. Design Windows 8-style multitask view

### Medium-term (Next Month)
1. Migrate shell to Qt Quick
2. Implement system tray (StatusNotifierItem)
3. Create native app template (using SeaDrop)

### Long-term (Q1-Q2 2026)
1. Build essential apps (Filer, Settings, Terminal)
2. Gaming optimizations (direct scanout)
3. Testing, documentation, v1.0 release

---

## Resources

**Documentation**:
- wlroots: https://gitlab.freedesktop.org/wlroots/wlroots
- Qt Quick: https://doc.qt.io/qt-6/qtquick-index.html
- DBusMenu: https://github.com/KDE/libdbusmenu-qt
- Wayland: https://wayland.freedesktop.org/

**Design Inspiration**:
- macOS: System tray, global menu
- Windows 8: Start menu tiles (for multitask view)
- KDE Plasma: DBusMenu integration
- Gamescope: Direct rendering for games

---

## Version History

- **v0.3** (Jan 8, 2026): Phase 3 complete, planning Qt migration
- **v0.2** (Jan 7, 2026): Phase 2 complete, framework integration
- **v0.1** (Dec 2025): Phase 1 complete, basic compositor

---

**Last Updated**: January 8, 2026  
**Next Review**: Before Phase 4 implementation begins

---

## Quick Reference

**Build**: `cmake -S opensef -B opensef/build && cmake --build opensef/build`  
**Run**: `bash scripts/run_vitus_ares.sh`  
**Logs**: `logs/compositor.log`, `logs/shell.log`, `logs/phase3.log`  
**Theme**: `opensef/opensef-base/include/opensef/OSFAresTheme.h`  
**Framework**: `opensef/opensef-framework/include/OSFDesktop.h`

---

**Remember**: This is a desktop OS, not a mobile app. Prioritize performance, gaming support, and cross-toolkit compatibility. The goal is to surpass existing Linux DEs in polish and usability while maintaining the warm, human-centered Ares aesthetic.
