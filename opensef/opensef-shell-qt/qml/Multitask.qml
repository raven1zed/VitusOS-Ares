/**
 * Multitask.qml - Apple Mission Control Level Polish
 * 
 * Refined to match Apple's attention to detail
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

Rectangle {
    id: multitaskRoot
    anchors.fill: parent
    
    // Refined warm gradient (more subtle)
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#1C1410" }
        GradientStop { position: 0.6; color: "#241810" }
        GradientStop { position: 1.0; color: "#2A1C10" }
    }
    
    opacity: multitaskController.active ? 1 : 0
    visible: opacity > 0
    
    Behavior on opacity {
        NumberAnimation { duration: 250; easing.type: Easing.OutCubic }
    }

    // Subtle vignette
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "transparent" }
            GradientStop { position: 0.7; color: "transparent" }
            GradientStop { position: 1.0; color: Qt.rgba(0, 0, 0, 0.2) }
        }
    }

    // Header - Apple style (minimal, elegant)
    Item {
        id: header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 80
        
        Text {
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.leftMargin: 50
            anchors.bottomMargin: 15
            text: "Activities"
            font.pixelSize: 28
            font.family: "Inter"
            font.weight: Font.Light
            color: "#FAFAF9"
            opacity: 0.95
        }
        
        // Window count (subtle)
        Text {
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 50
            anchors.bottomMargin: 15
            text: multitaskController.windows.length + (multitaskController.windows.length === 1 ? " window" : " windows")
            font.pixelSize: 12
            font.family: "Inter"
            font.weight: Font.Medium
            color: Qt.rgba(1, 1, 1, 0.4)
            font.letterSpacing: 0.5
        }
    }

    // Empty State (Apple-minimal)
    Column {
        anchors.centerIn: parent
        spacing: 16
        visible: multitaskController.windows.length === 0
        opacity: 0.6
        
        Rectangle {
            width: 64
            height: 64
            radius: 32
            color: Qt.rgba(1, 1, 1, 0.03)
            anchors.horizontalCenter: parent.horizontalCenter
            border.width: 1
            border.color: Qt.rgba(1, 1, 1, 0.05)
            
            Text {
                anchors.centerIn: parent
                text: "⊞"
                font.pixelSize: 32
                color: Qt.rgba(1, 1, 1, 0.15)
            }
        }
        
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "No Open Windows"
            color: Qt.rgba(1, 1, 1, 0.3)
            font.pixelSize: 16
            font.family: "Inter"
            font.weight: Font.Light
        }
    }

    // GRID - Apple Mission Control style
    GridView {
        id: taskGrid
        anchors.fill: parent
        anchors.leftMargin: 50
        anchors.rightMargin: 50
        anchors.topMargin: 100
        anchors.bottomMargin: 70
        
        cellWidth: 350
        cellHeight: 250
        
        clip: true
        model: multitaskController.windows
        
        // Smooth Apple-style scrolling
        flickDeceleration: 4000
        maximumFlickVelocity: 6000
        
        delegate: Item {
            width: 330
            height: 230
            
            // Tile with refined shadow (Apple-level)
            Rectangle {
                id: tile
                anchors.centerIn: parent
                width: parent.width - 16
                height: parent.height - 16
                
                radius: 12 // Apple uses ~12px for window previews
                color: "#1E1E1E"
                
                // Subtle border
                border.width: hoverHandler.hovered ? 1.5 : 0.5
                border.color: hoverHandler.hovered ? "#E85D04" : Qt.rgba(1, 1, 1, 0.08)
                
                // Shadow (refined)
                layer.enabled: true
                layer.effect: DropShadow {
                    transparentBorder: true
                    color: hoverHandler.hovered ? "#60000000" : "#40000000"
                    radius: hoverHandler.hovered ? 24 : 16
                    samples: 25
                    verticalOffset: hoverHandler.hovered ? 12 : 6
                    horizontalOffset: 0
                }
                
                // Content
                Rectangle {
                    anchors.fill: parent
                    anchors.margins: 1
                    radius: 11
                    color: "#2A2A2A"
                    clip: true
                    
                    // Thumbnail
                    Image {
                        anchors.fill: parent
                        anchors.margins: 3
                        source: modelData.thumbnail || ""
                        fillMode: Image.PreserveAspectCrop
                        visible: source != ""
                        opacity: 0.95
                    }
                    
                    // Fallback - elegant letter
                    Rectangle {
                        width: 72
                        height: 72
                        radius: 36
                        anchors.centerIn: parent
                        color: Qt.rgba(1, 1, 1, 0.04)
                        visible: modelData.thumbnail === ""
                        border.width: 1
                        border.color: Qt.rgba(1, 1, 1, 0.06)
                        
                        Text {
                            anchors.centerIn: parent
                            text: (modelData.name || "A").charAt(0).toUpperCase()
                            font.pixelSize: 40
                            font.family: "Inter"
                            font.weight: Font.Light
                            color: Qt.rgba(1, 1, 1, 0.25)
                        }
                    }
                }
                
                // Info bar (Apple style - subtle)
                Rectangle {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    height: 50
                    radius: 12
                    
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "transparent" }
                        GradientStop { position: 0.4; color: Qt.rgba(0, 0, 0, 0.5) }
                        GradientStop { position: 1.0; color: Qt.rgba(0, 0, 0, 0.85) }
                    }
                    
                    Column {
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.margins: 14
                        spacing: 3
                        
                        Text {
                            text: modelData.name || "Application"
                            color: "white"
                            font.pixelSize: 14
                            font.family: "Inter"
                            font.weight: Font.DemiBold
                        }
                        
                        Text {
                            text: modelData.title || ""
                            color: Qt.rgba(1, 1, 1, 0.6)
                            font.pixelSize: 11
                            font.family: "Inter"
                            font.weight: Font.Normal
                            elide: Text.ElideRight
                            width: 270
                        }
                    }
                }
                
                // Close button (Apple style - appears on hover)
                Rectangle {
                    width: 24
                    height: 24
                    radius: 12
                    color: closeHover.hovered ? "#FF3B30" : Qt.rgba(0, 0, 0, 0.5)
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.margins: 10
                    visible: hoverHandler.hovered
                    opacity: closeHover.hovered ? 1.0 : 0.7
                    
                    border.width: closeHover.hovered ? 0 : 0.5
                    border.color: Qt.rgba(1, 1, 1, 0.1)
                    
                    Text {
                        anchors.centerIn: parent
                        text: "×"
                        color: "white"
                        font.pixelSize: 18
                        font.weight: Font.Bold
                    }
                    
                    HoverHandler { id: closeHover }
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            multitaskController.closeWindow(modelData.id)
                            mouse.accepted = true
                        }
                    }
                    
                    Behavior on color { ColorAnimation { duration: 120 } }
                    Behavior on opacity { NumberAnimation { duration: 120 } }
                }
                
                HoverHandler { id: hoverHandler }
                
                // Apple-style subtle scale
                scale: hoverHandler.hovered ? 1.02 : 1.0
                Behavior on scale { 
                    NumberAnimation { duration: 180; easing.type: Easing.OutCubic } 
                }
                
                Behavior on border.width {
                    NumberAnimation { duration: 120 }
                }
                
                Behavior on border.color {
                    ColorAnimation { duration: 120 }
                }
            }
            
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    multitaskController.activateWindow(modelData.id)
                }
            }
        }
        
        // Apple-style entry animation
        populate: Transition {
            NumberAnimation { 
                properties: "opacity,scale" 
                from: 0 
                to: 1 
                duration: 350
                easing.type: Easing.OutCubic
            }
        }
    }
    
    // Bottom hint (Apple-minimal)
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 25
        width: hintText.width + 32
        height: 30
        radius: 15
        color: Qt.rgba(0, 0, 0, 0.25)
        border.width: 0.5
        border.color: Qt.rgba(1, 1, 1, 0.05)
        
        Text {
            id: hintText
            anchors.centerIn: parent
            text: "Click to switch • ESC to close"
            color: Qt.rgba(1, 1, 1, 0.4)
            font.pixelSize: 11
            font.family: "Inter"
            font.weight: Font.Medium
            font.letterSpacing: 0.3
        }
    }
}
