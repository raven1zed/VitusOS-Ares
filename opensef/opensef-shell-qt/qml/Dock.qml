/**
 * Dock.qml - Application Dock
 * 
 * VitusOS Ares bottom dock with:
 * - Centered pill shape
 * - App icons with hover effects
 * - Running app indicators (dot)
 * - Autohide support
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Item {
    id: dock
    
    width: dockPill.width
    height: 120
    
    // Autohide animation
    y: dockController.isHidden ? 100 : 0
    
    Behavior on y {
        NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
    }
    
    // Dock pill background
    Rectangle {
        id: dockPill
        
        width: Math.max(dockRow.width + 24, 200)
        height: 64
        radius: 12
        
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 12
        
        // Glassmorphism background
        color: Qt.rgba(0.15, 0.15, 0.15, 0.85)
        border.width: 1
        border.color: Qt.rgba(1, 1, 1, 0.1)
        
        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            color: "#50000000"
            radius: 12
            samples: 25
            verticalOffset: 4
        }
        
        Row {
            id: dockRow
            anchors.centerIn: parent
            spacing: 8
            
            Repeater {
                model: dockController.dockItems
                
                delegate: Item {
                    width: 48
                    height: 48
                    
                    // Icon background (colored square with rounded corners)
                    Rectangle {
                        id: iconBg
                        anchors.centerIn: parent
                        width: dockController.hoveredIndex === index ? 44 : 40
                        height: width
                        radius: 8
                        color: modelData.color
                        
                        // Icon (using system icon or placeholder)
                        Image {
                            anchors.centerIn: parent
                            width: 24
                            height: 24
                            source: "image://icon/" + modelData.icon
                            sourceSize: Qt.size(24, 24)
                            
                            // Fallback to text if icon not found
                            visible: status === Image.Ready
                        }
                        
                        // Fallback: First letter of app name
                        Text {
                            anchors.centerIn: parent
                            text: modelData.name.charAt(0)
                            color: "white"
                            font.pixelSize: 18
                            font.bold: true
                            visible: parent.children[0].status !== Image.Ready
                        }
                        
                        Behavior on width {
                            NumberAnimation { duration: 100 }
                        }
                        
                        Behavior on height {
                            NumberAnimation { duration: 100 }
                        }
                    }
                    
                    // Running indicator dot
                    Rectangle {
                        visible: modelData.running
                        width: 4
                        height: 4
                        radius: 2
                        color: "#FFFFFF"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                    }
                    
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        
                        onEntered: dockController.hoveredIndex = index
                        onExited: dockController.hoveredIndex = -1
                        onClicked: dockController.launchApp(index)
                    }
                    
                    // Tooltip
                    ToolTip {
                        visible: dockController.hoveredIndex === index
                        text: modelData.name
                        delay: 500
                    }
                }
            }
        }
    }
}
