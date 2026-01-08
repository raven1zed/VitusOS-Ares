#!/bin/bash
# Direct build script that bypasses Nix and uses system packages
# For WSL/NixOS when nix develop is broken

set -e

echo "=== VitusOS Ares - Direct Build (No Nix Shell) ==="
echo ""

# Check if we're in WSL
if ! grep -qi microsoft /proc/version; then
    echo "⚠️  Warning: Not running in WSL"
fi

# Set up environment for WSL
export WAYLAND_DISPLAY=wayland-0
export XDG_RUNTIME_DIR=/mnt/wslg/runtime-dir
export WLR_RENDERER=pixman
export WLR_NO_HARDWARE_CURSORS=1

# Add WSL lib path
export LD_LIBRARY_PATH=/usr/lib/wsl/lib:${LD_LIBRARY_PATH}

# Try to find system cmake and ninja
if ! command -v cmake &> /dev/null; then
    echo "❌ cmake not found in PATH"
    echo "   Please install: nix-env -iA nixos.cmake"
    exit 1
fi

if ! command -v ninja &> /dev/null; then
    echo "❌ ninja not found in PATH"
    echo "   Please install: nix-env -iA nixos.ninja"
    exit 1
fi

echo "✅ Found cmake: $(which cmake)"
echo "✅ Found ninja: $(which ninja)"
echo ""

# Configure
echo "📦 Configuring with CMake..."
cmake -S opensef -B opensef/build -G Ninja \
    -DCMAKE_C_COMPILER=gcc \
    -DCMAKE_CXX_COMPILER=g++ \
    -DBUILD_TESTING=ON \
    -DBUILD_APPS=ON

echo ""

# Build
echo "🔨 Building openSEF..."
cmake --build opensef/build --parallel $(nproc)

echo ""
echo "✅ Build complete!"
echo ""
echo "Binaries:"
echo "  - opensef/build/opensef-compositor/opensef-compositor"
echo "  - opensef/build/opensef-shell/osf-shell"
