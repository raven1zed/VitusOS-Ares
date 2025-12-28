# VitusOS Ares - Linux Validation Checklist

## ⚠️ Use a VM First!

**Don't risk your main system.** Test in a VM before bare metal.

### Option A: QEMU/KVM (Recommended - fastest)
```bash
# Install on Windows (via WSL2 or MSYS2)
# Or on existing Linux:
sudo apt install qemu-kvm virt-manager

# Create VM with NixOS or Ubuntu
virt-manager  # GUI to create VM
```

### Option B: VirtualBox
1. Download [VirtualBox](https://www.virtualbox.org/)
2. Download NixOS ISO or Ubuntu 24.04
3. Create VM: 4GB RAM, 40GB disk, enable 3D acceleration

### Option C: WSL2 (Limited - no Wayland)
```powershell
# On Windows
wsl --install -d Ubuntu-24.04
```
**Note:** WSL2 can build openSEF but can't run Wayland tests.

---

## Pre-requisites
- [ ] VM running Linux (NixOS recommended, Ubuntu/Fedora also works)
- [ ] Git configured
- [ ] Internet connection

---

## Step 1: Clone and Setup

```bash
# If starting fresh
git clone https://github.com/YOUR_USERNAME/VitusOS-Ares.git
cd VitusOS-Ares

# Or if using existing local copy, init git
git init
git add .
git commit -m "Initial scaffolding"

# Add SeagrEnv as submodule
git submodule add https://github.com/raven1zed/SeagrEnv.git external/SeagrEnv
git submodule update --init --recursive
```

---

## Step 2: Enter Nix Dev Shell

```bash
# Enter development environment
nix develop

# Expected output:
# ╔════════════════════════════════════════════════════════════╗
# ║       VitusOS Ares - openSEF Development Shell             ║
# ║  openSEF: Open SeagrEnv Framework                          ║
# ║  Wayland: 1.xx.x                                           ║
# ║  Vulkan:  Available                                        ║
# ╚════════════════════════════════════════════════════════════╝

# Verify toolchain
clang --version    # Should be 16+
pkg-config --modversion wayland-client
vulkaninfo | head
```

---

## Step 3: Build openSEF

```bash
# Configure
cmake -B build -S . -G Ninja

# Build
cmake --build build

# Expected: Build succeeds (may have warnings, but no errors)
```

---

## Step 4: Run Test (Hello Window)

```bash
# If on Wayland desktop:
./build/opensef/test/hello-window

# Expected: A window appears with VitusOS styling
```

---

## Troubleshooting

### "GNUStep not found"
```bash
# Check gnustep packages
pkg-config --cflags gnustep-base
# If missing, add to flake.nix
```

### "Wayland display not found"
```bash
# Must run on Wayland session, not X11
echo $XDG_SESSION_TYPE  # Should be "wayland"
```

### "Vulkan not available"
```bash
# Check NVIDIA driver
nvidia-smi
# Ensure vulkan-loader is installed
```

---

## Success Criteria

- [ ] `nix develop` enters shell without errors
- [ ] `cmake -B build` configures successfully
- [ ] `cmake --build build` compiles all components
- [ ] openSEF libraries created in `build/opensef/`
