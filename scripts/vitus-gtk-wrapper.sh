#!/bin/bash
# vitus-gtk-wrapper.sh - Wrapper script for GTK applications
# 
# Enables global menu support for GTK3/GTK4 apps via gmenu-dbusmenu-proxy.
# This script sets appropriate environment variables before launching the app.
#
# Usage: vitus-gtk-wrapper.sh <gtk-application> [args...]

# Enable Unity menubar export for GTK apps
export UBUNTU_MENUPROXY=1
export GTK_MODULES="unity-gtk-module"

# Enable appmenu support
export APPMENU_DISPLAY_BOTH=0

# For GTK4, use the GtkApplicationWindow with app_menu
export GDK_BACKEND=wayland

# Ensure DBus session is available
if [ -z "$DBUS_SESSION_BUS_ADDRESS" ]; then
    eval $(dbus-launch --sh-syntax)
fi

# Start gmenu-dbusmenu-proxy if not already running
if ! pgrep -x "gmenudbusmenu" > /dev/null; then
    gmenudbusmenu-proxy &
    sleep 0.5
fi

# Launch the application
exec "$@"
