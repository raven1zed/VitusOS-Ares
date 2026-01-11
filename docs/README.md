# openSEF Framework Documentation

**Single source of truth for VitusOS Ares openSEF framework**

---

## Core Documents

### 1. **[Architecture Overview](VitusOS%20Ares.md)** 📐
Complete technical architecture - how openSEF works as a unified framework.

**Read this to understand**:
- Unified framework vs Linux DE paradigm
- Tech stack (Qt Quick + Vulkan + GNUstep)
- Boot-to-shutdown integration
- Current vs future architecture

### 2. **[Design Reference](openSEF%20Design%20Reference.md)** 🎨
UI/UX guidelines and design language.

**Read this for**:
- Ares aesthetic (macOS Aqua + OS1)
- Color palette
- Component sizes (panel 28px, dock 64px)
- Typography (Inter font)

### 3. **[API Documentation](API.md)** 💻
Framework API reference for developers.

**Read this for**:
- `OSFDesktop::shared()` usage
- EventBus pub/sub patterns
- WindowManager integration
- Theme system

### 4. **[GNUstep Integration Plan](gnustep_architecture_plan.md.resolved)** 🔧
Complete plan for forking GNUstep AppKit.

**Read this for**:
- Why GNUstep (widget architecture)
- AppKitBridge design (Objective-C++)
- Migration from Qt-only to Qt+AppKit
- Implementation phases

### 5. **[Known Limitations](KNOWN_LIMITATIONS.md)** ⚠️
Current issues and workarounds.

**Read this for**:
- WSLg-specific issues (dropdowns, lockscreen)
- Platform compatibility matrix
- Temporary limitations

### 6. **[Cocoa Alignment Roadmap](COCOA_ALIGNMENT_ROADMAP.md)** 🗺️
Phase tracking for Cocoa-like features.

**Read this for**:
- What phase we're in (Phase 4)
- Completed features (Phases 1-3)
- Upcoming work (Phases 5-7)

---

## Quick Reference

### Current Phase
**Phase 4: Controls & Integration (uptc release)**

We are building the **uptc (Upstream Color)** development release with:
- Qt Quick shell (functional)
- Framework integration (OSFDesktop, EventBus)
- Linux protocol compatibility (DBusMenu, StatusNotifier)

### Next Phase
**Phase 5: GNUstep AppKit Integration (up1 release)**

Future work for **up1 (Upstream One)** stable release:
- Fork GNUstep libs-gui
- AppKitBridge (Objective-C++)
- Replace Linux protocols with native openSEF APIs

---

## Documentation Standards

### What Belongs Here
✅ Architecture decisions  
✅ API references  
✅ Design guidelines  
✅ Implementation plans  
✅ Known limitations  

### What Doesn't Belong Here
❌ Build logs (→ `logs/`)  
❌ Temporary notes  
❌ Outdated migration plans  
❌ Implementation artifacts  

---

## Changelog

**2026-01-11**: Major cleanup
- Removed outdated docs (BOOT_TO_SHUTDOWN, V1_READINESS, old DEVELOPER_GUIDE)
- Created this index for clarity
- Updated README.md with current phase
- Consolidated architecture into `VitusOS Ares.md`

**2026-01-07**: Cocoa alignment
- Added `COCOA_ALIGNMENT_ROADMAP.md`
- Updated `gnustep_architecture_plan.md.resolved`

---

## Reading Order for New Contributors

1. **Start**: `../README.md` (Project overview)
2. **Architecture**: `VitusOS Ares.md` (How it all works)
3. **Design**: `openSEF Design Reference.md` (UI/UX)
4. **API**: `API.md` (Code integration)
5. **Deep Dive**: `gnustep_architecture_plan.md.resolved` (Future direction)

---

**Keep this folder clean**. If a document is outdated or duplicates another, remove it.
