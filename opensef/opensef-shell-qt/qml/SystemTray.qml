/**
 * SystemTray.qml - Procedural Icons (No Assets)
 * Guarantees visibility by drawing icons with QML shapes.
 */
import QtQuick 2.15
import QtQuick.Layouts 1.15
import Qt5Compat.GraphicalEffects

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
    
    // CERAMIC SYSTEM TRAY PILL
    Rectangle {
        id: trayPill
        height: 24
        width: trayRow.width + 24
        radius: 12 
        color: "#FAFAF9"
        border.width: 0.5
        border.color: "#E0E0E0"
        anchors.verticalCenter: parent.verticalCenter
        
        // Ensure 4-corner rounding standard (even for simple pill)
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: trayPill.width
                height: trayPill.height
                radius: trayPill.radius
            }
        }
    
        Row {
            id: trayRow
            anchors.centerIn: parent
            spacing: 16
    
            // --- 1. WIFI ICON (Procedural) ---
            Item {
                width: 20; height: 18
                anchors.verticalCenter: parent.verticalCenter
                
                // Signal Arcs (Approximated with transparent circles and borders)
                // Largest Arc
                Rectangle {
                    width: 16; height: 8
                    radius: 8
                    color: "transparent"
                    border.color: "#333333"; border.width: 1.5
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom; anchors.bottomMargin: 4
                    visible: true
                    clip: true // Basic approximation
                }
                 // Dot
                Rectangle {
                    width: 3; height: 3
                    radius: 1.5
                    color: "#333333"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom; anchors.bottomMargin: 2
                }
            }
    
            // --- 2. BATTERY ICON (Procedural) ---
            Item {
                width: 26; height: 12
                anchors.verticalCenter: parent.verticalCenter
                
                // Body
                Rectangle {
                    id: batBody
                    width: 20; height: 10
                    radius: 2.5
                    color: "transparent"
                    border.width: 1.5
                    border.color: "#333333"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    
                    // Level (75%)
                    Rectangle {
                        width: 12; height: 6
                        radius: 1.5
                        color: "#333333"
                        anchors.left: parent.left; anchors.leftMargin: 2
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                
                // Positive Terminal (Nub)
                Rectangle {
                    width: 2; height: 4
                    radius: 1
                    color: "#333333"
                    anchors.left: batBody.right; anchors.leftMargin: 1
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
    
            // --- 3. CONTROL CENTER (Switches) ---
            Item {
                width: 18; height: 18
                anchors.verticalCenter: parent.verticalCenter
                
                Column {
                    anchors.centerIn: parent
                    spacing: 3
                    
                    // Toggle 1
                    Rectangle { width: 14; height: 2; color: "#333333"; radius: 1 }
                    // Toggle 2 (Active)
                    Rectangle { 
                        width: 14; height: 2; color: "#333333"; radius: 1 
                        Rectangle { width: 4; height: 4; radius: 2; color: "#333333"; anchors.verticalCenter: parent.verticalCenter; x: 8 }
                    }
                    // Toggle 3
                    Rectangle { width: 14; height: 2; color: "#333333"; radius: 1 }
                }
            }
        }
    }
}
