# Tomorrow's Session - January 1, 2026

## Rules (Don't Repeat Yesterday's Mistakes)

1. **Test smallest thing first** — Build → Run before adding complexity
2. **Research before architecture** — Check how others solved it
3. **Fail fast = pivot fast** — 3 failed attempts → research alternatives
4. **Respect language boundaries** — wlroots = C only, UI = C++
5. **No compiler flag rabbit holes** — If it doesn't parse, it won't compile

---

## TODO List

### Priority 1: Test C Compositor (30 min max)

```bash
# On NixOS VM:
exit  # Exit current shell if in one
cd ~/vitusos-ares
nix develop

cd opensef/opensef-compositor
rm -rf build && mkdir build && cd build
cmake .. -G Ninja
ninja
```

- [ ] **If compiles** → Run with `WLR_BACKENDS=wayland ./opensef-compositor`
- [ ] **If fails** → Fix ONLY the specific error, don't redesign

---

### Priority 2: First Cairo Surface (1 hour)

Create `opensef-shell/src/render/OSFSurface.cpp`:

- [ ] Connect to Wayland display
- [ ] Create wl_surface + layer_surface
- [ ] Create Cairo surface from SHM buffer
- [ ] Render solid color rectangle
- [ ] Display via layer-shell

**Success = Colored rectangle visible on screen**

---

### Priority 3: Global Menu Bar (2 hours)

- [ ] Panel anchored to top edge
- [ ] Orange square (12x12) on left
- [ ] "Filer" text next to it
- [ ] Clock on right side
- [ ] Semi-transparent background

---

### Priority 4: Dock (if time permits)

- [ ] Anchored to bottom center
- [ ] Rounded rectangle background
- [ ] Placeholder icons (colored circles for now)

---

## Key Files to Focus On

| File | Purpose |
|------|---------|
| `opensef-compositor/src/*.c` | C compositor (7 files) |
| `opensef-shell/include/OSFSurface.h` | Cairo surface header |
| `opensef-shell/include/OSFAresTheme.h` | Theme colors/dimensions |
| `opensef-shell/src/render/OSFSurface.cpp` | **CREATE THIS** |
| `opensef-shell/src/panel/OSFPanel.cpp` | **CREATE THIS** |

---

## Quick Reference

### Colors
- Space Orange: `#E85D04`
- Panel background: `#252525` at 90% opacity
- Text: `#F5F5F5`

### Dimensions
- Panel height: 28px
- Dock height: 64px
- Corner radius: 8px (windows), 16px (dock)

---

## If Stuck

1. Check `sway` source for reference (pure C, works)
2. Check `labwc` source for layer-shell examples
3. Check `waybar` for Cairo + layer-shell panel

---

*Good luck tomorrow. Build first, polish later.*
