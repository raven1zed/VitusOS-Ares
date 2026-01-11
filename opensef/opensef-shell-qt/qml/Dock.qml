/**
 * Dock.qml - Compact Application Dock
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

Item {
    id: dockRoot
    
    width: Screen.width
    height: 70 // Extreme compaction, enough for indicators
    
    // Translucent background pill - COMPACT
    Rectangle {
        id: dockPill
        
        width: Math.max(dockRow.width + 24, 160)
        height: 48 // Thinner
        radius: 14
        
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 4
        
        color: Qt.rgba(1, 1, 1, 0.75)
        border.width: 0.5
        border.color: Qt.rgba(0, 0, 0, 0.1)
        
        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            color: "#25000000"
            radius: 12
            samples: 25
            verticalOffset: 2
        }
    }

    // Icons
    Row {
        id: dockRow
        anchors.centerIn: dockPill
        spacing: 6 // Tighter spacing
        
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
                    color: "transparent" // Use icon alpha
                    
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

    // Indicator dots - ULTRA TIGHT
    Row {
        id: dotsRow
        anchors.horizontalCenter: dockPill.horizontalCenter
        anchors.top: dockPill.bottom
        anchors.topMargin: 2 // Tighter
        spacing: 6 // Match icons
        
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
                    color: "#333333"
                    anchors.centerIn: parent
                }
            }
        }
    }
}
