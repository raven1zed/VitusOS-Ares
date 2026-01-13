/**
 * TrafficLightButton.qml - Individual Traffic Light Button
 * 
 * 12px circular button with hover effects and symbol display
 */

import QtQuick 2.15

Rectangle {
    id: button
    
    // Properties
    property string buttonType: "close"  // "close", "minimize", "maximize"
    property color baseColor: "#E85D04"
    property color hoverColor: "#FF6347"
    property string symbol: "×"
    property bool active: true
    
    // Size (12px diameter circle)
    width: 12
    height: 12
    radius: 6  // Perfect circle
    
    // Color based on state
    color: mouseArea.containsMouse ? hoverColor : baseColor
    opacity: active ? 1.0 : 0.5
    
    // Smooth transitions
    Behavior on color { ColorAnimation { duration: 100 } }
    Behavior on opacity { NumberAnimation { duration: 150 } }
    Behavior on scale { NumberAnimation { duration: 100; easing.type: Easing.OutQuad } }
    
    // Pressed state
    scale: mouseArea.pressed ? 0.9 : 1.0
    
    // Symbol (shows on hover)
    Text {
        id: symbolText
        anchors.centerIn: parent
        text: symbol
        color: "white"
        font.pixelSize: symbol === "□" ? 6 : 10
        font.bold: true
        opacity: mouseArea.containsMouse ? 1.0 : 0.0
        
        Behavior on opacity { NumberAnimation { duration: 100 } }
    }
    
    // Alternative symbol for maximize (box outline)
    Rectangle {
        id: maxSymbol
        anchors.centerIn: parent
        width: 6
        height: 6
        color: "transparent"
        border.width: 1.5
        border.color: "white"
        visible: buttonType === "maximize" && mouseArea.containsMouse
        opacity: mouseArea.containsMouse ? 1.0 : 0.0
        
        Behavior on opacity { NumberAnimation { duration: 100 } }
    }
    
    // Minimize line symbol
    Rectangle {
        id: minSymbol
        anchors.centerIn: parent
        width: 6
        height: 2
        color: "white"
        visible: buttonType === "minimize" && mouseArea.containsMouse
        opacity: mouseArea.containsMouse ? 1.0 : 0.0
        
        Behavior on opacity { NumberAnimation { duration: 100 } }
    }
    
    // Close X symbol (using two rotated rectangles)
    Item {
        id: closeSymbol
        anchors.centerIn: parent
        width: 6
        height: 6
        visible: buttonType === "close" && mouseArea.containsMouse
        opacity: mouseArea.containsMouse ? 1.0 : 0.0
        
        Behavior on opacity { NumberAnimation { duration: 100 } }
        
        Rectangle {
            anchors.centerIn: parent
            width: 8
            height: 1.5
            color: "white"
            rotation: 45
        }
        
        Rectangle {
            anchors.centerIn: parent
            width: 8
            height: 1.5
            color: "white"
            rotation: -45
        }
    }
    
    // Signal
    signal clicked()
    
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        
        onClicked: button.clicked()
    }
}
