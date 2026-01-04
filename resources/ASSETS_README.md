# Visual Assets Setup

## Wallpaper
✅ **Saved to:** `resources/wallpapers/mars_default.jpg`
- Automatically loaded by `OSFSceneWallpaper::loadDefault()`

## Icons (WhiteSur Theme)

Download the WhiteSur icon theme for macOS-like dock icons:

### Option 1: Git Clone (Recommended)
```bash
cd resources
git clone https://github.com/vinceliuice/WhiteSur-icon-theme.git icons/WhiteSur

# The icons are in: icons/WhiteSur/src/apps/
```

### Option 2: Manual Download
1. Go to: https://github.com/vinceliuice/WhiteSur-icon-theme
2. Download ZIP
3. Extract to `resources/icons/WhiteSur`

### Icon Mapping for Dock
```
files      → apps/scalable/system-file-manager.svg
terminal   → apps/scalable/utilities-terminal.svg  
browser    → apps/scalable/firefox.svg
settings   → apps/scalable/preferences-system.svg
```

## Font (Inter)
Already in `resources/fonts/` via NixOS flake.

## Usage in Code
```cpp
// Wallpaper (OSFSceneWallpaper.cpp)
loadDefault();  // Loads mars_default.jpg

// Icons (OSFSceneDock.cpp) - TODO
// loadIcon("resources/icons/WhiteSur/src/apps/scalable/...");
```
