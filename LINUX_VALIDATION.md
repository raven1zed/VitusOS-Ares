# VitusOS Ares - NixOS VM Setup

## ⚠️ Use a VM First!

**Test in VM before bare metal.**

### Recommended: VirtualBox + NixOS
1. Download [VirtualBox](https://www.virtualbox.org/)
2. Download [NixOS Plasma ISO](https://nixos.org/download) (has GUI)
3. Create VM: **4GB RAM**, **40GB disk**, enable **3D acceleration**
4. Boot and install NixOS

---

## Step 1: Clone from GitHub

```bash
```

---

## Step 2: Enter Nix Dev Shell

```bash
nix develop

# You should see:
# ╔════════════════════════════════════════════════════════════╗
# ║       VitusOS Ares - openSEF Development Shell             ║
# ║  Theme:   Ares (The Martian)                               ║
# ╚════════════════════════════════════════════════════════════╝
```

---

## Step 3: Build

```bash
# Configure with Ninja (faster)
cmake -B build -G Ninja

# Build
cmake --build build

# Expected: Libraries built in build/opensef/
```

---

## Step 4: Run Test

```bash
# Must be in Wayland session!
./build/opensef/test/hello-window
```

---

## Troubleshooting

| Error | Fix |
|-------|-----|
| `nix develop` fails | Run `nix flake update` first |
| `Clang not found` | You're not in nix shell |
| `Wayland not found` | Run in desktop (not TTY) |
| `GNUStep not found` | Check nix shell includes gnustep.* |

---

## Quick Commands

```bash
# Enter shell
nix develop

# Full rebuild
rm -rf build && cmake -B build -G Ninja && cmake --build build

# Run test
./build/opensef/test/hello-window
```
