# VM Testing Guide

How to verify the current build of VitusOS Ares (Phase 1 + Partial Shell).

## 1. Sync & Build

Run these commands in your VM terminal:

```bash
# 1. Update code
cd virtucos-ares
git pull

# 2. Enter dev environment
nix develop

# 3. Clean Rebuild (to ensure integrity)
rm -rf opensef/build
cd opensef
mkdir -p build && cd build
cmake .. -G Ninja
ninja
```

## 2. Verify Phase 1 (Foundation)

This tests the application lifecycle, run loop, notifications, and bundle system. It is a console app.

```bash
# From opensef/build
./test/phase1_validation
```

**Expected Output:**
```
[1] Testing OSFBundle...
    ✓ Bundle identifier: opensef.app
    ✓ Resource path: .
[2] Testing OSFNotificationCenter...
    ✓ Notification callback executed!
    ✓ NotificationCenter works correctly
...
PHASE 1 VALIDATION: PASSED
```

## 3. Verify Shell (Visual)

This launches the compositor, panel, and dock.

```bash
# From project root
./scripts/start-opensef.sh
```

**Expected Result:**
1. Screen should flicker/switch to compositor
2. Orange-themed Plymouth boot animation (if integrated) OR black screen initially
3. **Top Bar:** Should appear with "Menu", clock
4. **Dock:** Should appear at bottom with icons
5. **Mouse:** Should move cursor
6. **Interaction:** Click buttons (verify if click works)

## Troubleshooting

- **If build fails on Vulkan:** Uninstall `opensef-appkit` from `opensef/CMakeLists.txt` (comment out line 17 `add_subdirectory(opensef-appkit)`).
- **If shell crashes:** Check logs: `dmesg | tail` or run directly: `./opensef-compositor -d`
