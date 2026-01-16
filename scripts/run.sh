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

if [ ! -f "opensef/opensef-shell-qt/build/osf-shell-qt-v2" ]; then
    echo "❌ Shell not built! Run ./scripts/build.sh first"
    exit 1
fi

echo "🚀 Starting openSEF Desktop..."
echo ""
echo "This will launch in nested Wayland mode (for testing)"
echo "Press Ctrl+C to stop"
echo ""

# Set library path for shared components
export LD_LIBRARY_PATH="$(pwd)/opensef/opensef-framework/build:$(pwd)/opensef/opensef-core/build:$(pwd)/opensef/opensef-base/build:$(pwd)/opensef/opensef-gnustep/build:$LD_LIBRARY_PATH"

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

# Run in nested Wayland mode
WLR_BACKENDS=wayland \
WLR_DEBUG=1 \
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

QT_QPA_PLATFORM=wayland \
QSG_RHI_BACKEND=opengl \
QT_WAYLAND_DISABLE_WINDOWDECORATION=1 \
./opensef/opensef-shell-qt/build/osf-shell-qt-v2 &

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
