#!/bin/bash
# run_vitus_ares.sh - Launch the full desktop environment
# VitusOS Ares - openSEF Desktop

set -e

# Kill any existing components
pkill -9 -f opensef-compositor || true
pkill -9 -f osf-shell || true
pkill -9 -f phase3-app || true
sleep 0.5

# Set library paths
export LD_LIBRARY_PATH=/usr/lib/wsl/lib:$PWD/build/opensef/opensef-base:$LD_LIBRARY_PATH
if [ -d "/mnt/wslg/runtime-dir" ]; then
    export XDG_RUNTIME_DIR="/mnt/wslg/runtime-dir"
fi
export XDG_RUNTIME_DIR=${XDG_RUNTIME_DIR:-/run/user/$(id -u)}

# Detect if running in WSL
if [ -n "$WSL_DISTRO_NAME" ]; then
    echo "WSL detected - launching Wayland-Pixman path"
    export VITUS_BACKEND=wayland
    export WAYLAND_DISPLAY=wayland-0
    export WLR_WL_OUTPUTS=1
    export WLR_WL_FULLSCREEN=0
    
    # Stability and Software stack
    export WLR_RENDERER=pixman
    export WLR_NO_HARDWARE_CURSORS=1
    export WLR_RENDERER_ALLOW_SOFTWARE=1
    export WLR_LOG_LEVEL=info
else
    echo "Native Linux - using auto-detect backend"
fi

# Socket name for our compositor
SOCKET_NAME="wayland-1"

# Clean up any stale socket
rm -f "$XDG_RUNTIME_DIR/$SOCKET_NAME" "$XDG_RUNTIME_DIR/$SOCKET_NAME.lock"

echo "=== VitusOS Ares Desktop Launcher ==="
echo "XDG_RUNTIME_DIR: $XDG_RUNTIME_DIR"
echo "Socket: $SOCKET_NAME"

# 1. Launch Compositor with explicit socket name
echo "[1/3] Launching Compositor..."
./build/opensef/opensef-compositor/opensef-compositor -s "$SOCKET_NAME" > compositor.log 2>&1 &
COMPOSITOR_PID=$!
echo "      Compositor PID: $COMPOSITOR_PID"

# Wait for socket to appear (up to 10 seconds)
echo "      Waiting for socket..."
for i in {1..20}; do
    if [ -S "$XDG_RUNTIME_DIR/$SOCKET_NAME" ]; then
        echo "      Socket ready!"
        break
    fi
    if ! kill -0 $COMPOSITOR_PID 2>/dev/null; then
        echo "ERROR: Compositor crashed. Check compositor.log"
        cat compositor.log
        exit 1
    fi
    sleep 0.5
done

if [ ! -S "$XDG_RUNTIME_DIR/$SOCKET_NAME" ]; then
    echo "ERROR: Socket not found after 10s. Aborting."
    kill $COMPOSITOR_PID 2>/dev/null || true
    cat compositor.log
    exit 1
fi

# Set environment for clients
export WAYLAND_DISPLAY="$SOCKET_NAME"

# 2. Launch Shell (Wallpaper, Panel, Dock)
echo "[2/3] Launching Shell..."
./build/opensef/opensef-shell/osf-shell > shell.log 2>&1 &
SHELL_PID=$!
echo "      Shell PID: $SHELL_PID"
sleep 1

# 3. Launch Phase 3 Test Application
echo "[3/3] Launching Phase 3 App..."
./build/opensef/test/phase3-app > phase3.log 2>&1 &
APP_PID=$!
echo "      App PID: $APP_PID"

echo ""
echo "=== VitusOS Ares Desktop Running ==="
echo "Compositor: $COMPOSITOR_PID"
echo "Shell:      $SHELL_PID"  
echo "App:        $APP_PID"
echo ""
echo "Logs: compositor.log, shell.log, phase3.log"
ps aux | grep -E "(opensef|phase3)" | grep -v grep
