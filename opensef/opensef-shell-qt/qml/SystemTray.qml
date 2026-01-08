/**
 * SystemTray.qml - macOS-style System Tray
 * 
 * Minimalist icon-only tray with:
 * - Dynamic app icons (StatusNotifierItem)
 * - Volume icon (with mute state)
 * - Battery icon (with level)
 */

import QtQuick 2.15
import QtQuick.Controls 2.15

Row {
    id: systemTray
    spacing: 8
    
    // Dynamic app tray icons from StatusNotifierWatcher
    Repeater {
        model: systemTrayController.trayIcons
        
        delegate: Item {
            width: 18
            height: 18
            anchors.verticalCenter: parent.verticalCenter
            
            Image {
                id: appIcon
                anchors.centerIn: parent
                width: 16
                height: 16
                source: modelData.iconName ? "image://icon/" + modelData.iconName : ""
                sourceSize: Qt.size(16, 16)
                visible: status === Image.Ready
            }
            
            // Fallback: colored circle with first letter
            Rectangle {
                visible: appIcon.status !== Image.Ready
                anchors.centerIn: parent
                width: 14
                height: 14
                radius: 7
                color: "#666666"
                
                Text {
                    anchors.centerIn: parent
                    text: modelData.title ? modelData.title.charAt(0).toUpperCase() : "?"
                    color: "white"
                    font.pixelSize: 8
                    font.bold: true
                }
            }
            
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                cursorShape: Qt.PointingHandCursor
                
                onClicked: {
                    if (mouse.button === Qt.RightButton) {
                        systemTrayController.trayIconRightClicked(modelData.id)
                    } else {
                        systemTrayController.trayIconClicked(modelData.id)
                    }
                }
            }
            
            ToolTip {
                visible: parent.containsMouse
                text: modelData.title || ""
                delay: 500
            }
        }
    }
    
    // Separator if there are app icons
    Rectangle {
        visible: systemTrayController.trayIcons.length > 0
        width: 1
        height: 14
        color: "#CCCCCC"
        anchors.verticalCenter: parent.verticalCenter
    }
    
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
        
        Rectangle {
            width: 2
            height: 6
            radius: 1
            color: "#1A1A1A"
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
        }
        
        Text {
            visible: systemTrayController.isCharging
            anchors.centerIn: batteryOutline
            text: "⚡"
            font.pixelSize: 8
            color: "#1A1A1A"
        }
    }
}
