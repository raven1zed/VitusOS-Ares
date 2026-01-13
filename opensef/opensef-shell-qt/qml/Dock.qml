/**
 * Dock.qml - Compact Application Dock
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

Item {
    id: dockRoot
    
    width: parent.width
    height: 90 // Larger hit area


    
    // Shadow behind pill
    Rectangle {
        width: dockPill.width
        height: dockPill.height
        radius: 16
        anchors.centerIn: dockPill
        anchors.verticalCenterOffset: 3
        color: "#20000000"
        z: -1
    }
    
    // CERAMIC DOCK PILL (16px radius per Ares Design Reference)
    Rectangle {
        id: dockPill
        
        width: Math.max(dockRow.width + 24, 160)
        height: 64
        radius: 16 // Ares standard
        
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 4
        
        color: "#FAFAF9" // Warm Ceramic
        border.width: 0.5
        border.color: "#E0E0E0"
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
        anchors.topMargin: 4
        spacing: 6 // Match icons
        z: 100 // Ensure visibility
        
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
                    color: "#E85D04"
                    anchors.centerIn: parent
                }
            }
        }
    }
}
