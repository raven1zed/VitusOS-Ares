/**
 * WindowDecorations.qml - Universal Window Titlebar Component
 * 
 * Provides consistent Ares Design Language decorations for ALL windows:
 * - Circular traffic light buttons (Close/Minimize/Maximize)
 * - Draggable titlebar
 * - Rounded corners (16px)
 * - Warm Ceramic background
 */

import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: decorations
    
    // Properties
    property string windowTitle: ""
    property bool showTrafficLights: true
    property bool isActive: true
    property var targetWindow: null  // The Window to control
    
    // Dimensions (Ares Design Language)
    height: 32
    color: isActive ? "#FAFAF9" : "#F5F5F5"  // Warm Ceramic / Slight gray when inactive
    
    // Rounded top corners only
    radius: 16
    
    // Signals for external handling
    signal closeClicked()
    signal minimizeClicked()
    signal maximizeClicked()
    signal dragStarted(real mouseX, real mouseY)
    signal dragged(real deltaX, real deltaY)
    
    // Traffic Lights Container
    Row {
        id: trafficLights
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.verticalCenter: parent.verticalCenter
        spacing: 8
        visible: showTrafficLights
        
        // Close Button (Space Orange)
        TrafficLightButton {
            buttonType: "close"
            baseColor: "#E85D04"
            hoverColor: "#FF6347"
            symbol: "×"
            active: decorations.isActive
            onClicked: {
                if (targetWindow) targetWindow.close()
                decorations.closeClicked()
            }
        }
        
        // Minimize Button (Mars Gold)
        TrafficLightButton {
            buttonType: "minimize"
            baseColor: "#D4A93E"
            hoverColor: "#FFE066"
            symbol: "−"
            active: decorations.isActive
            onClicked: {
                if (targetWindow) targetWindow.showMinimized()
                decorations.minimizeClicked()
            }
        }
        
        // Maximize Button (Mission Blue)
        TrafficLightButton {
            buttonType: "maximize"
            baseColor: "#3D5A80"
            hoverColor: "#3395FF"
            symbol: "□"
            active: decorations.isActive
            onClicked: {
                if (targetWindow) {
                    if (targetWindow.visibility === Window.Maximized) {
                        targetWindow.showNormal()
                    } else {
                        targetWindow.showMaximized()
                    }
                }
                decorations.maximizeClicked()
            }
        }
    }
    
    // Window Title (centered)
    Text {
        anchors.centerIn: parent
        text: windowTitle
        color: isActive ? "#1A1A1A" : "#666666"
        font.family: "Inter"
        font.pixelSize: 13
        font.weight: Font.DemiBold
        elide: Text.ElideMiddle
        maximumLineCount: 1
        width: Math.min(implicitWidth, parent.width - 180)  // Leave space for buttons
    }
    
    // Drag Area (whole titlebar except buttons)
    MouseArea {
        id: dragArea
        anchors.fill: parent
        anchors.leftMargin: showTrafficLights ? 80 : 0  // Skip button area
        
        property point clickPos: Qt.point(0, 0)
        
        onPressed: (mouse) => {
            clickPos = Qt.point(mouse.x, mouse.y)
            decorations.dragStarted(mouse.x, mouse.y)
        }
        
        onPositionChanged: (mouse) => {
            if (pressed && targetWindow) {
                var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
                targetWindow.x += delta.x
                targetWindow.y += delta.y
                decorations.dragged(delta.x, delta.y)
            }
        }
        
        // Double-click to maximize/restore
        onDoubleClicked: {
            if (targetWindow) {
                if (targetWindow.visibility === Window.Maximized) {
                    targetWindow.showNormal()
                } else {
                    targetWindow.showMaximized()
                }
            }
        }
    }
}
