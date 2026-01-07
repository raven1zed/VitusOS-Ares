#!/bin/bash
# Test compositor with framework integration

cd "$(dirname "$0")/.."

echo "=== Testing VitusOS Compositor with Framework ==="
echo ""

# Set environment
export LD_LIBRARY_PATH=/usr/lib/wsl/lib:$PWD/opensef/build/opensef-base:$PWD/opensef/build/opensef-framework:$LD_LIBRARY_PATH
export WAYLAND_DISPLAY=wayland-0
export XDG_RUNTIME_DIR=/mnt/wslg/runtime-dir
export WLR_RENDERER=pixman
export WLR_NO_HARDWARE_CURSORS=1

echo "Starting compositor..."
./opensef/build/opensef-compositor/opensef-compositor

echo ""
echo "Compositor exited"
