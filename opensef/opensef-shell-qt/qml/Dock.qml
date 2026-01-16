/**
 * Dock.qml - Compact Application Dock (FIXED VERSION)
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Effects

Item {
    id: dockRoot
    
    width: parent.width
    height: 100

    // Hit Area for Dock Reveal (Bottom edge of screen)
    MouseArea {
        id: mainMouseArea
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 60 // LARGE trigger for robust discovery
        hoverEnabled: true
        z: 1000
        propagateComposedEvents: true
        onClicked: mouse.accepted = false
    }
    
    // Shadow behind pill
    Rectangle {
        width: dockPill.width
        height: dockPill.height
        radius: 16
        x: dockPill.x
        y: dockPill.y + 3
        color: "#20000000"
        opacity: dockPill.y < 50 ? 1 : 0
        z: -1
    }
    
    // CERAMIC DOCK PILL
    Rectangle {
        id: dockPill
        
        width: Math.max(dockRow.width + 24, 160)
        height: 64
        radius: 16
        
        anchors.horizontalCenter: parent.horizontalCenter
        
        // SIMPLE REVEAL: Show dock when mouse is at bottom OR no windows overlap
        property bool showDock: mainMouseArea.containsMouse || mouseArea.containsMouse || !dockController.isHidden
        
        y: showDock ? 4 : 120
        
        Behavior on y { NumberAnimation { duration: 250; easing.type: Easing.OutCubic } }
        
        color: "#FAFAF9" // Warm Ceramic
        border.width: 0.5
        border.color: "#E0E0E0"
        
        // Hit Area for Dock Itself
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            propagateComposedEvents: true
            onClicked: mouse.accepted = false
        }

        // Icons
        Row {
            id: dockRow
            anchors.centerIn: parent
            spacing: 6
            
            Repeater {
                model: dockController.dockItems
                
                delegate: Item {
                    id: dockItem
                    width: 36
                    height: 48 
                    
                    property bool isHovered: ma.containsMouse
                    
                    Rectangle {
                        id: iconBg
                        anchors.centerIn: parent
                        width: 32
                        height: 32
                        radius: 8
                        color: "transparent"
                        
                        Image {
                            anchors.fill: parent
                            anchors.margins: 1
                            source: Qt.resolvedUrl("../../../assets/icons/" + modelData.name.toLowerCase() + ".svg")
                            sourceSize: Qt.size(48, 48)
                            smooth: true
                            antialiasing: true
                        }

                        scale: dockItem.isHovered ? 1.2 : 1.0
                        Behavior on scale { NumberAnimation { duration: 150; easing.type: Easing.OutBack } }
                    }
                    
                    MouseArea {
                        id: ma
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: dockController.launchApp(index)
                    }
                }
            }
        }
    }

    // Indicator dots
    Row {
        id: dotsRow
        anchors.horizontalCenter: dockPill.horizontalCenter
        y: dockPill.y + dockPill.height + 4
        spacing: 6
        z: 100
        
        Repeater {
            model: dockController.dockItems
            
            delegate: Item {
                width: 36
                height: 6
                
                Rectangle {
                    visible: modelData.running
                    width: 4
                    height: 4
                    radius: 2
                    color: "#E85D04" // Space Orange
                    anchors.centerIn: parent
                }
            }
        }
    }
}
