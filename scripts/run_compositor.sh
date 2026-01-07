#!/bin/bash
# VitusOS Ares - Compositor Launch Script
# Runs the compositor with proper environment for WSLg or native Linux

set -e

echo "=== VitusOS Ares Desktop Environment ==="
echo ""

# Detect environment
if [ -n "$WSL_DISTRO_NAME" ]; then
    echo "Running on WSL2 with WSLg"
    echo "Backend: Wayland (nested in WSLg)"
    
    # WSLg environment
    export VITUS_BACKEND=wayland
    export WAYLAND_DISPLAY=wayland-1
    export XDG_RUNTIME_DIR=/mnt/wslg/runtime-dir
    
    # Stability and Software stack
    export WLR_RENDERER=pixman
    export WLR_NO_HARDWARE_CURSORS=1
    export WLR_RENDERER_ALLOW_SOFTWARE=1
    export WLR_LOG_LEVEL=info
else
    echo "Native Linux - using auto-detect backend"
fi

# Common environment
export XDG_SESSION_TYPE=wayland
export QT_QPA_PLATFORM=wayland
export GDK_BACKEND=wayland
export MOZ_ENABLE_WAYLAND=1

# Launch compositor
echo ""
echo "Starting openSEF compositor..."
echo ""

cd "$(dirname "$0")/.."

# Run compositor and log output
./build/opensef/opensef-compositor/opensef-compositor 2>&1 | tee compositor.log
