#!/bin/bash
# Definitive Self-Test for VitusOS Ares
# This script will run the stack and capture PROOF of life.

PROJECT_ROOT="/mnt/c/Users/hp/Documents/VitusOS-Ares"
LOG_DIR="$PROJECT_ROOT/logs"
mkdir -p "$LOG_DIR"

echo "--- DEFINITIVE SELF-TEST STARTING ---"

# Clean
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

# 1. Start Compositor
echo "Starting Compositor..."
"$PROJECT_ROOT/opensef/build/opensef-compositor/opensef-compositor" > "$LOG_DIR/TEST_comp.log" 2>&1 &
COMP_PID=$!
sleep 5

# Check if compositor is alive
if ! ps -p $COMP_PID > /dev/null; then
    echo "CRITICAL: Compositor died immediately."
    cat "$LOG_DIR/TEST_comp.log"
    exit 1
fi

WAYLAND_SOCK=$(grep -o "wayland-[0-9]*" "$LOG_DIR/TEST_comp.log" | head -n 1)
if [ -z "$WAYLAND_SOCK" ]; then
    echo "ERROR: Wayland socket not found in compositor log."
    exit 1
fi
export WAYLAND_DISPLAY="$WAYLAND_SOCK"
echo "Compositor OK on $WAYLAND_DISPLAY"

# 2. Start Shell
echo "Starting Shell..."
"$PROJECT_ROOT/opensef/build/opensef-shell-qt/osf-shell-qt-v2" > "$LOG_DIR/TEST_shell.log" 2>&1 &
SHELL_PID=$!
sleep 5

if grep -q "qml: Shell Window loaded" "$LOG_DIR/TEST_shell.log"; then
    echo "Shell: OK"
else
    echo "Shell: FAILED or slow."
    cat "$LOG_DIR/TEST_shell.log"
fi

# 3. Start Filer
echo "Starting Filer..."
"$PROJECT_ROOT/opensef/build/apps/osf-filer-native/osf-filer-native" > "$LOG_DIR/TEST_filer.log" 2>&1 &
FILER_PID=$!
sleep 5

echo "--- TEST COMPLETE ---"
echo "Processes are still running for verification."
echo "Comp: $COMP_PID, Shell: $SHELL_PID, Filer: $FILER_PID"

# Capture evidence
echo "--- LOG EVIDENCE ---"
cat "$LOG_DIR/TEST_shell.log" | grep "qml:"
cat "$LOG_DIR/TEST_filer.log" | grep "Filer"

# Keep alive for 30s to check for delayed crashes (like double free)
echo "Monitoring for 30s..."
sleep 30

if ! ps -p $COMP_PID > /dev/null; then echo "Compositor CRASHED (Double free check)"; fi
if ! ps -p $SHELL_PID > /dev/null; then echo "Shell CRASHED"; fi

pkill -9 -f 'osf-'
echo "Cleanup done."
