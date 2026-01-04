#!/bin/bash
# Quick setup script for fresh clone

set -e

echo "VitusOS Ares - Quick Setup"
echo ""

# Check for git
if ! command -v git &> /dev/null; then
    echo "❌ git not found"
    exit 1
fi

# Check for nix
if ! command -v nix &> /dev/null; then
    echo "❌ nix not found"
    echo "   Install Nix: https://nixos.org/download"
    exit 1
fi

# Initialize submodules
echo "📦 Initializing git submodules..."
git submodule update --init --recursive

echo ""
echo "✅ Submodules initialized"
echo ""
echo "Next steps:"
echo "  1. nix develop"
echo "  2. ./scripts/build.sh"
echo ""
echo "Or manually:"
echo "  nix develop"
echo "  cmake -B build -G Ninja"
echo "  cmake --build build"
