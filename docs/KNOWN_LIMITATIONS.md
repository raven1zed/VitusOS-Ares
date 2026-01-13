# VitusOS Ares - Known Limitations & Roadmap

## Current Limitations

### 1. Dropdown Menus (WSLg Only)

**Status**: Known Issue  
**Severity**: Medium (cosmetic glitch)  
**Platforms Affected**: WSLg only (native Linux works correctly)

**Description**:
Dropdown menus from the global menu bar are rendered inside the same Qt Quick layer-shell surface as the panel. In WSLg's RDP-Rail integration, any QML content that extends beyond the surface bounds is clipped, so menu drop-downs can appear truncated.

**Root Cause**:
```qml
// opensef/opensef-shell-qt/qml/AresPanel.qml
// DropdownMenu items are children of the panel surface and inherit its bounds.
Item {
  id: panelRoot
  // Panel is a single layer-shell surface with fixed height.
  // Dropdowns rendered here are clipped in WSLg when they overflow.
}
```

**Correct Solution** (deferred):
Create separate popup surfaces for menus using Qt Quick `Window`/`Popup` with an xdg-popup backend (or a dedicated layer-shell popup surface). Position them via `xdg_positioner` so they are independent of the panel surface bounds.

**Workaround**:
None currently. Dropdowns are functional but may appear glitchy in WSLg. Use native Linux for development if this is problematic.

**Timeline**: Will be addressed after core DE features are complete.

---

### 2. Lockscreen Visibility (WSLg Only)

**Status**: Known Issue  
**Severity**: Low (development environment only)  
**Platforms Affected**: WSLg only

**Description**:
The lockscreen uses a full-screen Qt Quick surface on the OVERLAY layer, but WSLg's RDP-Rail integration does not reliably display overlay-layer surfaces.

**Root Cause**:
WSLg's `weston_rdprail_shell` does not consistently map layer-shell overlay surfaces into the Windows desktop. Qt Quick renders correctly, but the surface is not forwarded to RDP in this layer.

**Workaround**:
Test lockscreen functionality on native Linux. For WSLg development, use compositor logs to verify the surface is created and mapped. Consider a temporary fallback to a fullscreen xdg-shell window for WSLg.

**Timeline**: Low priority. Will investigate after XDG popup implementation.

---

## Architecture Notes

### Current Shell Architecture

```
opensef-shell-qt
  ├─ qml/AresPanel.qml
  ├─ qml/DropdownMenu.qml
  └─ src/PanelController.cpp
        ↓
   Single layer-shell surface (Qt Quick)
```

**Limitation**: Layer-shell surfaces are designed for fixed UI elements (panels, docks), not dynamic popups.

### Future Architecture (Planned)

```
opensef-shell-qt
  ├─ Panel layer-shell surface (Qt Quick)
  └─ Popup surfaces (xdg-popup or layer-shell popup)
        └─ Positioned via xdg_positioner
        └─ Rendered by dedicated Qt Quick windows
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
| Panel | ✅ Works | ✅ Works | Qt Quick layer-shell surface |
| Dock | ✅ Works | ✅ Works | Qt Quick layer-shell surface |
| Wallpaper | ✅ Works | ✅ Works | Qt Quick scene renders correctly |
| Windows | ✅ Works | ✅ Works | XDG shell |
| Dropdowns | ✅ Works | ⚠️ Glitches | Qt Quick menus clipped inside panel surface |
| Lockscreen | ✅ Works | ❌ Invisible | Overlay layer surface not mapped in WSLg |
| Input Events | ✅ Works | ✅ Works | Mouse & keyboard |

**Legend**:
- ✅ Works: Fully functional
- ⚠️ Glitches: Functional but visual issues
- ❌ Invisible: Not displayed but runs

---

Last Updated: 2026-01-11
