# VitusOS-Ares Build Scripts

Simplified scripts for easy development workflow.

## Quick Start

```bash
# In WSL2
cd /mnt/c/Users/hp/Documents/VitusOS-Ares

# 1. Setup environment (one-time)
./scripts/setup.sh

# 2. Build all components
./scripts/build.sh

# 3. Run openSEF Desktop
./scripts/run.sh

# 4. Verify everything works
./scripts/test.sh
```

## Scripts

### `setup.sh`
**Purpose**: Initialize development environment

**What it does:**
- Checks for Nix installation
- Verifies WSL2 environment
- Enters nix development shell
- Shows installed tools and versions
- Displays next steps

**Usage:**
```bash
./scripts/setup.sh
```

**Run this:** Once, before first build

---

### `build.sh`
**Purpose**: Build all VitusOS components

**What it does:**
- Builds components in correct dependency order:
  1. openSEF Framework (core C++ APIs)
  2. openSEF Compositor (wlroots + Vulkan)
  3. GNUstep C++ Fork (AppKit widgets)
  4. openSEF Shell (Qt Quick UI)
  5. Native Apps (Filer, etc.)
- Uses Ninja for fast compilation
- Shows progress for each component
- Stops on first error

**Usage:**
```bash
# Normal build
./scripts/build.sh

# Clean build (removes previous builds)
./scripts/build.sh clean
```

**Run this:** After code changes

---

### `run.sh`
**Purpose**: Start openSEF Desktop for testing

**What it does:**
- Launches compositor in nested Wayland mode
- Starts shell with Qt debugging enabled
- Shows component PIDs
- Displays what you should see
- Handles cleanup on Ctrl+C

**Usage:**
```bash
./scripts/run.sh
```

**Expected output:**
- Panel at top with Filer menu
- Dock at bottom with app icons
- SystemTray on right (clickable)
- Wallpaper background
- Filer has orange dot (always running)

**Stop with:** Ctrl+C

---

### `test.sh`
**Purpose**: Verify openSEF functionality

**What it does:**
- Checks all component builds exist
- Verifies source code structure
- Tests design system compliance (Inter font, Ares colors)
- Validates architecture (OSFDesktop usage, EventBus)
- Counts passed/failed tests
- Shows detailed summary

**Usage:**
```bash
./scripts/test.sh
```

**Run this:** Before committing changes, after builds

---

## Workflow

### First Time Setup
```bash
cd /mnt/c/Users/hp/Documents/VitusOS-Ares
./scripts/setup.sh
./scripts/build.sh
./scripts/test.sh
./scripts/run.sh
```

### Daily Development
```bash
# Make code changes...

./scripts/build.sh       # Rebuild changed components
./scripts/test.sh        # Verify nothing broke
./scripts/run.sh         # Test visually
```

### Clean Rebuild
```bash
./scripts/build.sh clean   # Remove all previous builds
./scripts/build.sh          # Full rebuild
```

### Debugging
```bash
# Build with debug symbols (default)
./scripts/build.sh

# Run with verbose output (automatically enabled)
./scripts/run.sh

# Check build status
./scripts/test.sh
```

---

##Troubleshooting

### "Nix is not installed"
```bash
# Install Nix
sh <(curl -L https://nixos.org/nix/install) --daemon
```

### "Must run from VitusOS-Ares root directory"
```bash
# Go to correct directory
cd /mnt/c/Users/hp/Documents/VitusOS-Ares

# Then run scripts
./scripts/build.sh
```

### "Compositor not built"
```bash
# BUILD first
./scripts/build.sh

# Then run
./scripts/run.sh
```

### "Build failed"
```bash
# Clean and rebuild
./scripts/build.sh clean
./scripts/build.sh
```

### Compositor crashes
```bash
# Check wlroots version
pkg-config --modversion wlroots

# Should be: 0.19+
```

---

## Build Output Locations

```
opensef/
├── opensef-framework/build/
│   └── libopensef.so
├── opensef-compositor/build/
│   └── opensef-compositor
├── opensef-gnustep/build/
│   └── libgnustep-appkit.so
├── opensef-shell-qt/build/
│   └── opensef-shell-qt
└── apps/osf-filer-native/build/
    └── osf-filer-native
```

---

## Environment Variables

Scripts automatically set these for WSL2 nested mode:

**Compositor:**
- `WLR_BACKENDS=wayland` - Nested Wayland mode
- `WLR_RENDERER=vulkan` - GPU acceleration
- `WLR_DEBUG=1` - Verbose logging

**Shell:**
- `QT_QPA_PLATFORM=wayland` - Use Wayland
- `QT_LOGGING_RULES="*.debug=true"` - Qt debug output
- `QT_WAYLAND_DISABLE_WINDOWDECORATION=1` - No client decorations

---

## Notes

- All scripts must be run from VitusOS-Ares root directory
- Scripts require Nix development environment
- Build order matters (framework → compositor → gnustep → shell → apps)
- run.sh uses nested mode for testing (not production use)
- Ctrl+C cleanly stops all components

---

**For up1 production:** These scripts will be updated to support:
- ISO generation
- Boot animation integration
- Installer
- Systemd service units
- Production optimizations
