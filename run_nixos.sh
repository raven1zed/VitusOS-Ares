#!/bin/bash
# run_nixos.sh - Launch VitusOS Ares on NixOS

# 1. Setup Runtime Directory
export XDG_RUNTIME_DIR=/tmp/vitus_runtime
mkdir -p "$XDG_RUNTIME_DIR"
chmod 700 "$XDG_RUNTIME_DIR"
# Clean up old sockets (to prevent connection errors)
rm -f "$XDG_RUNTIME_DIR/wayland-1" "$XDG_RUNTIME_DIR/wayland-1.lock"

# 2. Launch Weston (Nested Wayland Compositor)
# We use 'nix develop' to ensure we have weston and all libs
echo "🚀 Launching Weston..."
nix develop --command bash -c "env XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR weston --backend=x11-backend.so --socket=wayland-1 --width=1280 --height=720 & echo \$! > $XDG_RUNTIME_DIR/weston.pid"

# Wait for socket
echo "⏳ Waiting for Wayland socket..."
for i in {1..50}; do
  if [ -S "$XDG_RUNTIME_DIR/wayland-1" ]; then
    echo "✅ Socket found!"
    break
  fi
  sleep 0.1
done

if [ ! -S "$XDG_RUNTIME_DIR/wayland-1" ]; then
  echo "❌ Error: Wayland socket not found!"
  exit 1
fi

# 3. Build & Launch VitusOS Shell
echo "🔨 Building Project..."
# We assume we are at project root.
# Using 'nix develop' to enter shell, then build, then run.
nix develop --command bash -c "
  mkdir -p opensef/build
  cd opensef/build
  echo '⚙️ Configuring...'
  cmake .. -G Ninja
  echo '🧱 Compiling...'
  ninja
  
  echo '🖥️ Launching Shell...'
  env XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR WAYLAND_DISPLAY=wayland-1 ./opensef-shell/osf-shell
"

# Cleanup
WESTON_PID=$(cat "$XDG_RUNTIME_DIR/weston.pid")
kill $WESTON_PID
rm -rf "$XDG_RUNTIME_DIR"
