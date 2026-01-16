#!/bin/bash
# setup.sh - Initial environment setup for VitusOS-Ares development
# Run this once to prepare your development environment

set -e  # Exit on error

echo "╔════════════════════════════════════════════╗"
echo "║  VitusOS Ares - Development Setup         ║"
echo "╚════════════════════════════════════════════╝"
echo ""

# Check if we're in WSL2
if ! grep -qi microsoft /proc/version; then
    echo "⚠️  Warning: Not running in WSL2"
    echo "   VitusOS-Ares is designed for WSL2 development"
    read -p "Continue anyway? (y/N) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# Check for nix
if ! command -v nix &> /dev/null; then
    echo "❌ Nix is not installed!"
    echo ""
    echo "Install Nix with:"
    echo "  sh <(curl -L https://nixos.org/nix/install) --daemon"
    echo ""
    exit 1
fi

echo "✅ Nix found: $(nix --version)"

# Check if we're in the correct directory
if [ ! -f "flake.nix" ]; then
    echo "❌ Error: Must run from VitusOS-Ares root directory"
    echo "   Current directory: $(pwd)"
    exit 1
fi

echo "✅ In correct directory: $(pwd)"

# Enter nix development shell
echo ""
echo "🔧 Entering Nix development shell..."
echo "   This will download all dependencies (nixos-unstable)"
echo ""

nix develop --command bash -c '
    echo "✅ Development environment ready!"
    echo ""
    echo "📦 Installed tools:"
    echo "   Compiler: $(clang --version | head -n1)"
    echo "   CMake: $(cmake --version | head -n1)"
    echo "   Ninja: $(ninja --version)"
    echo "   Qt: $(qmake --version | head -n1)"
    echo ""
    echo "🎯 Next steps:"
    echo "   1. Run: ./scripts/build.sh (to build all components)"
    echo "   2. Run: ./scripts/run.sh (to start openSEF Desktop)"
    echo "   3. Run: ./scripts/test.sh (to verify everything works)"
    echo ""
'

echo ""
echo "✅ Setup complete!"
echo ""
echo "To enter development shell manually:"
echo "  nix develop"
echo ""
