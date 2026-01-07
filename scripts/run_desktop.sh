#!/bin/bash
# VitusOS Ares - Complete Desktop Launch Script
# Launches compositor + shell components

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

echo "=== VitusOS Ares - Complete Desktop ==="
echo ""

# Detect environment
if [ -n "$WSL_DISTRO_NAME" ]; then
    echo "Environment: WSL2 with WSLg"
    export VITUS_BACKEND=wayland
    export WAYLAND_DISPLAY=wayland-1
    export XDG_RUNTIME_DIR=/mnt/wslg/runtime-dir
    export WLR_RENDERER=pixman
    export WLR_NO_HARDWARE_CURSORS=1
    export WLR_RENDERER_ALLOW_SOFTWARE=1
    export WLR_LOG_LEVEL=info
else
    echo "Environment: Native Linux"
fi

# Common environment
export XDG_SESSION_TYPE=wayland
export QT_QPA_PLATFORM=wayland
export GDK_BACKEND=wayland
export MOZ_ENABLE_WAYLAND=1

cd "$PROJECT_ROOT"

# Start compositor in background
echo "Starting compositor..."
./build/opensef/opensef-compositor/opensef-compositor > compositor.log 2>&1 &
COMPOSITOR_PID=$!

# Wait for compositor to be ready
sleep 2

# Check if compositor is running
if ! kill -0 $COMPOSITOR_PID 2>/dev/null; then
    echo "ERROR: Compositor failed to start"
    cat compositor.log
    exit 1
fi

echo "Compositor started (PID: $COMPOSITOR_PID)"

# Start shell components
echo "Starting shell..."
./build/opensef/opensef-shell/osf-shell > shell.log 2>&1 &
SHELL_PID=$!

echo "Shell started (PID: $SHELL_PID)"
echo ""
echo "VitusOS Ares is running!"
echo ""
echo "To stop:"
echo "  kill $COMPOSITOR_PID $SHELL_PID"
echo ""
echo "Logs:"
echo "  Compositor: compositor.log"
echo "  Shell: shell.log"
echo ""

# Wait for compositor
wait $COMPOSITOR_PID
