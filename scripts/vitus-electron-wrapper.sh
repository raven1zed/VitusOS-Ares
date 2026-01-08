#!/bin/bash
# vitus-electron-wrapper.sh - Wrapper script for Electron applications
# 
# Enables global menu support for Electron apps via built-in DBusMenu.
# This script sets appropriate environment variables before launching the app.
#
# Usage: vitus-electron-wrapper.sh <electron-application> [args...]
#
# Supported apps: Discord, VS Code, Slack, Spotify, etc.

# Enable Unity menubar for Electron
export UBUNTU_MENUPROXY=1

# Use native Wayland if available
export ELECTRON_OZONE_PLATFORM_HINT=wayland

# Enable DBusMenu support (built into Electron)
export XDG_CURRENT_DESKTOP=Unity

# Ensure proper scaling
export GDK_SCALE=1

# Ensure DBus session is available
if [ -z "$DBUS_SESSION_BUS_ADDRESS" ]; then
    eval $(dbus-launch --sh-syntax)
fi

# Launch the application with menubar enabled
exec "$@" --enable-features=UseOzonePlatform --ozone-platform=wayland
