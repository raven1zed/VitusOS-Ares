/**
 * Shell.qml - VitusOS Ares Shell Root
 * 
 * Fullscreen shell with proper background
 */

import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    id: shellWindow
    
    width: Screen.width > 0 ? Screen.width : 1280
    height: Screen.height > 0 ? Screen.height : 720
    visible: true
    flags: Qt.FramelessWindowHint
    title: "VitusOS Ares Shell"
    
    // Use solid background color that matches wallpaper
    // "transparent" doesn't work correctly in WSLg
    color: "#2A1810"  // Dark Mars color as fallback
    
    Connections {
        target: panelController
        function onMenuRequested(index, x, y) {
            globalMenu.showAt(x, y, panelController.globalMenuItems[index].items, index)
        }
    }

    // Component to properly load and show in debug log
    Component.onCompleted: {
        console.log("Shell Window loaded. Size:", width, "x", height)
        console.log("Screen size:", Screen.width, "x", Screen.height)
    }
    
    // Wallpaper - FULL SCREEN, z: 0
    Wallpaper {
        id: wallpaper
        anchors.fill: parent
        z: 0
    }
    
    // Panel - TOP
    AresPanel {
        id: panel
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        z: 10
    }
    
    // Dock - BOTTOM CENTER
    Dock {
        id: dock
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        z: 10
    }
    
    // Multitask Overlay
    Multitask {
        id: multitask
        anchors.fill: parent
        visible: multitaskController.active
        z: 100
    }
    
    // Global Dropdown Menu
    DropdownMenu {
        id: globalMenu
        z: 150
    }
    
    Shortcut {
        sequence: "Super+Tab"
        onActivated: multitaskController.toggle()
    }
    
    Shortcut {
        sequence: "Escape"
        enabled: multitaskController.active
        onActivated: multitaskController.toggle()
    }
}
