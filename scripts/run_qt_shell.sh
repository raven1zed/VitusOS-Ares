#!/bin/bash
# run_qt_shell.sh - Launch Qt Shell with proper environment
# This script sets up Qt6 Wayland environment before launching osf-shell-qt

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
REPO_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

# Find Qt6 packages in nix store
QTBASE_PATH=$(nix-instantiate --eval -E '(import <nixpkgs> {}).qt6.qtbase.outPath' 2>/dev/null | tr -d '"')
QTWAYLAND_PATH=$(nix-instantiate --eval -E '(import <nixpkgs> {}).qt6.qtwayland.outPath' 2>/dev/null | tr -d '"')
QTDECLARATIVE_PATH=$(nix-instantiate --eval -E '(import <nixpkgs> {}).qt6.qtdeclarative.outPath' 2>/dev/null | tr -d '"')
QT5COMPAT_PATH=$(nix-instantiate --eval -E '(import <nixpkgs> {}).qt6.qt5compat.outPath' 2>/dev/null | tr -d '"')

echo "Qt6 paths:"
echo "  qtbase: $QTBASE_PATH"
echo "  qtwayland: $QTWAYLAND_PATH"
echo "  qtdeclarative: $QTDECLARATIVE_PATH"
echo "  qt5compat: $QT5COMPAT_PATH"
QTSVG_PATH=$(nix-instantiate --eval -E '(import <nixpkgs> {}).qt6.qtsvg.outPath' 2>/dev/null | tr -d '"')
echo "  qtsvg: $QTSVG_PATH"

# Set Qt environment
export QT_QPA_PLATFORM=wayland
export QT_PLUGIN_PATH="$QTBASE_PATH/lib/qt-6/plugins:$QTWAYLAND_PATH/lib/qt-6/plugins:$QTSVG_PATH/lib/qt-6/plugins"
export QML2_IMPORT_PATH="$QTDECLARATIVE_PATH/lib/qt-6/qml:$QT5COMPAT_PATH/lib/qt-6/qml"
export QML_IMPORT_PATH="$QML2_IMPORT_PATH"

echo "QT_PLUGIN_PATH=$QT_PLUGIN_PATH"
echo "QML2_IMPORT_PATH=$QML2_IMPORT_PATH"

# Check for wayland plugin
if [ -f "$QTWAYLAND_PATH/lib/qt-6/plugins/platforms/libqwayland-generic.so" ]; then
    echo "✅ Wayland platform plugin found"
else
    echo "❌ Wayland platform plugin NOT found!"
    exit 1
fi

# Run the Qt shell
exec "$REPO_ROOT/opensef/build/opensef-shell-qt/osf-shell-qt-v2" "$@"
