#!/bin/bash
# run.sh - Start openSEF Desktop for testing
# Launches compositor and shell in WSL2 nested mode

set -e

echo "╔════════════════════════════════════════════╗"
echo "║  VitusOS Ares - Start openSEF Desktop     ║"
echo "╚════════════════════════════════════════════╝"
echo ""

# Check builds exist
if [ ! -f "opensef/opensef-compositor/build/opensef-compositor" ]; then
    echo "❌ Compositor not built! Run ./scripts/build.sh first"
    exit 1
fi

if [ ! -f "opensef/build/opensef-shell-qt/osf-shell-qt-v2" ]; then
    echo "❌ Shell not built! Run ./scripts/build.sh first"
    exit 1
fi

echo "🚀 Starting openSEF Desktop..."
echo ""
echo "This will launch in nested Wayland mode (for testing)"
echo "Press Ctrl+C to stop"
echo ""

# Add build directories and WSL drivers to LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$(pwd)/opensef/opensef-framework/build:$(pwd)/opensef/opensef-core/build:$(pwd)/opensef/opensef-base/build:$(pwd)/opensef/opensef-gnustep/build:/usr/lib/wsl/lib:$LD_LIBRARY_PATH

# Force D3D12 Driver for WSL2 Hardware Acceleration
# Dynamically find the path to d3d12_dri.so (preferring the version matching the system mesa) using fast lookup
D3D12_DRIVER_DIR=$(ls -d /nix/store/*mesa-25.0.7*/lib/dri 2>/dev/null | head -n 1)
if [ -n "$D3D12_DRIVER_DIR" ]; then
    export LIBGL_DRIVERS_PATH=$D3D12_DRIVER_DIR
    export MESA_LOADER_DRIVER_OVERRIDE=d3d12
    export GALLIUM_DRIVER=d3d12
    echo "GPU Acceleration Enabled: Using d3d12 driver from $D3D12_DRIVER_DIR"
else
    echo "WARNING: d3d12_dri.so not found! Falling back to software rendering."
fi

# Export LD_LIBRARY_PATH so all components can find libopensef-framework.so
export LD_LIBRARY_PATH=$PWD/opensef/build/opensef-framework:$LD_LIBRARY_PATH

# Create cleanup function
cleanup() {
    echo ""
    echo "🛑 Stopping openSEF Desktop..."
    kill $COMPOSITOR_PID 2>/dev/null || true
    kill $SHELL_PID 2>/dev/null || true
    exit 0
}

trap cleanup INT TERM

# Start compositor in background
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "1️⃣  Starting Compositor..."
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

# Nested Wayland mode - compositor will attempt GPU rendering
WLR_BACKENDS=wayland \
WLR_NO_HARDWARE_CURSORS=1 \
./opensef/opensef-compositor/build/opensef-compositor &

COMPOSITOR_PID=$!
echo "✅ Compositor started (PID: $COMPOSITOR_PID)"

# Wait for compositor to initialize
echo "⏳ Waiting for compositor to initialize..."
sleep 2

# Check if compositor is still running
if ! kill -0 $COMPOSITOR_PID 2>/dev/null; then
    echo "❌ Compositor failed to start"
    exit 1
fi

# Start shell
echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "2️⃣  Starting Shell..."
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

LD_LIBRARY_PATH=$PWD/opensef/build/opensef-framework:$LD_LIBRARY_PATH \
QT_QPA_PLATFORM=wayland \
QSG_RHI_BACKEND=opengl \
QT_WAYLAND_DISABLE_WINDOWDECORATION=1 \
./opensef/build/opensef-shell-qt/osf-shell-qt-v2 &

SHELL_PID=$!
echo "✅ Shell started (PID: $SHELL_PID)"

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "✅ openSEF Desktop is running!"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""
echo "You should see:"
echo "  • Panel at top (with Filer menu)"
echo "  • Dock at bottom (with app icons)"
echo "  • SystemTray on right (clickable dropdown)"
echo "  • Wallpaper background"
echo ""
echo "To launch Filer:"
echo "  ./opensef/build/apps/osf-filer-native/osf-filer-native"
echo ""
echo "Press Ctrl+C to stop"
echo ""

# Wait for processes
wait $COMPOSITOR_PID $SHELL_PID
