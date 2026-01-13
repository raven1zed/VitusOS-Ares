#!/bin/bash
# VitusOS Ares - Stable WSLg Launch Script
# THIS IS THE ONLY SCRIPT YOU SHOULD USE.

PROJECT_ROOT="/mnt/c/Users/hp/Documents/VitusOS-Ares"
LOG_DIR="$PROJECT_ROOT/logs"
mkdir -p "$LOG_DIR"

echo "--- STARTING VITUSOS ARES (WSLg) ---"

# Kill existing
pkill -9 -f 'osf-'
pkill -9 -f 'weston'
sleep 2

# CRITICAL: These must be set for the NESTED compositor to show a window in WSL
export WAYLAND_DISPLAY=wayland-0
export VITUS_BACKEND=wayland
export XDG_RUNTIME_DIR=/mnt/wslg/runtime-dir
export WLR_RENDERER=pixman
export WLR_NO_HARDWARE_CURSORS=1

# 1. Start Compositor
echo "Starting Compositor (Check your taskbar for a new window)..."
"$PROJECT_ROOT/opensef/build/opensef-compositor/opensef-compositor" > "$LOG_DIR/compositor.log" 2>&1 &
COMP_PID=$!

# Wait and detect the NEW Wayland socket created by our compositor
sleep 5
WAYLAND_SOCK=$(grep -o "Running on Wayland display: wayland-[0-9]" "$LOG_DIR/compositor.log" | tail -n 1 | cut -d' ' -f5)

if [ -z "$WAYLAND_SOCK" ] || [ "$WAYLAND_SOCK" == "wayland-0" ]; then
    echo "ERROR: Compositor failed to create a nested socket."
    tail -n 20 "$LOG_DIR/compositor.log"
    exit 1
fi

export WAYLAND_DISPLAY="$WAYLAND_SOCK"
echo "Nested socket: $WAYLAND_DISPLAY (Ready for Shell & Apps)"

# 2. Start Shell
echo "Loading Shell..."
"$PROJECT_ROOT/opensef/build/opensef-shell-qt/osf-shell-qt-v2" > "$LOG_DIR/shell.log" 2>&1 &
SHELL_PID=$!
sleep 3

# 3. Start Filer App
echo "Launching Filer..."
"$PROJECT_ROOT/opensef/build/apps/osf-filer-native/osf-filer-native" > "$LOG_DIR/filer.log" 2>&1 &
FILER_PID=$!

echo "------------------------------------------------"
echo " VitusOS Ares is ACTIVE"
echo " Compositor (PID: $COMP_PID) outputting to Windows"
echo " Shell (PID: $SHELL_PID) running inside Ares"
echo " Filer (PID: $FILER_PID) running inside Ares"
echo "------------------------------------------------"

# Monitor loop
while true; do
    if ! kill -0 $COMP_PID 2>/dev/null; then
        echo "Compositor died. Exiting."
        pkill -P $$
        exit 1
    fi
    sleep 5
done
