/**
 * WindowDecorations.qml - Universal Window Titlebar Component
 * 
 * Provides consistent Ares Design Language decorations for ALL windows:
 * - Circular traffic light buttons (Close/Minimize/Maximize)
 * - Draggable titlebar
 * - Rounded corners (16px)
 * - Warm Ceramic background
 */

import QtQuick
import QtQuick.Controls
import OpenSEF.AppKit 1.0

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
    
    // Rounded top corners standardized to 16 (Ares Design)
    radius: 16
    
    // Cover bottom rounded corners to join perfectly with content
    Rectangle {
        height: parent.radius
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        color: parent.color
        visible: parent.radius > 0
    }
    
    // Signals for external handling
    signal closeClicked()
    signal minimizeClicked()
    signal maximizeClicked()
    signal dragStarted(real mouseX, real mouseY)
    signal dragged(real deltaX, real deltaY)
    
    // Drag Area (whole titlebar)
    MouseArea {
        id: dragArea
        anchors.fill: parent
        z: 0
        
        onPressed: (mouse) => {
            if (targetWindow) {
                targetWindow.startSystemMove()
            }
        }
        
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

    // Traffic Lights Container (GNUstep Native) - Z: 1 to be on top of drag area
    Row {
        id: trafficLights
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.verticalCenter: parent.verticalCenter
        spacing: 12
        visible: showTrafficLights
        z: 1
        
        // Close Button
        NSWindowButton {
            buttonType: NSWindowButton.CloseButton
            width: 12
            height: 12
            onClicked: {
                if (targetWindow) targetWindow.close()
                decorations.closeClicked()
            }
        }
        
        // Minimize Button
        NSWindowButton {
            buttonType: NSWindowButton.MinimizeButton
            width: 12
            height: 12
            onClicked: {
                if (targetWindow) targetWindow.showMinimized()
                decorations.minimizeClicked()
            }
        }
        
        // Maximize Button
        NSWindowButton {
            buttonType: NSWindowButton.MaximizeButton
            width: 12
            height: 12
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
        z: 1 // Above drag area
    }
}
