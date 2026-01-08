/**
 * SystemTray.qml - macOS-style System Tray
 * 
 * Minimalist icon-only tray with:
 * - Volume icon (with mute state)
 * - Battery icon (with level)
 * - Network status (future)
 */

import QtQuick 2.15
import QtQuick.Controls 2.15

Row {
    id: systemTray
    spacing: 12
    
    // Volume icon
    Item {
        width: 16
        height: 16
        anchors.verticalCenter: parent.verticalCenter
        
        Text {
            anchors.centerIn: parent
            text: systemTrayController.isMuted ? "🔇" : 
                  systemTrayController.volume > 50 ? "🔊" : 
                  systemTrayController.volume > 0 ? "🔉" : "🔈"
            font.pixelSize: 14
        }
        
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: systemTrayController.toggleMute()
        }
    }
    
    // Battery icon
    Item {
        width: 24
        height: 16
        anchors.verticalCenter: parent.verticalCenter
        
        Rectangle {
            id: batteryOutline
            anchors.fill: parent
            anchors.rightMargin: 2
            radius: 3
            color: "transparent"
            border.width: 1.5
            border.color: "#1A1A1A"
            
            // Battery fill
            Rectangle {
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 2
                width: (parent.width - 4) * systemTrayController.batteryLevel / 100
                radius: 1
                color: systemTrayController.batteryLevel < 20 ? "#E53935" :
                       systemTrayController.batteryLevel < 50 ? "#FFC107" : "#4CAF50"
            }
        }
        
        // Battery nub
        Rectangle {
            width: 2
            height: 6
            radius: 1
            color: "#1A1A1A"
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
        }
        
        // Charging indicator
        Text {
            visible: systemTrayController.isCharging
            anchors.centerIn: batteryOutline
            text: "⚡"
            font.pixelSize: 8
            color: "#1A1A1A"
        }
    }
}
