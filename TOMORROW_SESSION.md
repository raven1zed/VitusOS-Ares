# Tomorrow's Session - January 2, 2026

## Context for AI Instances

**READ FIRST**: See `walkthrough.md` for full openSEF vision.

**Key understanding**: openSEF hides Linux's plumbing (systemd, kernel, GRUB). Users never see code/logs.

---

## Session Goals

### Priority 1: Wallpaper Client
- Create `osf-wallpaper` (layer-shell background)
- Load image, render via Cairo
- Ends the "black void" era

### Priority 2: Dock Interactivity
- Load SVG icons (librsvg)
- Click icon → spawn app (fork + exec)
- Hover effect (scale 1.1x)

### Priority 3: Session Integration
- Create `opensef.desktop` session entry
- Write `start-opensef.sh` launcher script
- Test selecting openSEF from SDDM/GDM

---

## Commands Reference

```bash
# Enter dev environment
nix develop

# Build everything
cd opensef/build
cmake .. -G Ninja && ninja

# Run compositor
WLR_NO_HARDWARE_CURSORS=1 ./opensef-compositor/opensef-compositor

# Run clients (separate terminals)
WAYLAND_DISPLAY=wayland-1 ./opensef-shell/osf-panel
WAYLAND_DISPLAY=wayland-1 ./opensef-shell/osf-dock
```

---

## Known Issues
- Panel/Dock crash if started before compositor is ready
- Clock format may need locale adjustment
- Dock icons are colored rectangles (need SVG)

---

*Build first, polish later.*
