#!/bin/sh
# start-opensef.sh - Session Launcher

# Set up environment variables
export XDG_SESSION_TYPE=wayland
export XDG_CURRENT_DESKTOP=openSEF
export MOZ_ENABLE_WAYLAND=1
export QT_QPA_PLATFORM=wayland
export GDK_BACKEND=wayland
export SDL_VIDEODRIVER=wayland
export CLUTTER_BACKEND=wayland

# Fix for Java apps
export _JAVA_AWT_WM_NONREPARENTING=1

# Run the compositor
# Assuming opensef-compositor is in the path or relative
# This script is likely installed to /usr/bin or similar in production

# Check if we are in dev environment (CMake build directory)
if [ -f "./build/opensef/opensef-compositor/opensef-compositor" ]; then
    COMPOSITOR="./build/opensef/opensef-compositor/opensef-compositor"
    # In dev mode, use relative paths to build artifacts
    SHELL_DIR="./build/opensef/opensef-shell"
    STARTUP_CMD="$SHELL_DIR/osf-shell"
else
    COMPOSITOR="opensef-compositor"
    # In installed mode, assume clients are in PATH
    STARTUP_CMD="osf-shell"
fi

echo "Starting openSEF..."
exec $COMPOSITOR -s "$STARTUP_CMD"
