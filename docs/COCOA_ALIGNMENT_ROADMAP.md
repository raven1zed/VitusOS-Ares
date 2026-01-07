# openSEF Cocoa-Alignment Roadmap (Ares)

**Goal:** Evolve openSEF into a Cocoa-like framework and desktop experience while preserving the Ares design identity.

*Last Updated: January 7, 2026*

---

## Status Overview

| Phase | Goal | Status |
|-------|------|--------|
| **1** | Framework Foundation | ✅ **Complete** |
| **2** | Windowing Model | ✅ **Complete** |
| **3** | Shell & Layout | ✅ **Complete** |
| **4** | Controls & Text | 🚧 **In Progress** |
| **5** | System Services | 📋 **Next Up** |
| **6** | Animation | 📋 **Planned** |
| **7** | Final Polish | 📋 **Planned** |

---

## Phase 1 — Framework Foundation (Foundation/AppKit analogs) ✅

**Objective:** Establish the core application framework and lifecycle APIs.

**Delivered:**
- `opensef-framework` library created.
- `OSFDesktop` singleton (Shared Application).
- `OSFEventBus` (Notification Center equivalent).
- `OSFStateManager` (User options/defaults).

---

## Phase 2 — Windowing Model + Responder Chain ✅

**Objective:** Provide a Cocoa-style window/view system.

**Delivered:**
- `OSFView` abstraction implemented.
- `OSFHelper` for Wayland surface management.
- Window mapping/unmapping logic in Compositor (pure C) bridged to Framework.
- Input event routing (Compositor -> Framework -> Focused Window).

---

## Phase 3 — Shell & Layout ✅

**Objective:** Implement the core Desktop UI using the Framework.

**Delivered:**
- **Panel**: Global menu bar tracking active window focus.
- **Dock**: dynamic application tracking.
- **Layer Shell**: Full integration for system UI.
- **Ares Theme**: `OSFAresTheme` implementation.

---

## Phase 4 — Controls, Text, and Accessibility 🚧

**Objective:** Reach Cocoa-like baseline in control breadth.

**Current State:**
- Basic `OSFButton`, `OSFLabel`, `OSFTextField` exist in `opensef-appkit`.
- **Missing**: Complex controls (Sliders, Tables, ScrollViews).
- **Missing**: Advanced Text (Pango layout is basic).

**Next Steps:**
- Implement `OSFScrollView`.
- Implement `OSFTableView` (crucial for Filer app).
- Implement `OSFMenu` (for context menus).

---

## Phase 5 — System Services, IPC, and App Model 📋

**Objective:** Deliver system-level services that make apps feel integrated.

**Plan:**
- **IPC**: Implement a real IPC mechanism (D-Bus or Socket) for `OSFEventBus` to work across process boundaries (currently monolithic/mocked for prototype).
- **Service Registry**: Allow apps to register as services (e.g., "Text Editor").
- **Clipboard**: Shared clipboard support.

---

## Phase 6 — Animation + Rendering Fidelity 📋

**Objective:** Achieve Cocoa-grade animation (Core Animation).

**Plan:**
- **Transaction Model**: `WSync` style atomic updates.
- **Implicit Animations**: `view.setAlpha(0.5)` automatically animates.
- **Render Loop**: Move to `black-hole` or frame-callback driven rendering for perfect sync.

---

## Phase 7 — Ares Final UI/UX Polish 📋

**Objective:** macOS-level polish.

**Plan:**
- Blur/Glass effects (requiring compositor shader support).
- Sound design (UI feedback sounds).
- Full accessibility audit.

---

**Summary:** We have successfully built the Foundation (Phases 1-3). The focus now shifts to "Richness" (Controls) and "Integration" (Services).
