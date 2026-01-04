#!/bin/bash
# run_shell.sh - Launch VitusOS shell in WSL

# 1. Set up Environment
export XDG_RUNTIME_DIR=/tmp/xdg_runtime_dir_$$
mkdir -p "$XDG_RUNTIME_DIR"
chmod 700 "$XDG_RUNTIME_DIR"
# 2. Check for X Server (Robust Diagnostic)
export DISPLAY=:0
echo "🔍 Checking X Server connection..."

# Function to test X server connection
check_display() {
  local TARGET_DISPLAY=$1
  if timeout 1s xset -display "$TARGET_DISPLAY" q &>/dev/null; then
    return 0
  fi
  return 1
}

# Try standard IP loopback first (most reliable on WSL1)
if check_display 127.0.0.1:0; then
  export DISPLAY=127.0.0.1:0
# Try localhost
elif check_display localhost:0; then
  export DISPLAY=localhost:0
# Default fallback
elif check_display :0; then
   export DISPLAY=:0
else
  echo "❌ CONNECTION FAILED: VcXsrv is not reachable."
  echo "   (Diagnostic: 'xset' failed to connect to 127.0.0.1:0)"
  echo "👉 RE-CHECK FIREWALL: Start > 'Allow an app...' > VcXsrv > ☑️ Private & ☑️ Public"
  exit 1
fi
echo "✅ Connected to X Server at $DISPLAY"

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
