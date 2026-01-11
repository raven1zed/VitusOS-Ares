/**
 * Multitask.qml - Windows 8-style Multitask View
 * 
 * Features:
 * - Large tile grid (400x300px tiles)
 * - Window titles
 * - Keyboard navigation
 * - Click to focus
 * - Hover highlight
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

Rectangle {
    id: multitaskView
    
    color: Qt.rgba(0.1, 0.1, 0.1, 0.9)
    opacity: multitaskController.active ? 1 : 0
    visible: opacity > 0
    
    Behavior on opacity {
        NumberAnimation { duration: 200 }
    }
    
    // Background blur placeholder removed - will be implemented with actual wallpaper integration
    
    // Title
    Text {
        id: title
        text: "Open Windows"
        color: "#FFFFFF"
        font.pixelSize: 24
        font.bold: true
        font.family: "Inter"
        
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 48
    }
    
    // Window grid
    GridView {
        id: windowGrid
        
        anchors.centerIn: parent
        width: Math.min(parent.width * 0.9, cellWidth * 3)
        height: Math.min(parent.height * 0.7, cellHeight * 2)
        
        cellWidth: 420
        cellHeight: 320
        
        model: multitaskController.windows
        
        delegate: Item {
            width: 400
            height: 300
            
            Rectangle {
                id: tile
                anchors.fill: parent
                anchors.margins: 10
                radius: 12
                color: "#2A2A2A"
                
                border.width: multitaskController.selectedIndex === index ? 3 : 0
                border.color: "#D4622A"  // Mars Orange for selection
                
                // Hover highlight
                Rectangle {
                    anchors.fill: parent
                    radius: 12
                    color: "#D4622A"
                    opacity: tileMouseArea.containsMouse ? 0.15 : 0
                    
                    Behavior on opacity {
                        NumberAnimation { duration: 100 }
                    }
                }
                
                // Window thumbnail placeholder
                Rectangle {
                    anchors.fill: parent
                    anchors.margins: 8
                    anchors.topMargin: 36
                    radius: 6
                    color: "#1A1A1A"
                    
                    // Placeholder content
                    Text {
                        anchors.centerIn: parent
                        text: "Window Preview"
                        color: "#666666"
                        font.pixelSize: 14
                    }
                }
                
                // Window title bar
                Rectangle {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 32
                    radius: 12
                    color: "#333333"
                    
                    // Only round top corners
                    Rectangle {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        height: 12
                        color: parent.color
                    }
                    
                    // App icon (placeholder)
                    Rectangle {
                        width: 18
                        height: 18
                        radius: 4
                        color: "#4A9FD4"
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    
                    // Window title
                    Text {
                        anchors.left: parent.left
                        anchors.leftMargin: 36
                        anchors.right: closeButton.left
                        anchors.verticalCenter: parent.verticalCenter
                        
                        text: modelData.title
                        color: "#FFFFFF"
                        font.pixelSize: 13
                        font.bold: true
                        font.family: "Inter"
                        elide: Text.ElideRight
                    }
                    
                    // Close button
                    Rectangle {
                        id: closeButton
                        width: 18
                        height: 18
                        radius: 9
                        color: closeMouseArea.containsMouse ? "#E53935" : "#666666"
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        
                        Text {
                            anchors.centerIn: parent
                            text: "×"
                            color: "white"
                            font.pixelSize: 12
                            font.bold: true
                        }
                        
                        MouseArea {
                            id: closeMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: multitaskController.closeWindow(modelData.id)
                        }
                        
                        Behavior on color {
                            ColorAnimation { duration: 100 }
                        }
                    }
                }
                
                MouseArea {
                    id: tileMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    
                    onClicked: multitaskController.focusWindow(modelData.id)
                    onEntered: multitaskController.selectedIndex = index
                }
                
                // Scale animation on hover
                scale: tileMouseArea.containsMouse ? 1.02 : 1.0
                
                Behavior on scale {
                    NumberAnimation { duration: 100 }
                }
            }
        }
    }
    
    // Escape hint
    Text {
        text: "Press ESC to close • Tab to navigate • Enter to select"
        color: "#666666"
        font.pixelSize: 12
        font.family: "Inter"
        
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 24
    }
    
    // Click outside to close
    MouseArea {
        anchors.fill: parent
        z: -1
        onClicked: multitaskController.toggle()
    }
}
