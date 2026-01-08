/**
 * Main.qml - VitusOS Ares Shell Root Component
 * 
 * Layered composition of shell elements:
 * - Panel (top, global menu)
 * - Dock (bottom, app launcher)
 * - Multitask overlay (on demand)
 */

import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    id: shellWindow
    
    // Full screen layer shell surface
    width: Screen.width
    height: Screen.height
    visible: true
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    color: "transparent"
    
    // Panel (Top)
    Panel {
        id: panel
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }
    
    // Dock (Bottom)
    Dock {
        id: dock
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
    
    // Multitask Overlay
    Multitask {
        id: multitask
        anchors.fill: parent
        visible: multitaskController.active
    }
    
    // Keyboard shortcuts
    Shortcut {
        sequence: "Super+Tab"
        onActivated: multitaskController.toggle()
    }
    
    Shortcut {
        sequence: "Tab"
        enabled: multitaskController.active
        onActivated: multitaskController.selectNext()
    }
    
    Shortcut {
        sequence: "Shift+Tab"
        enabled: multitaskController.active
        onActivated: multitaskController.selectPrevious()
    }
    
    Shortcut {
        sequence: "Return"
        enabled: multitaskController.active
        onActivated: multitaskController.activateSelected()
    }
    
    Shortcut {
        sequence: "Escape"
        enabled: multitaskController.active
        onActivated: multitaskController.toggle()
    }
}
