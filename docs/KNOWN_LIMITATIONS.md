# VitusOS Ares - Known Limitations & Roadmap

## Current Limitations

### 1. Dropdown Menus (WSLg Only)

**Status**: Known Issue  
**Severity**: Medium (cosmetic glitch)  
**Platforms Affected**: WSLg only (native Linux works correctly)

**Description**:
Dropdown menus from the global menu bar are drawn directly on the panel's layer surface instead of using separate XDG popup windows. This causes rendering glitches in WSLg's RDP-Rail integration where content extending beyond the panel bounds may be clipped or not rendered properly.

**Root Cause**:
```cpp
// Current implementation in OSFPanel.cpp
void OSFPanel::drawDropdown(cairo_t *cr, int menuIndex) {
  // Draws dropdown on same Cairo surface as panel
  // WSLg clips content outside expected bounds
}
```

**Correct Solution** (deferred):
Create separate XDG popup surfaces for dropdown menus using the `xdg_wm_base` protocol with proper positioning via `xdg_positioner`.

**Workaround**:
None currently. Dropdowns are functional but may appear glitchy in WSLg. Use native Linux for development if this is problematic.

**Timeline**: Will be addressed after core DE features are complete.

---

### 2. Lockscreen Visibility (WSLg Only)

**Status**: Known Issue  
**Severity**: Low (development environment only)  
**Platforms Affected**: WSLg only

**Description**:
The lockscreen creates a layer surface on the OVERLAY layer, but WSLg's RDP-Rail integration doesn't display it on the Windows desktop.

**Root Cause**:
WSLg's `weston_rdprail_shell` appears to have specific requirements or limitations for OVERLAY layer surfaces that we don't currently meet.

**Workaround**:
Test lockscreen functionality on native Linux. For WSLg development, use compositor logs to verify lockscreen is being created and mapped correctly.

**Timeline**: Low priority. Will investigate after XDG popup implementation.

---

## Architecture Notes

### Current Shell Architecture

```
OSFPanel (C++)
  └─ OSFSurface (layer shell wrapper)
     └─ Cairo rendering
        └─ Everything drawn on one surface
```

**Limitation**: Layer shell surfaces are designed for fixed UI elements (panels, docks), not dynamic popups.

### Future Architecture (Planned)

```
OSFPanel (C++)
  ├─ OSFSurface (layer shell) - panel bar
  └─ OSFPopupSurface (XDG popup) - dropdown menus
     └─ Positioned relative to parent
     └─ Separate rendering surface
```

**Benefit**: Architecturally correct, works on all platforms including WSLg.

---

## Development Recommendations

### For WSLg Development
1. **Accept dropdown glitches** as a known limitation
2. **Focus on functionality** rather than visual perfection
3. **Test critical features** on native Linux periodically
4. **Use compositor logs** to verify behavior

### For Production Deployment
1. **Target native Linux** as the primary platform
2. **WSLg is for development convenience only**
3. **All features work correctly on native Linux**

---

## Roadmap

### Phase 1: Core DE Features (Current Priority)
- [ ] Window management (minimize, maximize, close buttons)
- [ ] Application launcher
- [ ] Settings panel implementation
- [ ] File manager integration
- [ ] Notification system
- [ ] Keyboard shortcuts

### Phase 2: Polish & Refinement
- [ ] XDG popup implementation for dropdowns
- [ ] Lockscreen WSLg compatibility investigation
- [ ] Animations and transitions
- [ ] Theme customization
- [ ] Accessibility features

### Phase 3: Advanced Features
- [ ] Multi-monitor support
- [ ] Workspace management
- [ ] System tray applications
- [ ] Power management integration

---

## Contributing

If you encounter issues:
1. Check if it's a known limitation (listed above)
2. Test on native Linux to verify it's not a WSLg-specific issue
3. Check compositor logs for errors
4. Report new issues with platform information

---

## Platform Support Matrix

| Feature | Native Linux | WSLg | Notes |
|---------|--------------|------|-------|
| Compositor | ✅ Works | ✅ Works | Stable |
| Panel | ✅ Works | ✅ Works | Fully functional |
| Dock | ✅ Works | ✅ Works | Fully functional |
| Wallpaper | ✅ Works | ✅ Works | Renders correctly |
| Windows | ✅ Works | ✅ Works | XDG shell |
| Dropdowns | ✅ Works | ⚠️ Glitches | Layer shell limitation |
| Lockscreen | ✅ Works | ❌ Invisible | OVERLAY layer issue |
| Input Events | ✅ Works | ✅ Works | Mouse & keyboard |

**Legend**:
- ✅ Works: Fully functional
- ⚠️ Glitches: Functional but visual issues
- ❌ Invisible: Not displayed but runs

---

Last Updated: 2026-01-07
