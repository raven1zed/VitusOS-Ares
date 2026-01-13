#!/bin/bash
# VitusOS Ares - Development Runner
# Keeps the desktop running until user presses Ctrl+C

PROJECT_ROOT="/mnt/c/Users/hp/Documents/VitusOS-Ares"
LOG_DIR="$PROJECT_ROOT/logs"
mkdir -p "$LOG_DIR"

echo "=== VitusOS Ares Desktop ==="
echo "Press Ctrl+C to stop"

# Clean previous instances
pkill -9 -f 'osf-'
pkill -9 -f 'weston'
rm -f /mnt/wslg/runtime-dir/wayland-[1-9]*
sleep 2

export XDG_RUNTIME_DIR=/mnt/wslg/runtime-dir
export WAYLAND_DISPLAY=wayland-0
export VITUS_BACKEND=wayland
export QT_QPA_PLATFORM=wayland
export WLR_RENDERER=pixman
export WLR_NO_HARDWARE_CURSORS=1
export LIBGL_ALWAYS_SOFTWARE=1
export QT_QUICK_BACKEND=software

# Trap Ctrl+C for clean exit
cleanup() {
    echo ""
    echo "Shutting down..."
    pkill -9 -f 'osf-'
    echo "Done."
    exit 0
}
trap cleanup SIGINT SIGTERM

# 1. Start Compositor
echo "[1/3] Starting Compositor..."
"$PROJECT_ROOT/opensef/build/opensef-compositor/opensef-compositor" > "$LOG_DIR/comp.log" 2>&1 &
COMP_PID=$!
sleep 5

if ! ps -p $COMP_PID > /dev/null; then
    echo "ERROR: Compositor failed to start"
    cat "$LOG_DIR/comp.log"
    exit 1
fi

WAYLAND_SOCK=$(grep -o "wayland-[0-9]*" "$LOG_DIR/comp.log" | head -n 1)
if [ -z "$WAYLAND_SOCK" ]; then
    echo "ERROR: Wayland socket not found"
    exit 1
fi
export WAYLAND_DISPLAY="$WAYLAND_SOCK"
echo "  Compositor OK ($WAYLAND_DISPLAY)"

# 2. Start Shell
echo "[2/3] Starting Shell..."
"$PROJECT_ROOT/opensef/build/opensef-shell-qt/osf-shell-qt-v2" > "$LOG_DIR/shell.log" 2>&1 &
SHELL_PID=$!
sleep 5

if grep -q "Shell Window loaded" "$LOG_DIR/shell.log"; then
    echo "  Shell OK"
else
    echo "  Shell: Slow start or error (check logs)"
fi

# 3. Start Filer
echo "[3/3] Starting Filer..."
"$PROJECT_ROOT/opensef/build/apps/osf-filer-native/osf-filer-native" > "$LOG_DIR/filer.log" 2>&1 &
FILER_PID=$!
sleep 3

if grep -q "SUCCESS" "$LOG_DIR/filer.log"; then
    echo "  Filer OK"
else
    echo "  Filer: Check logs"
fi

echo ""
echo "=== VitusOS Desktop Running ==="
echo "PIDs: Compositor=$COMP_PID Shell=$SHELL_PID Filer=$FILER_PID"
echo "Logs: $LOG_DIR"
echo ""
echo "Press Ctrl+C to stop the desktop"

# Keep running until Ctrl+C
while true; do
    sleep 10
    # Check if processes are still alive
    if ! ps -p $COMP_PID > /dev/null 2>&1; then
        echo "WARNING: Compositor died!"
        break
    fi
done

cleanup
