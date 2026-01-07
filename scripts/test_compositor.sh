#!/bin/bash
# Test compositor with framework integration

cd "$(dirname "$0")/.."

echo "=== Testing VitusOS Compositor with Framework ==="
echo ""

# Set WSLg environment (wayland-0 is the correct socket)
export WAYLAND_DISPLAY=wayland-0
export XDG_RUNTIME_DIR=/mnt/wslg/runtime-dir
export WLR_RENDERER=pixman
export WLR_NO_HARDWARE_CURSORS=1

echo "Starting compositor..."
./build/opensef/opensef-compositor/opensef-compositor

echo ""
echo "Compositor exited"
