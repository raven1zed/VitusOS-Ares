# Tomorrow's Session - January 2, 2026

## Goal: Interactivity & Window Management

We have the *look* (Mars Light, Panel, Dock). Now we need the *feel* (Windows that actually work).

## TODO List

### Priority 1: Window Management (Tiling)
- [ ] **Snap to Grid**: Windows should respect the Panel and Dock areas (work area implementation).
- [ ] **Basic Tiling**: When a second window opens, split screen (50/50).
- [ ] **Focus Follows Mouse**: Hovering a window should activate it.

### Priority 2: Dock Interactivity
- [ ] **Click to Launch**: Clicking the "Terminal" icon should spawn `alacritty` or `foot`.
- [ ] **Hover Effect**: Icons should scale up slightly (1.1x) when hovered (Animation Framework test).
- [ ] **Active Indicator**: Little dot below running apps.

### Priority 3: Wallpaper
- [ ] Remove the black void.
- [ ] Implement `osf-wallpaper` client that renders the Mars landscape.

---

## Known Issues
- [ ] Dock icons are just colored rectangles (Need SVG loading).
- [ ] Text rendering on the panel is static.
- [ ] No window decorations (Title bars) yet.

*Build first, polish later.*
