# VitusOS Boot Theme

## Boot Animation

```
    ┌─────────────────────────────────────┐
    │                                     │
    │                                     │
    │            VitusOS                  │  ← Text logo (Inter Bold 48)
    │                                     │
    │            ● ● ●                    │  ← Pulsing dots
    │                                     │
    │                                     │
    └─────────────────────────────────────┘
```

- **Background:** Warm off-white gradient
- **Logo:** "VitusOS" text, dark gray, fades in
- **Loading:** 3 coral-colored dots, pulse sequentially
- **Messages:** Hidden (no systemd output)

## No Images Required

This theme uses text rendering only - no image assets needed.

## Installation

```bash
# Copy theme files
sudo cp -r boot/plymouth/* /usr/share/plymouth/themes/vitusos/

# Set as default
sudo plymouth-set-default-theme vitusos

# Rebuild initramfs
sudo update-initramfs -u  # Debian/Ubuntu
# or
sudo dracut -f            # Fedora/NixOS
```

## Testing

```bash
sudo plymouthd
sudo plymouth --show-splash
sleep 3
sudo plymouth quit
```

## Font Note

For Inter font, install: `fonts-inter` (Debian) or add to Nix config.
Falls back to system font if Inter not available.
