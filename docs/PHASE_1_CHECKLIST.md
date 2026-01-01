# Phase 1 Checklist — Framework Foundation

**Source:** VitusOS Ares.md v2.0 (January 2, 2026)

---

## Objective

Establish the minimal Foundation and AppKit surface that makes openSEF a real application framework.

---

## Deliverables

### 1. Application Lifecycle
- [ ] `OSFApplication` class with `run()`, `stop()`, launch/terminate callbacks
- [ ] Singleton access pattern (`OSFApplication::shared()`)
- [ ] Command-line argument handling

### 2. Run Loop
- [ ] `OSFRunLoop` task scheduling
- [ ] Timer integration
- [ ] Event source management

### 3. Notification Center
- [ ] `OSFNotificationCenter` pub/sub system
- [ ] Post notifications by name
- [ ] Add/remove observers with callbacks

### 4. Bundle System
- [ ] `OSFBundle` metadata loader
- [ ] Info.plist or equivalent manifest parsing
- [ ] Resource path resolution

### 5. Window + View Hierarchy
- [ ] `OSFWindow` class (minimal, not yet connected to Wayland surface)
- [ ] `OSFView` base class with frame, subviews, rendering
- [ ] View tree traversal and rendering order

### 6. Sample App
- [ ] Minimal app that proves lifecycle + view rendering
- [ ] Uses OSFApplication to launch
- [ ] Creates OSFWindow + OSFView hierarchy
- [ ] Renders something visible (even offscreen buffer is acceptable for Phase 1)

---

## Definition of Done

> A minimal openSEF app can launch via OSFApplication, create a window, and render a view hierarchy.

---

## Current Status Assessment (January 2, 2026)

| Component | Status | Notes |
|-----------|--------|-------|
| OSFApplication | ✅ Complete | Has run/stop/callbacks |
| OSFRunLoop | ✅ Complete | Thread-safe task queue |
| OSFNotificationCenter | ✅ Complete | Full pub/sub with tokens |
| OSFBundle | ✅ Complete | Identifier + resource path |
| OSFObject | ✅ Complete | Base class with unique IDs |
| OSFWindow | 🔄 Phase 2 | Needs Wayland integration |
| OSFView | ✅ Exists | In opensef-appkit |
| Sample App | ✅ Created | `phase1_validation.cpp` |

**Assessment:** Phase 1 is **COMPLETE**. All foundation classes exist and are functional. Phase 2 work (Wayland window integration) is next.

---

## Notes for AI Instances

- **Phase 1 is framework, not shell.** Don't confuse with panel/dock (Phase 7).
- **OSFApplication is the linchpin.** Everything else depends on it.
- **Keep it minimal.** Phase 1 success = one app runs. Polish comes later.
