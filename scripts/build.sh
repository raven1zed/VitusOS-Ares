#!/bin/bash
# VitusOS Ares - Build & Test Script
# Run this after 'nix develop'

set -e  # Exit on error

echo "╔════════════════════════════════════════════════════════╗"
echo "║       VitusOS Ares - Build & Test Script               ║"
echo "╚════════════════════════════════════════════════════════╝"
echo ""

# Check if in nix shell
if [ -z "$GNUSTEP_MAKEFILES" ]; then
    echo "❌ Not in nix develop shell!"
    echo "   Run: nix develop"
    exit 1
fi

echo "✅ Nix environment detected"
echo ""

# Configure
echo "📦 Configuring with CMake..."
cmake -B build -G Ninja \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DBUILD_TESTING=ON \
    -DBUILD_APPS=OFF

echo ""

# Build
echo "🔨 Building openSEF..."
cmake --build build --parallel $(nproc)

echo ""
echo "✅ Build complete!"
echo ""

# Check for Wayland
if [ "$XDG_SESSION_TYPE" = "wayland" ]; then
    echo "🖥️  Wayland session detected"
    echo ""
    echo "Running hello-window test..."
    ./opensef/build/test/hello-window
else
    echo "⚠️  Not a Wayland session (XDG_SESSION_TYPE=$XDG_SESSION_TYPE)"
    echo "   hello-window requires Wayland to display"
    echo ""
    echo "   To test visually:"
    echo "   1. Log out"
    echo "   2. Choose 'Plasma (Wayland)' or 'GNOME' at login"
    echo "   3. Run this script again"
    echo ""
    echo "   Build succeeded - binaries are in ./build/"
fi

echo ""
echo "╔════════════════════════════════════════════════════════╗"
echo "║                    Build Summary                       ║"
echo "╠════════════════════════════════════════════════════════╣"
echo "║  Libraries:                                            ║"
echo "║    - opensef/build/opensef-base/libopensef-base.so     ║"
echo "║    - opensef/build/opensef-appkit/libopensef-appkit.so ║"
echo "║    - opensef/build/opensef-backend/libopensef-backend.so║"
echo "║    - opensef/build/opensef-ui/libopensef-ui.so         ║"
echo "║                                                        ║"
echo "║  Test Programs:                                        ║"
echo "║    - opensef/build/test/hello-window                   ║"
echo "╚════════════════════════════════════════════════════════╝"
