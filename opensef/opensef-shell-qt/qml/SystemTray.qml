/**
 * SystemTray.qml - Procedural Icons (No Assets)
 * Guarantees visibility by drawing icons with QML shapes.
 */
import QtQuick 2.15
import QtQuick.Layouts 1.15

Item {
    id: systemTrayRoot
    height: 24 // Fits in 28px panel with margin
    width: trayPill.width
    // Alignment handled by parent Row in AresPanel
    
    // Shadow
    Rectangle {
        width: trayPill.width
        height: trayPill.height
        radius: 12 // Slightly smaller than pill
        anchors.centerIn: trayPill
        anchors.verticalCenterOffset: 1
        color: "#10000000"
        z: -1
    }
    
    // CERAMIC SYSTEM TRAY PILL (16px radius per design spec)
    Rectangle {
        id: trayPill
        height: 24
        width: trayRow.width + 24 // Tighter padding
        radius: 12 // Half of height for perfect pill
        color: "#FAFAF9"
        border.width: 0.5
        border.color: "#E0E0E0"
        anchors.verticalCenter: parent.verticalCenter
    
        Row {
            id: trayRow
            anchors.centerIn: parent
            spacing: 16
    
            // --- 1. WIFI ICON (Procedural) ---
            Item {
                width: 20; height: 18
                anchors.verticalCenter: parent.verticalCenter
                
                // Signal Bars
                Row {
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 2
                    
                    Rectangle { width: 3; height: 4; color: "#333333"; radius: 1 }
                    Rectangle { width: 3; height: 8; color: "#333333"; radius: 1 }
                    Rectangle { width: 3; height: 12; color: "#333333"; radius: 1 }
                }
            }
    
            // --- 2. BATTERY ICON (Procedural) ---
            Item {
                width: 24; height: 12
                anchors.verticalCenter: parent.verticalCenter
                
                // Body
                Rectangle {
                    id: batBody
                    width: 20; height: 12
                    radius: 3
                    color: "transparent"
                    border.width: 1.5
                    border.color: "#333333"
                    
                    // Level
                    Rectangle {
                        width: 12; height: 8
                        radius: 1
                        color: "#333333"
                        anchors.left: parent.left
                        anchors.leftMargin: 2
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                
                // Nub
                Rectangle {
                    width: 2; height: 4
                    radius: 1
                    color: "#333333"
                    anchors.left: batBody.right
                    anchors.leftMargin: 1
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
    
            // --- 3. CONTROL CENTER (Procedural) ---
            Item {
                width: 20; height: 20
                anchors.verticalCenter: parent.verticalCenter
                
                // Toggles look
                Column {
                    anchors.centerIn: parent
                    spacing: 2
                    
                    Rectangle { width: 16; height: 2; color: "#333333"; radius: 1 }
                    Rectangle { width: 16; height: 2; color: "#333333"; radius: 1 }
                    Rectangle { width: 16; height: 2; color: "#333333"; radius: 1 }
                }
                // Circle overlay to look like sliders
                Rectangle { width: 5; height: 5; radius: 2.5; color: "#333333"; x: 4; y: 4 }
                Rectangle { width: 5; height: 5; radius: 2.5; color: "#333333"; x: 10; y: 9 }
            }
        }
    }
}
