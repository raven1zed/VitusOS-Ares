#!/bin/bash
# run_shell.sh - Launch VitusOS shell in WSL

# 1. Set up Environment
export XDG_RUNTIME_DIR=/tmp/xdg_runtime_dir_$$
mkdir -p "$XDG_RUNTIME_DIR"
chmod 700 "$XDG_RUNTIME_DIR"
export DISPLAY=:0

# 2. Check for X Server
if ! xset q &>/dev/null; then
  echo "❌ No X Server detected at $DISPLAY"
  echo "👉 Please install VcXsrv on Windows and launch it with 'Disable Access Control'"
  exit 1
fi

echo "✅ X Server detected!"

# 3. Launch Weston (Nested Compositor)
echo "🚀 Launching Weston..."
# Weston runs in the background
weston --backend=x11-backend.so --socket=wayland-1 &
WESTON_PID=$!

# Wait for socket
export WAYLAND_DISPLAY=wayland-1
echo "⏳ Waiting for Wayland socket..."
while [ ! -e "$XDG_RUNTIME_DIR/$WAYLAND_DISPLAY" ]; do
  sleep 0.1
done

# 4. Launch VitusOS Shell
echo "🖥️  Launching VitusOS Shell..."
# IMPORTANT: CD to build dir so it finds resources!
BUILD_DIR="/mnt/c/Users/hp/Documents/VitusOS Ares/opensef/build"
cd "$BUILD_DIR"

./opensef-shell/osf-shell &
SHELL_PID=$!

# Wait for shell to exit
wait $SHELL_PID

# Cleanup
kill $WESTON_PID
rm -rf "$XDG_RUNTIME_DIR"
