# VitusOS Ares - NixOS VM Setup

## ⚠️ Use a VM First!

Test in VM before bare metal.

### VirtualBox + NixOS Plasma
1. Download [VirtualBox](https://www.virtualbox.org/)
2. Download [NixOS Plasma ISO](https://nixos.org/download)
3. Create VM: **4GB RAM**, **40GB disk**, **3D acceleration ON**
4. Install NixOS

---

## Step 1: Clone

```bash
git clone https://github.com/raven1zed/vitusos-ares.git
cd vitusos-ares
```

---

## Step 2: Dev Shell

```bash
nix develop
```

---

## Step 3: Build

```bash
cmake -B build -G Ninja
cmake --build build
```

---

## Step 4: Test

```bash
./build/opensef/test/hello-window
```

---

## Troubleshooting

| Error | Fix |
|-------|-----|
| `nix develop` fails | `nix flake update` |
| Clang not found | Not in nix shell |
| Wayland not found | Run in desktop, not TTY |
