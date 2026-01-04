#!/bin/bash
# run_force.sh - Launch VitusOS shell (Diagnostic Bypass)

# Force display to localhost (most likely to work if Firewall is fixed)
export DISPLAY=127.0.0.1:0

echo "🚀 Launching Weston (Checking disabled)..."
export XDG_RUNTIME_DIR=/tmp/xdg_runtime_dir_$$
mkdir -p "$XDG_RUNTIME_DIR"
chmod 700 "$XDG_RUNTIME_DIR"

# Launch Weston
weston --backend=x11-backend.so --socket=wayland-1 &
WESTON_PID=$!

echo "⏳ Waiting for Wayland socket..."
sleep 2

# Launch VitusOS
export WAYLAND_DISPLAY=wayland-1
cd "/mnt/c/Users/hp/Documents/VitusOS Ares/opensef/build"
./opensef-shell/osf-shell &
SHELL_PID=$!

wait $SHELL_PID
kill $WESTON_PID
rm -rf "$XDG_RUNTIME_DIR"
