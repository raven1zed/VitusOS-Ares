#!/bin/bash
# vitus-qt-wrapper.sh - Wrapper script for Qt applications
# 
# Enables global menu support for Qt5/Qt6 apps via QDBusMenu.
# Qt apps have built-in DBusMenu support that just needs to be enabled.
#
# Usage: vitus-qt-wrapper.sh <qt-application> [args...]

# Enable Unity menubar export for Qt apps
export UBUNTU_MENUPROXY=1
export QT_QPA_PLATFORMTHEME=appmenu-qt5

# Use Wayland for modern Qt apps
export QT_QPA_PLATFORM=wayland

# Enable DBus integration
export QT_ACCESSIBILITY=1

# Ensure DBus session is available
if [ -z "$DBUS_SESSION_BUS_ADDRESS" ]; then
    eval $(dbus-launch --sh-syntax)
fi

# Launch the application
exec "$@"
